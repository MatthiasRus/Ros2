#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.h"
#include <chrono>


class TFBroadcaster : public rclcpp::Node{
public:
    TFBroadcaster() : Node("tf2_broadcaster"){
            tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
            timer_ = this->create_wall_timer(
            std::chrono::milliseconds(1000),
            std::bind(&TFBroadcaster::timer_callback, this)
        );

    }

private:
    void timer_callback(){
    
        geometry_msgs::msg::TransformStamped msg;
        msg.header.stamp   = this->get_clock()->now();
        msg.header.frame_id = "base_link";
        msg.child_frame_id =  "target_obj";

        msg.transform.translation.x = 3.2;
        msg.transform.translation.y = 2.54;
        msg.transform.translation.z = 2.32;

        msg.transform.rotation.x = 0;
        msg.transform.rotation.y = 0;
        msg.transform.rotation.z = 0;
        msg.transform.rotation.w = 1;


     
        RCLCPP_INFO(this->get_logger(), "We have started broadcasting\n");
        tf_broadcaster_->sendTransform(msg);
        
    }

    std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::TimerBase::SharedPtr timer_;
};


int main(int argc, char* argv[]){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<TFBroadcaster>());
    rclcpp::shutdown();

    return 0;
}