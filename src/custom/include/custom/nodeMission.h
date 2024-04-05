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
    /*
     * Subscriber data and function
     */
    // Bool
    bool stateBool_msg = true;
    // Target position
    float target_x = 0;
    float target_y = 0;
    float target_z = 0;
    float target_psi = 0;
    float target_theta = 0;
    float target_phi = 0;
    // Fonction callback
    void callbackSubscriptionReceiveRealPosition(const geometry_msgs::msg::PoseStamped &realPosition_msg);
    /*
     * Publisher data and function
     */
    // commande
    geometry_msgs::msg::PoseStamped pose_voiture_msg;
    std_msgs::msg::Bool stateBool_msg;
    // Fonction callback
    void timerSendCmd(void);
};


#endif
