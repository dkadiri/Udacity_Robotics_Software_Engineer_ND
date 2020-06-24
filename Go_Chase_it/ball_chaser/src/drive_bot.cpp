#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"  

ros::Publisher motor_command_publisher;

bool handle_drive_request(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res)
{
    geometry_msgs::Twist RobotCmd;

    RobotCmd.linear.x = req.linear_x;
    RobotCmd.angular.z = req.angular_z;

    motor_command_publisher.publish(RobotCmd);

    res.msg_feedback = "Robot is waiting for the command";
    ROS_INFO_STREAM(res.msg_feedback);

    return true;
}

int main(int argc, char** argv)
{
    // Initialize a ROS node
    ros::init(argc, argv, "drive_bot");

    // Create a ROS NodeHandle object
    ros::NodeHandle nh;

    // Inform ROS master that we will be publishing a message of type geometry_msgs::Twist on the robot actuation topic with a publishing queue size of 10
    motor_command_publisher = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    ros::ServiceServer service = nh.advertiseService("/ball_chaser/command_robot", handle_drive_request);
    // TODO: Define a drive /ball_chaser/command_robot service with a handle_drive_request callback function
    ROS_INFO("Ready to send motor commands");

    // TODO: Handle ROS communication events
    ros::spin();

    return 0;
}