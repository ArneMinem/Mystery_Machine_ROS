//
// Created by matti on 20/03/2024.
//

#include "../include/custom/nodeRegulateur.h"

nodeRegulateur::nodeRegulateur() : Node("nodeRegulateur") {


    timer_ = this->create_wall_timer(100ms, std::bind(&TurretSimulated::timer_callbackPublisher, this));
    publisher_ = this->create_publisher<visualization_msgs::msg::Marker>("turret3D", 10);

    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ = std::make_unique<tf2_ros::TransformListener>(*tf_buffer_);
}
