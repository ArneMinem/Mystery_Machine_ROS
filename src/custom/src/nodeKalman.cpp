#include "custom/nodeKalman.h"

#include <chrono>
#include <functional>

typedef struct {
    double x;
    double y;
} Lambert93Result;

// Function to convert latitude and longitude to x and y coordinates using Lambert 93 projection
Lambert93Result convertToLambert93(double latitude, double longitude) {
    double n = 0.7256077650532670;
    double C = 11754255.4260960;
    double Xs = 700000.0;
    double Ys = 12655612.0498760;
    double lambda0 = 3.0 * (M_PI / 180.0);
    double phi0 = 46.5 * (M_PI / 180.0);

    double l = 0.5 * log((1 + sin(latitude)) / (1 - sin(latitude)));
    double L = n * (longitude - lambda0);

    double R = C * exp(-n * l);
    double gamma = n * (longitude - lambda0);

    double X = Xs + R * sin(gamma);
    double Y = Ys - R * cos(gamma);

    Lambert93Result result = {X, Y};
    return result;
}

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

void nodeKalman::timer_callback() {
    // Check if the GPS and RPY data have been received
    if (gps_data_received_ && rpy_data_received_) {
        // Create a new message of type "geometry_msgs::msg::PoseStamped"
        auto message = geometry_msgs::msg::PoseStamped();

        // Set the header of the message
        message.header.stamp = this->now();
        message.header.frame_id = "map";

        // Transform latitude and longitude to x and y coordinates using Lambert 93 projection
        Lambert93Result result = convertToLambert93(longitude_, latitude_);

        // Set the position of the message
        message.pose.position.x = result.x;
        message.pose.position.y = result.y;

        // Create a Quaternion from the roll, pitch, and yaw angles
        tf2::Quaternion q;
        q.setRPY(roll_, pitch_, yaw_);

        // Set the orientation of the message
        message.pose.orientation = tf2::toMsg(q);
        
        // Publish the message
        publisher_pose_->publish(message);

    }
}

void nodeKalman::gps_callback(const gpsd_client::msg::GpsFix::SharedPtr msg)
{
    // Store the received GPS data
    latitude_ = msg->latitude;
    longitude_ = msg->longitude;
    gps_data_received_ = true;
}

void nodeKalman::rpy_callback(const icm20948_driver::msg::RPY::SharedPtr msg)
{
    // Store the received RPY data
    roll_ = msg->roll;
    pitch_ = msg->pitch;
    yaw_ = msg->yaw;
    rpy_data_received_ = true;
}


int main(int argc, char * argv[]) {
    // Initialise ROS 2 pour l'executable
    rclcpp::init(argc, argv);
    
    auto node = std::make_shared<nodeKalman>();
    
    // Créer le node et se met en attente de messages ou d'évènements du timer
    // Attention, cette fonction est bloquante !
    rclcpp::spin(node);
    
    // Coupe ROS 2 pour l'executable
    rclcpp::shutdown(); 
    
    return 0;
};


