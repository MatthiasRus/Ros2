#include "rclcpp/rclcpp.hpp"
#include "arm_interface/srv/servo_gripper.hpp"
#include <chrono>

int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    if (argc < 2){
        std::cout<<"We need a Position Value lad"<<std::endl;
        return 0;
    }

    auto node = rclcpp::Node::make_shared("gripper_client");
    auto client = node->create_client<arm_interface::srv::ServoGripper>("set_gripper");

    while(!client->wait_for_service(std::chrono::seconds(1))){
            if (!rclcpp::ok()) {
                RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
                return 0;
            }
            RCLCPP_INFO(node->get_logger(), "waiting for Service ... \n");
 
    }

    auto request = std::make_shared<arm_interface::srv::ServoGripper::Request>();
    request->position = atoll(argv[1]);

    client->async_send_request(request,
            [node](rclcpp::Client<arm_interface::srv::ServoGripper>::SharedFuture future){
                auto response = future.get();
                RCLCPP_INFO(node->get_logger(), "Position: %d, Message: %s",
            response->current_position,
            response->message.c_str());
            rclcpp::shutdown();
            }

    );

    rclcpp::spin(node);
    return 0;

}