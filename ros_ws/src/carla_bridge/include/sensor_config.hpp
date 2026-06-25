#pragma once // define once

#include <string>
#include <vector>
#include <unordered_map>

namespace carla_bridge
{
struct TransformConfig
{
  double x{0.0};
  double y{0.0};
  double z{0.0};

  double roll{0.0};
  double pitch{0.0};
  double yaw{0.0};
};

struct SensorConfig
{
  std::string name;
  std::string type;
  std::string topic;
  std::string frame_id;

  TransformConfig transform;
  std::unordered_map<std::string, std::string> attributes;
};

struct SensorSuiteConfig
{
  std::vector<SensorConfig> sensors;
};
}
