#include "custom/nodeMission.h"
using namespace Eigen;
using namespace std::chrono_literals;

nodeMission::nodeMission() : rclcpp::Node("nodeMission") {
    // Timer
    auto this->timerSendCmd_ = this->create_wall_timer(100ms, std::bind(&nodeMission::timerSendCmd, this)); // Timer d'envoie de la commande au regulateur

    // Publisher
    auto this->publisherSendPositionRegulator = this->create_publisher<geometry_msgs::msg::PoseStamped>("pose_voiture", 10);
    auto this->publisherSendStateBoolRegulator = this->create_publisher<std_msgs::msg::Bool>("stateBool", 10);
    // Subscriber
    auto this->subscriberReceiveRealPosition = this->create_subscription<geometry_msgs::msg::PoseStamped>("realPosition", 10, std::bind(&nodeMission::callbackSubscriptionReceiveRealPosition, this, _1)); // Subscriber reçoit la position du robot
};

void nodeMission::callbackSubscriptionReceiveRealPosition(const geometry_msgs::msg::PoseStamped &realPosition_msg){
    this->pose_voiture = targetPosition_msg.data;
}
// void nodeMission::publisherSendStateBoolRegulator(const std_msgs::msg::Bool &stateBool_msg){
//     this->stateBool = stateBool_msg.data;
// }
// void nodeMission::publisherSendPositionRegulator(const geometry_msgs::msg::PoseStamped &pose_voiture_msg){
//     this->pose_voiture = pose_voiture_msg.data;
// }
void nodeMission::timerSendCmd(){
    
    publisherSendPositionRegulator->publish(this->pose_voiture);
    publisherSendStateBoolRegulator->publish(this->stateBool);
}

int main(int argc, char * argv[]) {
    // Initialise ROS 2 pour l'executable
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<nodeMission>());
    rclcpp::shutdown();
    return 0;
}