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
#include "std_msgs/msg/bool.hpp"
#include "tf2/utils.h"
#include <cmath>
#include <vector>
using namespace std::chrono_literals;
using namespace std::placeholders;


class nodeMission : public rclcpp::Node {
public:
    nodeMission();
    /*
     * Subscriber data and function
     */

    // Target position
    float target_x = 0;
    float target_y = 0;

    // target_list est organisé en paires [x0, y0, x1, y1, ...]
    std::vector<float> target_list = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4};
    long unsigned int k = 0;

    // Fonction callback
    void callbackSubscriptionReceivePosition(const geometry_msgs::msg::PoseStamped &Position_msg);
    /*
     * Publisher data and function
     */
    // commande
    geometry_msgs::msg::PoseStamped pose_voiture_msg;
    std_msgs::msg::Bool stateBool_msg;
    // Fonction callback
    void timerSendCmdcallback();
private:
    const float epsilon = 13;
    rclcpp::TimerBase::SharedPtr timerSendCmd;
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr publisherSendPositionRegulator;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr publisherSendStateBoolRegulator;
    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr subscriberReceivePosition;
};


#endif
