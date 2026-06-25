#pragma once

#include <string>

#include "sensor_config.hpp"

namespace carla_bridge
{
SensorSuiteConfig loadSensorConfig(
  const std::string & path);
}
