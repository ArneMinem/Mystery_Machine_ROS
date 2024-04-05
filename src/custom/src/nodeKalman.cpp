#include "custom/nodeKalman.h"

#include <chrono>
#include <functional>

#include <proj.h>

// Function to convert latitude and longitude to x and y coordinates using Lambert 93 projection
std::pair<double, double> transformLatLonToXY(double latitude, double longitude) {
    projPJ pj_latlon, pj_lambert;
    const char *proj4_latlon = "+proj=longlat +ellps=WGS84";
    const char *proj4_lambert = "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000 +ellps=GRS80";

    // Initialize the projection for latitude and longitude (WGS84)
    pj_latlon = pj_init_plus(proj4_latlon);
    if (pj_latlon == nullptr) {
        // Handle projection initialization error
    }

    // Initialize the Lambert 93 projection
    pj_lambert = pj_init_plus(proj4_lambert);
    if (pj_lambert == nullptr) {
        // Handle projection initialization error
    }

    // Define input point in latitude and longitude
    projXY latlon, lambert;

    latlon.u = DEG_TO_RAD * longitude;
    latlon.v = DEG_TO_RAD * latitude;

    // Transform latitude and longitude to Lambert 93 x and y coordinates
    lambert = pj_transform(pj_latlon, pj_lambert, 1, 1, &latlon);

    // Clean up
    pj_free(pj_latlon);
    pj_free(pj_lambert);

    // Return the transformed x and y coordinates
    return std::make_pair(lambert.u, lambert.v);
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

void nodeKalman::timer_callback()
{
    // Check if the GPS and RPY data have been received
    if (gps_data_received_ && rpy_data_received_) {
        // Create a new message of type "geometry_msgs::msg::PoseStamped"
        auto message = geometry_msgs::msg::PoseStamped();

        // Set the header of the message
        message.header.stamp = this->now();
        message.header.frame_id = "map";

        // Transform latitude and longitude to x and y coordinates (you may need to use a suitable projection method)
        std::pair<double, double> XY = transformLatLonToXY(latitude, longitude)

        // Set the position of the message using the transformed x and y coordinates
        message.pose.position.x = XY.first;
        message.pose.position.y = XY.second;
        message.pose.position.z = 0.0;

        // Set the orientation of the message using the heading from RPY
        message.pose.orientation = calculateOrientationFromRPY(roll_, pitch_, yaw_);

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
