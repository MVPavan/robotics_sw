#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "local_vars.h"

int main(int argc, char** argv){

    ros::init(argc, argv, local_nodes.simple_mover);
    ros::NodeHandle n;

    ros::Publisher joint1_pub = n.advertise<std_msgs::Float64>(local_topics.joint1_cmd, 10);
    ros::Publisher joint2_pub = n.advertise<std_msgs::Float64>(local_topics.joint2_cmd, 10);

    ros::Rate loop_rate(10);
    int start_time (0), elapsed (0);
    while(not start_time) start_time = ros::Time::now().toSec();

    while(ros::ok()){
        elapsed = ros::Time::now().toSec() - start_time;
        std_msgs::Float64 joint1_angle, joint2_angle {};
        joint1_angle.data = sin(2*M_PI*.1*elapsed)*(M_PI/2);
        joint2_angle.data = sin(2*M_PI*.1*elapsed)*(M_PI/2);

        joint1_pub.publish(joint1_angle);
        joint2_pub.publish(joint2_angle);
        loop_rate.sleep();
    }
    return 0;
}