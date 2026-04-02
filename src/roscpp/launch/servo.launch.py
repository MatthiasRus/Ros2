from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package="roscpp",
            executable="ServoPublisherNode",
            namespace="arm",
            name="servo_publisher",
            parameters=[{"channel": 1, "pulse": 1775}]
        ),
        Node(
            package="roscpp",
            executable="ServoSubscriberNode",
            namespace="arm",
            name="servo_subscriber"
        )
    ])