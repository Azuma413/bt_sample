#pragma once
#include "rclcpp/rclcpp.hpp"
#include "bt_sample/srv/call_text.hpp"

using namespace std::chrono_literals;

class BTNode : public rclcpp::Node{
    public:
    BTNode() : Node("bt_node"){
        std::cout << "bt_node is called" << std::endl;
        call_text_cli = create_client<bt_sample::srv::CallText>("call_text");
        while(!call_text_cli->wait_for_service(1s)){
            if(!rclcpp::ok()){
                break;
            }
            std::cout << "call_text service not available" << std::endl;

        }
        std::cout << "call_text service available" << std::endl;
    }

    std::string get_text(){
        auto request = std::make_shared<bt_sample::srv::CallText::Request>();
        auto future_result = call_text_cli->async_send_request(request);
        if (rclcpp::spin_until_future_complete(this->shared_from_this(), future_result) == rclcpp::FutureReturnCode::SUCCESS){
            return future_result.get()->text;
        }
        std::cout << "can't get future_result" << std::endl;
        return "";
    }

    private:
    rclcpp::Client<bt_sample::srv::CallText>::SharedPtr call_text_cli;
};

std::shared_ptr<BTNode> ros_node;