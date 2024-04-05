#include "custom/nodeKalman.h"

#include <chrono>
#include <functional>


nodeKalman::nodeKalman() : Node("nodeKalman")
{
    // Create a timer that calls the callback function once every 500ms
    timer_ = this->create_wall_timer(500ms, std::bind(&nodeKalman::timer_callback, this));

    // Create a subscription to the topic "gps" with the callback function "gps_callback"
    subscription_gps_ = this->create_subscription<gpsd_client::msg::GpsFix>("gps", 10, std::bind(&nodeKalman::gps_callback, this, _1));

    // Create a subscription to the topic "rpy" with the callback function "rpy_callback"
    subscription_rpy_ = this->create_subscription<icm20948_driver::msg::RPY>("rpy", 10, std::bind(&nodeKalman::rpy_callback, this, _1));

    // Create a publisher to the topic "pose" with a queue size of 10
    publisher_pose_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("pose", 10);
}

void nodeKalman::timer_callback()
{
    // Create a new message of type "geometry_msgs::msg::PoseStamped"
    auto message = geometry_msgs::msg::PoseStamped();

    // Set the header of the message
    message.header.stamp = this->now();
    message.header.frame_id = "map";

    // Set the position of the message
    message.pose.position.x = 0.0;
    message.pose.position.y = 0.0;
    message.pose.position.z = 0.0;

    // Set the orientation of the message
    message.pose.orientation.x = 0.0;
    message.pose.orientation.y = 0.0;
    message.pose.orientation.z = 0.0;
    message.pose.orientation.w = 1.0;

    // Publish the message
    publisher_pose_->publish(message);
}