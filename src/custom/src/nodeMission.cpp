#include "custom/nodeMission.h"
using namespace Eigen;
using namespace std::chrono_literals;

nodeMission::nodeMission() : rclcpp::Node("nodeMission") {
    timer_ = this->create_wall_timer(loop_dt_, std::bind(&nodeMission::timer_callback, this));
    publisher_pose = this->create_publisher<geometry_msgs::msg::PoseStamped>("pose_voiture", 10);
    subscription_pose_kalman = this->create_subscription<geometry_msgs::msg::PoseStamped>("pose_kalman", 10, std::bind(&nodeMission::callback, this, std::placeholders::_1));
    publisher_etat = this->create_publisher<geometry_msgs::msg::Bool>("etat_misssion", 10);
    
};


nodeMission::timer_callback() {
    //TODO
    //publish the pose of the car
    //publish the state of the mission
    //publish the command to the regulator
    //publish the command to the manual
};

int main(int argc, char * argv[]) {
    // Initialise ROS 2 pour l'executable
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<nodeMission>());
    rclcpp::shutdown();
    return 0;
}