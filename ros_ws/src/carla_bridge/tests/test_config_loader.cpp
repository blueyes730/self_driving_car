#include <gtest/gtest.h>

#include <string>

#include "config_loader.hpp"

namespace carla_bridge
{
namespace
{

TEST(ConfigLoaderTest, LoadsSensorConfiguration)
{
    const SensorSuiteConfig config = loadSensorConfig(TEST_SENSOR_CONFIG_PATH);

    ASSERT_EQ(config.sensors.size(), 8U);

    const SensorConfig & front_camera = config.sensors.at(0);
    EXPECT_EQ(front_camera.name, "front_rgb");
    EXPECT_EQ(front_camera.type, "sensor.camera.rgb");
    EXPECT_EQ(front_camera.topic, "/ego/front_camera/image_raw");
    EXPECT_EQ(front_camera.frame_id, "ego/front_camera");
    EXPECT_DOUBLE_EQ(front_camera.transform.x, 1.8);
    EXPECT_DOUBLE_EQ(front_camera.transform.z, 1.6);
    EXPECT_EQ(front_camera.attributes.at("image_size_x"), "1280");
    EXPECT_EQ(front_camera.attributes.at("sensor_tick"), "0.05");

    const SensorConfig & lidar = config.sensors.at(6);
    EXPECT_EQ(lidar.name, "roof_lidar");
    EXPECT_EQ(lidar.type, "sensor.lidar.ray_cast");
    EXPECT_DOUBLE_EQ(lidar.transform.z, 2.4);
    EXPECT_EQ(lidar.attributes.at("channels"), "64");
    EXPECT_EQ(lidar.attributes.at("points_per_second"), "1300000");

    const SensorConfig & radar = config.sensors.at(7);
    EXPECT_EQ(radar.name, "front_radar");
    EXPECT_EQ(radar.type, "sensor.other.radar");
    EXPECT_EQ(radar.attributes.at("horizontal_fov"), "35");
    EXPECT_EQ(radar.attributes.at("range"), "100");
}

}  // namespace
}  // namespace carla_bridge
