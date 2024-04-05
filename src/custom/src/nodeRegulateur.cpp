//
// Created by matti on 20/03/2024.
//

#include "custom/nodeRegulateur.h"
#include "tf2/utils.h"

nodeRegulateur::nodeRegulateur() : Node("nodeRegulateur") {
    // Timer
    this->timerSendCmd_ = this->create_wall_timer(100ms, std::bind(&nodeRegulateur::timerSendCmd, this)); // Timer d'envoie de la commande aux actionneurs

    // Publisher
    this->publisherSendCmd_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd", 10);

    // Subscriber
    this->subscriberReceiveRealPosition = this->create_subscription<geometry_msgs::msg::PoseStamped>("realPosition", 10, std::bind(&nodeRegulateur::callbackSubscriptionReceiveRealPosition, this, _1)); // Subscriber reçoit la position du robot
    this->subscriberReceiveTargetPosition = this->create_subscription<geometry_msgs::msg::PoseStamped>("targetPosition", 10, std::bind(&nodeRegulateur::callbackSubscriptionReceiveTargetPosition, this, _1));// Subscriber reçoit la position cible
    this->subscriberReceiveStateBool = this->create_subscription<std_msgs::msg::Bool>("stateBool", 10, std::bind(&nodeRegulateur::callbackSubscriptionReceiveStateBool, this, _1)); // Subscriber reçoit de la part du node Mission si il doit s'activer
    this->subscriberReceiveManualBool = this->create_subscription<std_msgs::msg::Bool>("manualBool", 10, std::bind(&nodeRegulateur::callbackSubscriptionReceiveManualBool, this, _1)); // Subscriber reçoit de la part du node Client si il doit s'activer
}

void nodeRegulateur::callbackSubscriptionReceiveRealPosition(const geometry_msgs::msg::PoseStamped &realPosition_msg){

    this->real_x = realPosition_msg.pose.position.x;
    this->real_y = realPosition_msg.pose.position.y;
    this->real_z = realPosition_msg.pose.position.z;
    this->real_psi = tf2::getYaw(realPosition_msg.pose.orientation); // Pitch
    this->real_phi = tf2::getYaw(realPosition_msg.pose.orientation); // Yaw
    this->real_theta = tf2::getYaw(realPosition_msg.pose.orientation); // Theta
}

void nodeRegulateur::callbackSubscriptionReceiveTargetPosition(const geometry_msgs::msg::PoseStamped &targetPosition_msg){

    this->target_x = targetPosition_msg.pose.position.x;
    this->target_y = targetPosition_msg.pose.position.y;
    this->target_z = targetPosition_msg.pose.position.z;
    this->target_psi = tf2::getYaw(targetPosition_msg.pose.orientation); // Pitch
    this->target_phi = tf2::getYaw(targetPosition_msg.pose.orientation); // Yaw
    this->target_theta = tf2::getYaw(targetPosition_msg.pose.orientation); // Theta
}

void nodeRegulateur::callbackSubscriptionReceiveStateBool(const std_msgs::msg::Bool &stateBool_msg){
    this->lastStateBool = stateBool_msg.data;
}

void nodeRegulateur::callbackSubscriptionReceiveManualBool(const std_msgs::msg::Bool &manualBool_msg){
    this->lastManualBool = manualBool_msg.data;
}

void nodeRegulateur::timerSendCmd(){

    if (this->lastManualBool){
        // Si le PC nous dit le PC d'être en manual (manual = True), on envoie rien
    }

    if (not (this->lastManualBool)){

        this->cmdTwist_msg.linear.x = 0; // Vitesse d'avance
        this->cmdTwist_msg.angular.z = 0; // Vitesse de rotation

        if (this->lastStateBool){
            this->createCommandTwist();
        }
        publisherSendCmd_->publish(this->cmdTwist_msg);
    }
}

void nodeRegulateur::createCommandTwist(void){
    double dx = this->target_x - this->real_x;
    double dy = this->target_y - this->real_y;
    double distance = sqrt(pow(dx,2) + pow(dy, 2));
    dx = (this->target_x  -  this->real_x)/(distance+0.01);
    dy = (this->target_y - this->real_y)/(distance+0.01);

    double ecartCap = sin(this->real_psi - this->target_psi);

    this->cmdTwist_msg.angular.z = 200/M_PI * ecartCap ; // Vitesse de rotation
    this->cmdTwist_msg.linear.x = 400 * 2/M_PI * atan(0.0726*distance);  // Vitesse d'avance'
}

int main(int argc, char * argv[]) {
    // Initialise ROS 2 pour l'executable
    rclcpp::init(argc, argv);

    auto node = std::make_shared<nodeRegulateur>();

    // Créer le node et se met en attente de messages ou d'évènements du timer
    // Attention, cette fonction est bloquante !
    rclcpp::spin(node);

    // Coupe ROS 2 pour l'executable
    rclcpp::shutdown();

    return 0;
};