#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"


class PublisherNode : public rclcpp::Node {

    public:
        PublisherNode() : Node("PublisherNode"){
            publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);
            timer_     = this->create_wall_timer(
                std::chrono::milliseconds(500),
                [this]() {this->callback_timer();}
            );

        } 

    private:
        void callback_timer(void){
            auto msg = std_msgs::msg::String();
            msg.data = "Hello this is rclcpp manigga";
            publisher_->publish(msg);
            RCLCPP_INFO(this->get_logger(), "Published %s", msg.data.c_str());
        };

        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr  publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
};


int main(int argc, char *argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PublisherNode>());
    rclcpp::shutdown();
    return 0;
}