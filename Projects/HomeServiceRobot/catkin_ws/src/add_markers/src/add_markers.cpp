//
// Created by pavanmv on 9/13/21.
//

#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "nav_msgs/Odometry.h"

float x_pickup = -4.1;
float y_pickup = -5.1;
float x_drop = 8.0;
float y_drop = -1.0;
float x_target = 0.0;
float y_target = 0.0;
bool at_location = false;

void odomCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
    float x = msg->pose.pose.position.x;
    float y = msg->pose.pose.position.y;

    float diff = std::abs(x-x_target)+std::abs(y-y_target);
//    ROS_INFO("Position-> x: [%f], y: [%f], x_target: [%f], y_target: [%f]", x,y,x_target,y_target);
//    ROS_INFO("Abs Diff: [%f]", diff);
    if(diff<1)
        at_location= true;
    else
        at_location= false;
}


int main( int argc, char** argv )
{
    ros::init(argc, argv, "add_markers");
    ros::NodeHandle n;
    ros::Rate r(1);
    ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

    // Set our initial cube type to be a cube
    uint32_t cube = visualization_msgs::Marker::CUBE;
    uint32_t circle = visualization_msgs::Marker::CYLINDER;

    ros::Subscriber sub = n.subscribe("/odom", 10, odomCallback);

    while (ros::ok())
    {
        while (marker_pub.getNumSubscribers() < 1)
        {
            if (!ros::ok())
            {
                return 0;
            }
            ROS_WARN("Please create a subscriber to the marker");
            sleep(1);
        }

        visualization_msgs::Marker marker;
        // Set the frame ID and timestamp.  See the TF tutorials for information on these.
        marker.header.frame_id = "map";
        marker.header.stamp = ros::Time::now();

        // Set the namespace and id for this marker.  This serves to create a unique ID
        // Any marker sent with the same namespace and id will overwrite the old one
        marker.ns = "add_markers";
        marker.id = 0;

        // Set the scale of the marker -- 1x1x1 here means 1m on a side
        marker.scale.x = 0.3;
        marker.scale.y = 0.3;
        marker.scale.z = 0.3;

        // Set the color -- be sure to set alpha to something non-zero!
        marker.color.r = 0.0f;
        marker.color.g = 0.0f;
        marker.color.b = 1.0f;
        marker.color.a = 1.0;

        // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
        marker.type = cube;
        marker.lifetime = ros::Duration();

        // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
        marker.pose.position.x = x_pickup;
        marker.pose.position.y = y_pickup;
        marker.pose.position.z = 0;
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 1.0;

        // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
        marker.action = visualization_msgs::Marker::ADD;

        marker_pub.publish(marker);
        ROS_INFO("Published Pick up Point!");

        x_target = x_pickup;
        y_target = y_pickup;
        sleep(1);
        ros::spinOnce();


        while(!at_location){
            ros::spinOnce();
            sleep(1);
        }
        ROS_INFO("Reached Pick up Point!");
        marker.action = visualization_msgs::Marker::DELETE;
        marker_pub.publish(marker);
        sleep(1);
        x_target = x_drop;
        y_target = y_drop;
        sleep(1);
        ros::spinOnce();
        sleep(4);



        marker.pose.position.x = x_drop;
        marker.pose.position.y = y_drop;
        while(!at_location){
            ros::spinOnce();
            sleep(1);
            ROS_INFO("Drop point-> x: [%f], y: [%f]", x_drop, y_drop);

        }
        ROS_INFO("Reached Drop Point!");
        marker.action = visualization_msgs::Marker::ADD;
        marker_pub.publish(marker);
        sleep(15);

        r.sleep();
        marker.action = visualization_msgs::Marker::DELETEALL;
        marker_pub.publish(marker);
        ROS_INFO("Episode reset!");
    }
}