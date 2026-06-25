cmake_minimum_required(VERSION 3.8)
project(carla_bridge)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(yaml-cpp REQUIRED)

add_library(config_loader src/config_loader.cpp)
target_include_directories(config_loader PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(config_loader PUBLIC yaml-cpp)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()

  ament_add_gtest(test_config_loader tests/test_config_loader.cpp)
  target_link_libraries(test_config_loader config_loader)
  target_compile_definitions(test_config_loader PRIVATE
    TEST_SENSOR_CONFIG_PATH="${CMAKE_CURRENT_SOURCE_DIR}/tes  ts/data/test_sensor_data.yaml"
  )

  add_executable(dump_config_loader tests/dump_config_loader.cpp)
  target_link_libraries(dump_config_loader config_loader)
  target_compile_definitions(dump_config_loader PRIVATE
    TEST_SENSOR_CONFIG_PATH="${CMAKE_CURRENT_SOURCE_DIR}/tests/data/test_sensor_data.yaml"
  )
endif()

install(TARGETS
  dump_config_loader
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
