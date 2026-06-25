#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "config_loader.hpp"

void writeSensor(std::ostream & output, const carla_bridge::SensorConfig & sensor)
{
  output << "- name: " << sensor.name << '\n';
  output << "  type: " << sensor.type << '\n';
  output << "  topic: " << sensor.topic << '\n';
  output << "  frame_id: " << sensor.frame_id << '\n';
  output << "  transform:\n";
  output << "    x: " << sensor.transform.x << '\n';
  output << "    y: " << sensor.transform.y << '\n';
  output << "    z: " << sensor.transform.z << '\n';
  output << "    roll: " << sensor.transform.roll << '\n';
  output << "    pitch: " << sensor.transform.pitch << '\n';
  output << "    yaw: " << sensor.transform.yaw << '\n';

  output << "  attributes:\n";
  const std::map<std::string, std::string> sorted_attributes(
    sensor.attributes.begin(), sensor.attributes.end());
  for (const auto & attribute : sorted_attributes) {
    output << "    " << attribute.first << ": " << attribute.second << '\n';
  }
}

int main(int argc, char ** argv)
{
  const std::string input_path = argc > 1 ? argv[1] : TEST_SENSOR_CONFIG_PATH;
  const std::string output_path = argc > 2 ? argv[2] : "parsed_sensor_data.txt";

  const carla_bridge::SensorSuiteConfig config =
    carla_bridge::loadSensorConfig(input_path);

  std::ofstream output(output_path);
  if (!output.is_open()) {
    std::cerr << "Failed to open output file: " << output_path << '\n';
    return 1;
  }

  output << "sensor_count: " << config.sensors.size() << '\n';
  output << "sensors:\n";
  for (const auto & sensor : config.sensors) {
    writeSensor(output, sensor);
  }

  std::cout << "Wrote parsed sensor data to " << output_path << '\n';
  return 0;
}
