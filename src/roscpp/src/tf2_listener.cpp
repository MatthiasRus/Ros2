#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "tf2/exceptions.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include <chrono>


class TF2ListenerNode : public rclcpp::Node{
public:
    TF2ListenerNode() : Node("tf2_listener"){
        buffer_ =std::make_shared<tf2_ros::Buffer>(this->get_clock());
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_, this);
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&TF2ListenerNode::timer_callback, this)
        );
    }


private:
    void timer_callback(){
        try{

            auto transform = buffer_->lookupTransform(
                "base_link",
                "end_effector",
                tf2::TimePointZero
            );

            auto &t = transform.transform.translation;

            RCLCPP_INFO(this->get_logger(),
                "end_effector in base_link → x: %.3f  y: %.3f  z: %.3f",
                t.x, t.y, t.z);
        }
        catch (tf2::TransformException &ex){
                RCLCPP_ERROR(this->get_logger(), "Exception Occured ... %s", ex.what());
            }
    }

    std::shared_ptr<tf2_ros::Buffer> buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    rclcpp::TimerBase::SharedPtr timer_;
};


int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TF2ListenerNode>());
    rclcpp::shutdown();

    return 0;
}
