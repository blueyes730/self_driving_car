import carla
import queue
import os

def main():
    # set up carla client to local server
    client = carla.Client('localhost', 2000)
    client.set_timeout(10.0)

    # get the world
    world = client.get_world()
    blueprint_library = world.get_blueprint_library()

    # set up in sync mode
    original_settings = world.get_settings()
    settings = world.get_settings()
    settings.synchronous_mode = True
    settings.fixed_delta_seconds = 0.05 # 0.05 is 20 FPS
    world.apply_settings(settings)

    actors = []
    sensor_queues = []

    # set up ego vehicle
    try: 
        vehicle_bp = blueprint_library.find('vehicle.dodge.charger_2020')
        vehicle_bp.set_attribute("role_name", "hero")

        spawn_point = world.get_map().get_spawn_points()[0]
        ego_vehicle = world.spawn_actor(vehicle_bp, spawn_point)
        actors.append(ego_vehicle)
        print(f"Spawned ego vehicle: {ego_vehicle}")

        # helper to attach sensors to the ego vehicle
        # For example, you might want to attach a camera or LIDAR sensor

        def attach_sensor(sensor_name, bp, transform):
            sensor = world.spawn_actor(bp, transform, attach_to=ego_vehicle)
            actors.append(sensor)
            q = queue.Queue()
            sensor.listen(q.put)
            sensor_queues.append((sensor_name, q))

            print(f"Attached sensor: {sensor_name}")
            return sensor


        # -------------------------
        # RGB camera blueprint
        # -------------------------
        rgb_bp = blueprint_library.find("sensor.camera.rgb")
        rgb_bp.set_attribute("image_size_x", "1280")
        rgb_bp.set_attribute("image_size_y", "720")
        rgb_bp.set_attribute("fov", "90")
        rgb_bp.set_attribute("sensor_tick", "0.05")

        # CARLA convention:
        # x = forward, y = right, z = up
        # yaw: 0 front, 90 right, -90 left, 180 back

        front_cam_tf = carla.Transform(
            carla.Location(x=1.8, y=0.0, z=1.6),
            carla.Rotation(pitch=0.0, yaw=0.0, roll=0.0)
        )

        back_cam_tf = carla.Transform(
            carla.Location(x=-1.8, y=0.0, z=1.6),
            carla.Rotation(pitch=0.0, yaw=180.0, roll=0.0)
        )

        left_cam_tf = carla.Transform(
            carla.Location(x=0.0, y=-0.9, z=1.5),
            carla.Rotation(pitch=0.0, yaw=-90.0, roll=0.0)
        )

        right_cam_tf = carla.Transform(
            carla.Location(x=0.0, y=0.9, z=1.5),
            carla.Rotation(pitch=0.0, yaw=90.0, roll=0.0)
        )

        front_cam = attach_sensor("front_rgb", rgb_bp, front_cam_tf)
        back_cam = attach_sensor("back_rgb", rgb_bp, back_cam_tf)
        left_cam = attach_sensor("left_rgb", rgb_bp, left_cam_tf)
        right_cam = attach_sensor("right_rgb", rgb_bp, right_cam_tf)

        # -------------------------
        # LiDAR on roof
        # -------------------------
        lidar_bp = blueprint_library.find("sensor.lidar.ray_cast")
        lidar_bp.set_attribute("channels", "64")
        lidar_bp.set_attribute("range", "100.0")
        lidar_bp.set_attribute("points_per_second", "1300000")
        lidar_bp.set_attribute("rotation_frequency", "20.0")
        lidar_bp.set_attribute("upper_fov", "10.0")
        lidar_bp.set_attribute("lower_fov", "-30.0")
        lidar_bp.set_attribute("sensor_tick", "0.05")

        roof_lidar_tf = carla.Transform(
            carla.Location(x=0.0, y=0.0, z=2.4),
            carla.Rotation(pitch=0.0, yaw=0.0, roll=0.0)
        )

        lidar = attach_sensor("roof_lidar", lidar_bp, roof_lidar_tf)

        # -------------------------
        # Radar in front
        # -------------------------
        radar_bp = blueprint_library.find("sensor.other.radar")
        radar_bp.set_attribute("horizontal_fov", "35")
        radar_bp.set_attribute("vertical_fov", "20")
        radar_bp.set_attribute("range", "100")
        radar_bp.set_attribute("sensor_tick", "0.05")

        front_radar_tf = carla.Transform(
            carla.Location(x=2.2, y=0.0, z=0.8),
            carla.Rotation(pitch=0.0, yaw=0.0, roll=0.0)
        )

        radar = attach_sensor("front_radar", radar_bp, front_radar_tf)

        # -------------------------
        # Optional: enable autopilot
        # -------------------------
        ego_vehicle.set_autopilot(True)

        # -------------------------
        # Output folders
        # -------------------------
        os.makedirs("out/front_rgb", exist_ok=True)
        os.makedirs("out/back_rgb", exist_ok=True)
        os.makedirs("out/left_rgb", exist_ok=True)
        os.makedirs("out/right_rgb", exist_ok=True)

        print("Running simulation... Press Ctrl+C to stop.")

        while True:
            frame = world.tick()

            data = {}

            for name, q in sensor_queues:
                sensor_data = q.get(timeout=2.0)
                data[name] = sensor_data

            # Save camera frames
            data["front_rgb"].save_to_disk(f"out/front_rgb/{frame:06d}.png")
            data["back_rgb"].save_to_disk(f"out/back_rgb/{frame:06d}.png")
            data["left_rgb"].save_to_disk(f"out/left_rgb/{frame:06d}.png")
            data["right_rgb"].save_to_disk(f"out/right_rgb/{frame:06d}.png")

            # LiDAR and radar are received here too.
            # You can parse them instead of saving PNGs.
            lidar_data = data["roof_lidar"]
            radar_data = data["front_radar"]

            print(
                f"Frame {frame} | "
                f"LiDAR points: {len(lidar_data)} | "
                f"Radar detections: {len(radar_data)}"
            )

    except KeyboardInterrupt:
        print("Stopping...")

    finally:
        world.apply_settings(original_settings)

        for actor in actors:
            if actor.is_alive:
                actor.destroy()

        print("Cleaned up actors and restored world settings.")


if __name__ == "__main__":
    main()

