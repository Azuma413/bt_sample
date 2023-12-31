#include "../include/action_node.hpp"
#include "ament_index_cpp/get_package_share_directory.hpp"

using namespace MyActionNodes;
using namespace BT;

int main(int argc, char* argv[]){
  rclcpp::init(argc, argv);
  ros_node = std::make_shared<BTNode>();
  BehaviorTreeFactory factory;
  factory.registerNodeType<Counter>("Counter");
  factory.registerNodeType<Display>("Display");
  factory.registerNodeType<GetText>("GetText");
  std::string package_path = ament_index_cpp::get_package_share_directory("bt_sample");
  factory.registerBehaviorTreeFromFile(package_path + "/config/main_bt.xml");
  BT::Tree tree = factory.createTree("MainBT");
  printTreeRecursively(tree.rootNode());
  NodeStatus status = NodeStatus::RUNNING;

  while(status == NodeStatus::RUNNING && rclcpp::ok()){
    rclcpp::spin_some(ros_node);
    status = tree.tickOnce();
  }

  rclcpp::shutdown();
  return 0;
}