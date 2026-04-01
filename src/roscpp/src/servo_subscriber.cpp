#include "rclcpp/rclcpp.hpp"
#include "arm_interface/msg/servo_command.hpp"


class ServoSubscriberNode : public rclcpp::Node {
public:
    ServoSubscriberNode() : Node("ServoSubscriberNode"){
        subscriber_ = this->create_subscription<arm_interface::msg::ServoCommand>(
            "ServoCommand",
            10,
             [this](const arm_interface::msg::ServoCommand::SharedPtr msg){
                this->callback_message(msg);
        });
    }

private:

    void callback_message(const arm_interface::msg::ServoCommand::SharedPtr msg){
        RCLCPP_INFO(this->get_logger(), "Channel : %d <=> pulse : %d", msg->channel, msg->pulse);
    }
    rclcpp::Subscription<arm_interface::msg::ServoCommand>::SharedPtr subscriber_;
};


int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ServoSubscriberNode>());
    rclcpp::shutdown();
    return 0;
}