//
// Created by pavanmv on 08/07/21.
//
#include "ros/ros.h"
#include "target_chaser/ChaseTarget.h"
#include <std_msgs/Float64.h>
#include "geometry_msgs/Twist.h"
#include "local_vars.h"

class TargetChaser{
private:
    ros::Publisher drive_command_pub;
    std_msgs::Float64 linear_x, angular_z {};
    geometry_msgs::Twist total_velocity {};
public:
    TargetChaser(int argc, char** argv);
    ~TargetChaser() = default;

    bool chase_request_handler(target_chaser::ChaseTarget::Request& req, target_chaser::ChaseTarget::Response& res);
};



bool
TargetChaser::chase_request_handler(target_chaser::ChaseTarget::Request &req, target_chaser::ChaseTarget::Response &res) {
    total_velocity.linear.x = (float)req.linear_x;
    total_velocity.angular.z = (float)req.angular_z;
//    ROS_INFO("GoToPositionRequest received for linear_X:%1.2f, angular_Z:%1.2f", total_velocity.linear.x, total_velocity.angular.z);
    drive_command_pub.publish(total_velocity);

    res.msg_feedback = "Velocities set to - linear_x: " + std::to_string(total_velocity.linear.x)
            + " , angular_z: " + std::to_string(total_velocity.angular.z);
    ROS_INFO_STREAM(res.msg_feedback);
    return true;
}


TargetChaser::TargetChaser(int argc, char **argv) {
    ros::init(argc, argv, local_vars.nodes.chase_it);
    ros::NodeHandle n;

    drive_command_pub = n.advertise<geometry_msgs::Twist>(local_vars.topics.velocity_cmd, 10);

    ros::ServiceServer service = n.advertiseService(
            local_vars.services.chase_step,
            &TargetChaser::chase_request_handler,
            this
    );
    ROS_INFO("Ready for chase!!");
    ros::spin();
}

int main(int argc, char** argv){
    TargetChaser target_chaser(argc, argv);
    return 0;
}
