#include "rclcpp/rclcpp.hpp"
#include "fmt/format.h"
#include "arm_interface/srv/servo_gripper.hpp"



class GripperServerNode : public rclcpp::Node{
    public:
    GripperServerNode() : Node("gripper_server"){
        service_ = this->create_service<arm_interface::srv::ServoGripper>(
            "set_gripper", 
           [this](const std::shared_ptr<arm_interface::srv::ServoGripper::Request> request,
                  const std::shared_ptr<arm_interface::srv::ServoGripper::Response> response)
                  {this->service_callback(request,response);}
            );
   
        }

    private:
    void service_callback(const std::shared_ptr<arm_interface::srv::ServoGripper::Request> request,
                          const std::shared_ptr<arm_interface::srv::ServoGripper::Response> response){
                           response->current_position = request->position;
                           response->success = true;
                           response->message = "Successful";
                           RCLCPP_INFO(this->get_logger(), "Hey lad, the position updated to %d", response->current_position);
                        }

        rclcpp::Service<arm_interface::srv::ServoGripper>::SharedPtr service_;
};


int main(int argc, char* argv[]){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<GripperServerNode>());
    rclcpp::shutdown();

    return 0;
}