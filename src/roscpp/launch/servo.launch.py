from launch import LaunchDescription
from launch_ros.actions import Node
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    path = os.path.join(get_package_share_directory("roscpp"), "config", "servo_param.yaml")
    return LaunchDescription([
        Node(
            package="roscpp",
            executable="ServoPublisherNode",
            namespace="arm",
            name="servo_publisher",
            parameters=[path]
        ),
        Node(
            package="roscpp",
            executable="ServoSubscriberNode",
            namespace="arm",
            name="servo_subscriber"
        )
    ])