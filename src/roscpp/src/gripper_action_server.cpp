#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "arm_interface/action/move_to_position.hpp"
#include <thread>

class GripperActionServer : public rclcpp::Node{
    public:
    using MoveToPosition = arm_interface::action::MoveToPosition;
    using GoalHandle = rclcpp_action::ServerGoalHandle<MoveToPosition>;

    GripperActionServer() : Node("gripper_action_server"){
        action_server_ = rclcpp_action::create_server<MoveToPosition>(
            this,
            "move_to_position",
            std::bind(&GripperActionServer::handle_goal,     this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&GripperActionServer::handle_cancel,   this, std::placeholders::_1),
            std::bind(&GripperActionServer::handle_accepted, this, std::placeholders::_1)
        );
        RCLCPP_INFO(this->get_logger(), "The Server is READY...\n");
    };

    private:
    rclcpp_action::Server<MoveToPosition>::SharedPtr action_server_;

    rclcpp_action::GoalResponse handle_goal(
        const rclcpp_action::GoalUUID &uuid,
        std::shared_ptr<const MoveToPosition::Goal> goal
    ){
       (void)uuid;
       RCLCPP_INFO(this->get_logger(), "The goal recieved is x=%.2f y=%.2f z=%.2f", goal->x, goal->y, goal->z);
       
       // for now let's accept everything
       return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    rclcpp_action::CancelResponse handle_cancel(
       const std::shared_ptr<GoalHandle> goal_handle
    ){
        (void)goal_handle; // some suppressing on the compiler (unused variable)
        RCLCPP_INFO(this->get_logger(), "GOAL Cancel Requested\n");
        return rclcpp_action::CancelResponse::ACCEPT;
    }

    // detached from the main thread
   void handle_accepted(
        const std::shared_ptr<GoalHandle> goal_handle
    ){
       std::thread([this, goal_handle](){
        this->execute(goal_handle);
       }).detach();
    }

   void execute(const std::shared_ptr<GoalHandle> goal_handle){
    RCLCPP_INFO(this->get_logger(), "Starting Execution... \n");
    auto feedback = std::make_shared<MoveToPosition::Feedback>();
    auto result  =  std::make_shared<MoveToPosition::Result>();


    // simulation
    for (int i{0} ; i<10; i++){
        if (goal_handle->is_canceling()){
             result->success = false;
             result->message = "Cancelled.";
             goal_handle->canceled(result);
             RCLCPP_INFO(this->get_logger(), "Goal Cancelled");
             return;
            }
            
            feedback->progress = i / 10.0;
            feedback->status = "Moving...";

            goal_handle->publish_feedback(feedback);

            RCLCPP_INFO(this->get_logger(), "Progress : %.1f", feedback->progress);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // Goal Completed 
    result->success = true;
    result->message = "Target Reached";
    result->joint_angles = {0, 0.4, 0.577, 1, 0};
    goal_handle->succeed(result);
    RCLCPP_INFO(this->get_logger(), "Goal Succeeded");

  }

};


int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GripperActionServer>());
    rclcpp::shutdown();
    return 0;
}