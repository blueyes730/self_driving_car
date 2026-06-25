#include "config_loader.hpp"

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <vector>
#include <unordered_map>

namespace carla_bridge
{
SensorSuiteConfig loadSensorConfig(
  const std::string & path)
{
  SensorSuiteConfig parsed_config;

  YAML::Node config = YAML::LoadFile(path);
  std::vector<SensorConfig> sensors;

  if (config["sensors"]) {
    for (const auto & sensor : config["sensors"]) {
      SensorConfig sensor_config;
      sensor_config.name = sensor["name"].as<std::string>();
      sensor_config.type = sensor["type"].as<std::string>();
      sensor_config.topic = sensor["topic"].as<std::string>();
      sensor_config.frame_id = sensor["frame_id"].as<std::string>();

      TransformConfig transform_config;
      transform_config.x = sensor["transform"]["x"].as<double>();
      transform_config.y = sensor["transform"]["y"].as<double>();
      transform_config.z = sensor["transform"]["z"].as<double>();
      transform_config.roll = sensor["transform"]["roll"].as<double>();
      transform_config.pitch = sensor["transform"]["pitch"].as<double>();
      transform_config.yaw = sensor["transform"]["yaw"].as<double>();

      sensor_config.transform = transform_config;
      std::unordered_map<std::string, std::string> attributes;
      if (sensor["attributes"]) {
        for (const auto & attribute : sensor["attributes"]) {
          attributes.emplace(attribute.first.as<std::string>(), attribute.second.as<std::string>());
        }
      }

      sensor_config.attributes = attributes;

      sensors.push_back(sensor_config);
    }
  }
  parsed_config.sensors = sensors;
  return parsed_config;
}
}
