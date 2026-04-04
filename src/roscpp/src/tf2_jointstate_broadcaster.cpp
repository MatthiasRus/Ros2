#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "sensor_msgs/msg/joint_state.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include <cmath>
#include <map>
#include <tuple>
#include <utility>
#include <string>
#include <vector>
#include <array>

class TFJointStateBroadcaster : public rclcpp::Node {
public:
    TFJointStateBroadcaster() : Node("tf_joint_state_broadcaster") {
        js_sub_ = this->create_subscription<sensor_msgs::msg::JointState>(
            "/joint_states", 
            10,
            [this](const sensor_msgs::msg::JointState::SharedPtr joint_state){this->callback_handle(joint_state);}
        );
        js_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

        frames_ = {
                        {"joint_1", {"base_link", "link_1"}},
                        {"joint_2", {"link_1",    "link_2"}},
                        {"joint_3", {"link_2",    "link_3"}},
                        {"joint_4", {"link_3",    "link_4"}},
                        {"joint_5", {"link_4",    "link_5"}},
                        {"gripper", {"link_5",    "end_effector"}},
        };

        origins_ = {
                        {"joint_1", {0.0, 0.0, 0.073}},
                        {"joint_2", {0.0, 0.0, 0.022}},
                        {"joint_3", {0.0, 0.0, 0.105}},
                        {"joint_4", {0.0, 0.0, 0.145}},
                        {"joint_5", {0.0, 0.0, 0.065}},
                        {"gripper", {0.0, 0.0, 0.030}},
        };


        axes_   = {
                        {"joint_1", 'z'},
                        {"joint_2", 'y'},
                        {"joint_3", 'y'},
                        {"joint_4", 'y'},
                        {"joint_5", 'x'},
                        {"gripper", 'z'},
        };


    }



private:

        void callback_handle(const sensor_msgs::msg::JointState::SharedPtr joints){
            size_t length = joints->name.size();
            std::string name;
            double theta;
            std::array<double, 4> quat_val = {0.0,0.0,0.0,0.0};
            std::array<double, 3> translation = {0.0,0.0,0.0};
            

            for (size_t i{0}; i<length; i++){
                name = joints->name[i];
                theta = joints->position[i];

                quat_val    = quaternionCalc(name, theta);
                const auto& [parent, child]   = frames_[name]; // i will later check what if the name doesn't exist
                translation =   origins_[name];

                /*
                I will start filling the TransformStamped message
                */
                geometry_msgs::msg::TransformStamped tf_msg_ ;

                tf_msg_.header.stamp =  this->get_clock()->now(); 
                tf_msg_.header.frame_id = parent;
                tf_msg_.child_frame_id = child;

                tf_msg_.transform.translation.x = translation[0];
                tf_msg_.transform.translation.y = translation[1];
                tf_msg_.transform.translation.z = translation[2];

                tf_msg_.transform.rotation.x = quat_val[0];
                tf_msg_.transform.rotation.y = quat_val[1];
                tf_msg_.transform.rotation.z = quat_val[2];
                tf_msg_.transform.rotation.w = quat_val[3];
            
                js_broadcaster_->sendTransform(tf_msg_);
                RCLCPP_INFO(this->get_logger(), "Name: %s Parent: %s  Child: %s Theta: %.2f", name.c_str(), parent.c_str(), child.c_str(), theta );
            }
        }

        std::array<double, 4> quaternionCalc(const std::string &name, double theta){
            char axis = axes_[name];
            std::array<double,4> quaternion = {0.0, 0.0, 0.0, 0.0} ;
            quaternion[3] = std::cos(theta / 2.0);

            if (axis == 'X' || axis == 'x'){
                quaternion[0] = std::sin(theta/2.0);
            }else if(axis == 'Y' || axis == 'y'){
                quaternion[1] = std::sin(theta/2.0);
            }else if (axis == 'Z' || axis == 'z'){
                quaternion[2] = std::sin(theta / 2.0);
            }else{
                std::cout<< "What kind of axis is this " << axis<<std::endl;
            }

            return quaternion;
        };


        std::shared_ptr<tf2_ros::TransformBroadcaster>                     js_broadcaster_;
        rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr      js_sub_;
        std::map<std::string, std::pair<std::string, std::string>>         frames_;
        std::map<std::string, std::array<double,3>>                        origins_;
        std::map<std::string, char>                                        axes_;
        


};



int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TFJointStateBroadcaster>());
    rclcpp::shutdown();
    return 0;
}