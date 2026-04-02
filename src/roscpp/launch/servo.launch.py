from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package="roscpp",
            executable="ServoPublisherNode",
            namespace="/ServoCommand",
            name="servoPub"
        ),

        Node(
            package="roscpp",
            executable="ServoSubscriberNode",
            namespace="/ServoCommand",
            name="servoSub"
        )

    ])
