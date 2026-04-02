#include <cstdint>
#include "rclcpp/rclcpp.hpp"
#include "arm_interface/msg/servo_command.hpp"


class ServoPublisherNode : public rclcpp::Node{
    public:
        ServoPublisherNode() : Node("servo_publisher"){
            this->declare_parameter("channel",1);
            this->declare_parameter("pulse", 1800);
           
            channel_  = this->get_parameter("channel").as_int();
            pulse_   = this->get_parameter("pulse").as_int();
            
            publisher_ = this->create_publisher<arm_interface::msg::ServoCommand>("servo_command", 10);
            timer_     = this->create_wall_timer(
                std::chrono::milliseconds(1000),
                [this]() {this->timer_callback();}
            );
        }

    private:
        void timer_callback(){
                auto msg = arm_interface::msg::ServoCommand();
                msg.channel = channel_;
                msg.pulse = pulse_;
                publisher_->publish(msg);
                RCLCPP_INFO(this->get_logger(), "Published : channel : %d <-> pulse : %d", msg.channel, msg.pulse);

        }

        rclcpp::Publisher<arm_interface::msg::ServoCommand>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
        uint8_t channel_ ; 
        uint16_t pulse_;   
    };

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ServoPublisherNode>());
    rclcpp::shutdown();
    return 0;
}