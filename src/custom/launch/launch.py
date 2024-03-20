import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
import launch

# Fonction appelé par ros2 launch pour avoir la liste des nodes à lancer
def generate_launch_description():
    gpsd_share_dir = get_package_share_directory('gpsd_client')
    icm_share_dir = get_package_share_directory('icm20948_driver')
    motors_share_dir = get_package_share_directory('voiture2a_motors_driver')

    
    nodeGps = Node(package = 'gpsd_client',
                   namespace = '',
                   executable= 'GpsdNode',
                   name='gpsd_node')


    nodeIcm = Node(package = 'icm20948_driver',
                   namespace = '',
                   executable= 'ICM20948Node',
                   name='icm20948_node')


    nodeMotors = Node(package = 'voiture2a_motors_driver',
                   namespace = '',
                   executable= 'MotorsNode',
                   name='motors_node')


    return LaunchDescription([nodeGps, nodeIcm, nodeMotors])