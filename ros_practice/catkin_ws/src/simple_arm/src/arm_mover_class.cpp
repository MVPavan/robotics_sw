#include "ros/ros.h"
#include "simple_arm/GoToPosition.h"
#include <std_msgs/Float64.h>
#include "local_vars.h"

ros::Publisher joint1_pub, joint2_pub;

std::vector<float> clamp_at_boundaries(float requested_joint1_angle, float requested_joint2_angle){
    float clamped_joint1_angle = requested_joint1_angle;
    float clamped_joint2_angle = requested_joint2_angle;

    float min_j1, max_j1, min_j2, max_j2 {0};
    ros::NodeHandle n;

    std::string node_name = ros::this_node::getName();

    n.getParam(node_name+locals.params.joint1_min_angle, min_j1);
    n.getParam(node_name+locals.params.joint1_max_angle, max_j1);
    n.getParam(node_name+locals.params.joint2_min_angle, min_j2);
    n.getParam(node_name+locals.params.joint2_max_angle, max_j2);

    if(requested_joint1_angle < min_j1 || requested_joint1_angle > max_j1){
        clamped_joint1_angle = std::min(std::max(requested_joint1_angle, min_j1), max_j1);
        ROS_WARN("Joint1 angle is out of bounds, valid range (%1.2f,%1.2f), clamping to: %1.2f", min_j1, max_j1, clamped_joint1_angle);
    }
    if(requested_joint2_angle < min_j2 || requested_joint2_angle > max_j2){
        clamped_joint2_angle = std::min(std::max(requested_joint2_angle, min_j2), max_j2);
        ROS_WARN("Joint2 angle is out of bounds, valid range (%1.2f,%1.2f), clamping to: %1.2f", min_j2, max_j2, clamped_joint2_angle);
    }

    std::vector<float> clamped_angles = {clamped_joint1_angle, clamped_joint2_angle};
    return clamped_angles;
}


bool safe_move_request_handler(simple_arm::GoToPosition::Request& req, simple_arm::GoToPosition::Response& res){
    ROS_INFO("GoToPositionRequest received - j1:%1.2f, j2:%1.2f", (float)req.joint_1, (float)req.joint_2);

    std::vector<float> joint_angles = clamp_at_boundaries(req.joint_1, req.joint_2);
    std_msgs::Float64 joint1_angle, joint2_angle {};
    joint1_angle.data = joint_angles[0];
    joint2_angle.data = joint_angles[1];

    joint1_pub.publish(joint1_angle);
    joint2_pub.publish(joint2_angle);

    ros::Duration(3).sleep();

    res.msg_feedback = "Joint angles set - j1: " + std::to_string(joint_angles[0]) + " , j2: " + std::to_string(joint_angles[1]);
    ROS_INFO_STREAM(res.msg_feedback);
    return true;
}


int main(int argc, char** argv){
    ros::init(argc, argv, locals.nodes.arm_mover);
    ros::NodeHandle n;

    joint1_pub = n.advertise<std_msgs::Float64>(locals.topics.joint1_cmd, 10);
    joint2_pub = n.advertise<std_msgs::Float64>(locals.topics.joint2_cmd, 10);

    ros::ServiceServer service = n.advertiseService(locals.services.safe_move,safe_move_request_handler);
    ROS_INFO("Ready to send joint commands");
    ros::spin();
    return 0;
}
