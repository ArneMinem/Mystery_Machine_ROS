cmake_minimum_required(VERSION 3.8)
project(custom)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)

find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(std_srvs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(icm20948_driver REQUIRED)
find_package(gpsd_client REQUIRED)

add_executable(node_kalman
  src/nodeKalman.cpp
  src/mainNK.cpp
)

ament_target_dependencies(node_kalman
  rclcpp
  tf2
  tf2_geometry_msgs
  geometry_msgs
  visualization_msgs
  tf2_ros
  gpsd_client
  std_srvs
  std_msgs icm20948_driver
)

target_link_libraries(node_kalman "${cpp_typesupport_target}")
target_include_directories(node_kalman PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:include>)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(TARGETS
  node_kalman
  DESTINATION lib/${PROJECT_NAME}
)


ament_package()
