#ifndef __nodeKalman_H__
#define __nodeKalman_H__

#include <cmath>
#include <chrono>
#include <iostream>

#include <eigen3/Eigen/Dense>

#include "rclcpp/rclcpp.hpp"

#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

// Définition des Quaternions
#include "tf2/LinearMath/Quaternion.h"
// Fonctions pour passer des Quaternions aux messages
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

// Pour les markers
#include "visualization_msgs/msg/marker.hpp"

// Pour le broadcast des transformations
#include "tf2_ros/transform_broadcaster.h"

#include "gpsd_client/msg/gps_fix.hpp"
#include "icm20948_driver/msg/rpy.hpp"

#include <proj.h>

// Attention à bien inclure chaque type de message !

using namespace std::chrono_literals;

using namespace std::placeholders;
using namespace Eigen;

// Création d'un node de classe Kalman
// Ce node subscribe à un topic de type msg::GPS et un autre de type msg::Rpy
// Il publie sur un topic de type msg::PoseStamped

class nodeKalman : public rclcpp::Node
{
public:
    nodeKalman();

    void gps_callback(const gpsd_client::msg::GpsFix::SharedPtr msg);
    void rpy_callback(const icm20948_driver::msg::RPY::SharedPtr msg);

private:
    void timer_callback();

    bool gps_data_received_ = false;
    bool rpy_data_received_ = false;

    double latitude_;
    double longitude_;

    double roll_;
    double pitch_;
    double yaw_;

    rclcpp::TimerBase::SharedPtr timer_;

    rclcpp::Subscription<gpsd_client::msg::GpsFix>::SharedPtr subscription_gps_;
    rclcpp::Subscription<icm20948_driver::msg::RPY>::SharedPtr subscription_rpy_;

    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr publisher_pose_;
};

#endif