#ifndef PROJET_RC_NODEMISSION_H
#define PROJET_RC_NODEMISSION_H
#include <chrono>
#include <functional>
#include <memory>
#include <string> 
#include <cmath> 
#include <eigen3/Eigen/Dense>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "visualization_msgs/msg/marker.hpp"
 



class nodeMission : public rclcpp::Node {
public:
    nodeMission();
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr publisher_pose;                   //publisher to the regulator
    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr subscription_pose_kalman;      //subscriber to the kalman filter
    rclcpp::Publisher<geometry_msgs::msg::bool>::SharedPtr publisher_etat;                          //publisher of the state to the regulator
    void timer_callback();          //Fonction de callback du timer
    
};


#endif
