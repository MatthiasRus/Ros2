cmake_minimum_required(VERSION 3.8)
project(roscpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(arm_interface REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rclcpp REQUIRED)

ament_export_dependencies(rclcpp)

add_executable(
  PublisherNode src/publisher_node.cpp)
add_executable(
  SubscriberNode src/subscriber_node.cpp)

  add_executable(ServoPublisherNode src/servo_publisher.cpp)
add_executable(ServoSubscriberNode src/servo_subscriber.cpp)

  ament_target_dependencies(PublisherNode std_msgs rclcpp)
  ament_target_dependencies(SubscriberNode  std_msgs rclcpp)

ament_target_dependencies( ServoPublisherNode rclcpp arm_interface)
ament_target_dependencies(ServoSubscriberNode  rclcpp arm_interface)

install(TARGETS
  PublisherNode
  SubscriberNode
  ServoPublisherNode
  ServoSubscriberNode
  DESTINATION lib/${PROJECT_NAME}
)
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)
install(DIRECTORY config
  DESTINATION share/${PROJECT_NAME}
)
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
