#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"


class SubscriberNode : public rclcpp::Node{
    public:    
    SubscriberNode() : Node("SubscriberNode"){
        subscriber_  = this->create_subscription<std_msgs::msg::String>(
                        "chatter",
                         10,
                        [this](const std_msgs::msg::String::SharedPtr msg) {this->callback_message(msg);}
                    );
    }

    private:
    void callback_message(const std_msgs::msg::String::SharedPtr msg){
       RCLCPP_INFO(this->get_logger(), "Subscribed to : %s" , msg->data.c_str()); 
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr  subscriber_;


};


int main(int argc , char *argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SubscriberNode>());
    rclcpp::shutdown();
    return 0;
}