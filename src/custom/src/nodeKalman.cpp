#include "custom/nodeKalman.h"

#include <chrono>
#include <functional>

struct Lambert93Result {
    double x;
    double y;
};

// Function to convert latitude and longitude to x and y coordinates using Lambert 93 projection
Lambert93Result convertToLambert93(double longitude, double latitude) {
    // Initialize the projection transformation
    PJ* P = proj_create_crs_to_crs(
        nullptr,
        "+proj=longlat +datum=WGS84",  // Source CRS definition (e.g., WGS84 geographic coordinates)
        "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",  // Target CRS definition (e.g., Lambert Conformal Conic projection)
        nullptr);

    PJ_COORD geo_coord, projected_coord;
    geo_coord.lpzt.lam = longitude;  // Longitude
    geo_coord.lpzt.phi = latitude;   // Latitude
    geo_coord.lpzt.z = 0;            // Altitude
    projected_coord = proj_trans(P, PJ_FWD, geo_coord);  // Forward projection from geographic to projected coordinates

    // Free the projection object after use
    proj_destroy(P);

    return {projected_coord.xyz.x, projected_coord.xyz.y};
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
    publisher_pose_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("realPosition", 10);
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
        Lambert93Result lambert93 = convertToLambert93(longitude_, latitude_);

        // Set the position of the message
        message.pose.position.x = lambert93.x;
        message.pose.position.y = lambert93.y;

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


