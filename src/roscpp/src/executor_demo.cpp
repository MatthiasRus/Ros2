#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <chrono>
#include <thread>


class   MTExecutor : public rclcpp::Node{
public:
    MTExecutor() : Node("multi_thread_node"){
        exclusive_group = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
        reentrant_group = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
       
        rclcpp::SubscriptionOptions options;
        options.callback_group = reentrant_group;

        sub_ = this->create_subscription<std_msgs::msg::String>(
            "chatter",
            10,
            [this](const std_msgs::msg::String::SharedPtr msg){this->callback_timer(msg);},
            options
        );
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(500),
            [this](){this->sleep_callback();},
            exclusive_group
        );
    }

private:
    void callback_timer(const std_msgs::msg::String::SharedPtr msg){
         RCLCPP_INFO(this->get_logger(), "The message %s ", msg->data.c_str());
    }

    void sleep_callback(){
         
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        RCLCPP_INFO(this->get_logger(), "\nThis the Lagging ONE\n");
    }

rclcpp::Subscription<std_msgs::msg::String>::SharedPtr  sub_;
rclcpp::TimerBase::SharedPtr                            timer_;
rclcpp::CallbackGroup::SharedPtr                        exclusive_group;
rclcpp::CallbackGroup::SharedPtr                        reentrant_group;
};


int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    rclcpp::executors::MultiThreadedExecutor executor;
    auto node = std::make_shared<MTExecutor>();
    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}

