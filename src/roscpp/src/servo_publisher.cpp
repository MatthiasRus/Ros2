#include "rclcpp/rclcpp.hpp"
#include "arm_interface/msg/servo_command.hpp"


class ServoPublisherNode : public rclcpp::Node{
    public:
        ServoPublisherNode() : Node("ServoPublisherNode"){
            publisher_ = this->create_publisher<arm_interface::msg::ServoCommand>("ServoCommand", 10);
            timer_     = this->create_wall_timer(
                std::chrono::milliseconds(1000),
                [this]() {this->timer_callback();}
            );
        }

    private:
        void timer_callback(){
                auto msg = arm_interface::msg::ServoCommand();
                msg.channel = 1;
                msg.pulse = 1800;
                publisher_->publish(msg);
                RCLCPP_INFO(this->get_logger(), "Published : channel : %d <-> pulse : %d", msg.channel, msg.pulse);

        }

        rclcpp::Publisher<arm_interface::msg::ServoCommand>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;

};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ServoPublisherNode>());
    rclcpp::shutdown();
    return 0;
}