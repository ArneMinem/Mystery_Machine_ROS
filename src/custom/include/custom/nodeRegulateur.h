//
// Created by matti on 20/03/2024.
//

#ifndef PROJET_RC_NODEREGULATEUR_H
#define PROJET_RC_NODEREGULATEUR_H


#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "std_srvs/srv/trigger.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/bool.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "visualization_msgs/msg/marker.hpp"
#include <cmath>

using namespace std::chrono_literals;
using namespace std::placeholders;


class nodeRegulateur : public rclcpp::Node {

    nodeRegulateur();

    /*
     * Subscriber data and function
     */
    // Bool
    bool lastStateBool = true;
    bool lastManualBool = true;
    // Real position
    float real_x = 0;
    float real_y = 0;
    float real_z = 0;
    float real_psi = 0;
    float real_theta = 0;
    float real_phi = 0;
    // Target position
    float target_x = 0;
    float target_y = 0;
    float target_z = 0;
    float target_psi = 0;
    float target_theta = 0;
    float target_phi = 0;
    // Fonction callback
    void callbackSubscriptionReceiveRealPosition(const geometry_msgs::msg::PoseStamped &realPosition_msg);
    void callbackSubscriptionReceiveTargetPosition(const geometry_msgs::msg::PoseStamped &targetPosition_msg);
    void callbackSubscriptionReceiveStateBool(const std_msgs::msg::Bool &stateBool_msg);
    void callbackSubscriptionReceiveManualBool(const std_msgs::msg::Bool &manualBool_msg);

    /*
     * Publisher data and function
     */
    // Twist commande
    geometry_msgs::msg::Twist cmdTwist_msg;
    // Fonction callback
    void timerSendCmd(void);

    /*
     * Fonction intermédiaire
     */
    void createCommandTwist(void);

};


#endif //PROJET_RC_NODEREGULATEUR_H
