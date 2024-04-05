cmake_minimum_required(VERSION 3.8)
project(custom)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(std_srvs REQUIRED)


# BoatSimulated
add_executable(nodeRegulateur src/nodeKalman.cpp
        include/custom/nodeKalman.h
)

add_executable(nodeRegulateur src/nodeRegulateur.cpp
        include/custom/nodeRegulateur.h
)

add_executable(nodeMission src/nodeMission.cpp
        include/custom/nodeMission.h
)


ament_target_dependencies(nodeMission
        std_srvs
        rclcpp
        std_msgs
        geometry_msgs
        tf2
        tf2_geometry_msgs
        visualization_msgs
        tf2_ros
)

ament_target_dependencies(nodeRegulateur
        std_srvs
        rclcpp
        std_msgs
        geometry_msgs
        tf2
        tf2_geometry_msgs
        visualization_msgs
        tf2_ros
)

ament_target_dependencies(nodeKalman
        std_srvs
        rclcpp
        std_msgs
        geometry_msgs
        tf2
        tf2_geometry_msgs
        visualization_msgs
        tf2_ros
)

target_link_libraries(nodeRegulateur "${cpp_typesupport_target}")
target_include_directories(nodeRegulateur PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(nodeKalman "${cpp_typesupport_target}")
target_include_directories(nodeKalman PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(nodeMission "${cpp_typesupport_target}")
target_include_directories(nodeMission PUBLIC
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




install(TARGETS nodeMission
        DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch
        DESTINATION share/${PROJECT_NAME}/
)

install(TARGETS nodeKalman
        DESTINATION lib/${PROJECT_NAME})

install(TARGETS nodeRegulateur
        DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch
        DESTINATION share/${PROJECT_NAME}/
)

ament_package()