#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "arm_interface/action/move_to_position.hpp"

class GripperActionClient : public rclcpp::Node{
    public:
        using MoveToPosition = arm_interface::action::MoveToPosition;
        using HandleGoal     = rclcpp_action::ClientGoalHandle<MoveToPosition>;

        GripperActionClient() : Node("move_to_position"){

            action_client_ = rclcpp_action::create_client<MoveToPosition>(this, "move_to_position");
            timer_         = this->create_wall_timer(
                std::chrono::milliseconds(500),
                std::bind(&GripperActionClient::send_goal, this)     
            );

        }
        void send_goal(){
            timer_->cancel();
            
            if (!action_client_->wait_for_action_server()){
                RCLCPP_INFO(this->get_logger(), "The Action Server is not available");
                rclcpp::shutdown();
            }


            auto goal_msg = MoveToPosition::Goal();
            goal_msg.x = 2.0;
            goal_msg.y = 3.5;
            goal_msg.z = 2.1;

            auto options = rclcpp_action::Client<MoveToPosition>::SendGoalOptions();

            options.goal_response_callback = [this] (const HandleGoal::SharedPtr &goal_handle){
                if (!goal_handle){
                    RCLCPP_ERROR(this->get_logger(), "The Server Rejected the Goal\n");
                    return;
                }
                RCLCPP_INFO(this->get_logger(),"Goal Accepted\n");
            };

            options.feedback_callback = [this](
                const HandleGoal::SharedPtr,
                std::shared_ptr<const MoveToPosition::Feedback> feedback
            ){
                RCLCPP_INFO(this->get_logger(), "Progress: %.1f  |  %s",
                    feedback->progress, feedback->status.c_str());
            };

            options.result_callback =  std::bind(&GripperActionClient::result_callback, this, std::placeholders::_1);


            RCLCPP_INFO(this->get_logger(), "Sending Goal...\n");
           
            action_client_->async_send_goal(goal_msg, options);

        }
    private:
        
        rclcpp_action::Client<MoveToPosition>::SharedPtr action_client_;
        rclcpp::TimerBase::SharedPtr  timer_;

        void result_callback(const HandleGoal::WrappedResult &result){
            switch(result.code){
                case rclcpp_action::ResultCode::SUCCEEDED:
                     break; 
                case rclcpp_action::ResultCode::ABORTED:
                    RCLCPP_ERROR(this->get_logger(), "Action Aborted\n");
                    return;
                case rclcpp_action::ResultCode::CANCELED:
                    RCLCPP_ERROR(this->get_logger(), "Cancelled Action\n");
                    return;
                default:
                    RCLCPP_ERROR(this->get_logger(), "Unkown Code\n");
                    return;
            }

            RCLCPP_INFO(this->get_logger(), "Result: %s", result.result->message.c_str());
            RCLCPP_INFO(this->get_logger(), "Success: %s",
            result.result->success ? "true" : "false");

            rclcpp::shutdown();
        }
};



int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GripperActionClient>());
    rclcpp::shutdown();
    return 0;
}