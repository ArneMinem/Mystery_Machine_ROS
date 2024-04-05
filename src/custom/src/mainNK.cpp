#include "custom/nodeKalman.h"

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

