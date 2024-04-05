#include "custom/nodeMission.h"
using namespace Eigen;
using namespace std::chrono_literals;

nodeMission::nodeMission() : rclcpp::Node("nodeMission") {
    // Timer
    this->timerSendCmd = this->create_wall_timer(100ms, std::bind(&nodeMission::timerSendCmdcallback, this)); // Timer d'envoie de la commande au regulateur

    // Publisher
    this->publisherSendPositionRegulator = this->create_publisher<geometry_msgs::msg::PoseStamped>("pose_voiture_msg", 10);
    this->publisherSendStateBoolRegulator = this->create_publisher<std_msgs::msg::Bool>("stateBool", 10);
    // Subscriber
    this->subscriberReceiveTargetPosition = this->create_subscription<geometry_msgs::msg::PoseStamped>("realPosition", 10, std::bind(&nodeMission::callbackSubscriptionReceiveRealPosition, this, _1)); // Subscriber reçoit la position du robot
};

void nodeMission::callbackSubscriptionReceiveTargetPosition(const geometry_msgs::msg::PoseStamped &targetPosition_msg){
    this->target_x = targetPosition_msg.pose.position.x;
    this->target_y = targetPosition_msg.pose.position.y;

}

void nodeMission::timerSendCmdcallback(){
    this->pose_voiture_msg.pose.position.x = this->target_x;
    this->pose_voiture_msg.pose.position.y = this->target_y;


    publisherSendPositionRegulator->publish(this->pose_voiture_msg);
    publisherSendStateBoolRegulator->publish(this->stateBool_msg);

    if (pose_voiture_msg.pose.position.x == 10){
        stateBool_msg.data = false;
    }

}

int main(int argc, char * argv[]) {
    // Initialise ROS 2 pour l'executable
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<nodeMission>());
    rclcpp::shutdown();
    return 0;
}