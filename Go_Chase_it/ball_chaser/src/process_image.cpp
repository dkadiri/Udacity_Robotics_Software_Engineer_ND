#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

//Global client
ros::ServiceClient client;

void drive_robot(float lin_x, float ang_z){
	ball_chaser::DriveToTarget srv;

	srv.request.linear_x = lin_x;
	srv.request.angular_z = ang_z;

	if (!client.call(srv)) {
		ROS_ERROR("Failed to call service drive_robot");
	}

}

void process_image_callback(const sensor_msgs::Image img)
{

	int white_pixel = 255;

	for (int i = 0; i < img.height * img.step; i++) {
		if (img.data[i] == white_pixel){
			ROS_INFO("White ball is detected and moving towards it");
      		if (i % img.step <= 0.3 * img.step && i % img.step > 0){
        		ROS_INFO("Turning Left with 0.2 velocity");
        		drive_robot(0.0, 0.2);
      		} else if (i % img.step > 0.7 * img.step && i % img.step <= img.step){
				ROS_INFO("Turning Right with 0.2 velocity");
				drive_robot(0.0, -0.2);
			} else if (i % img.step > 0.3 * img.step && i % img.step <= 0.7 * img.step){
				ROS_INFO("Moving Forward with 0.2 velocity");
				drive_robot(0.2, 0.0);
			}
		break;
		}
	}
}

int main(int argc, char** argv)
{
	//initialize ros node
	ros::init(argc, argv, "process_image");

	//node handle
	ros::NodeHandle nh;

	//service client
	client = nh.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

	//subscriber
	ros::Subscriber sub1 = nh.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

	//Handle ROS communication events
	ros::spin();

	return 0;
}