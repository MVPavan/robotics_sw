//
// Created by pavanmv on 04/07/21.
//

#include "ros/ros.h"
#include "simple_arm/GoToPosition.h"
#include <std_msgs/Float64.h>
#include "local_vars.h"

class ArmMover{
private:
    ros::Publisher joint1_pub, joint2_pub;
    std_msgs::Float64 joint1_angle, joint2_angle {};
public:
    ArmMover(int argc, char** argv);
    ~ArmMover() = default;
    void clamp_at_boundaries();
    bool safe_move_request_handler(simple_arm::GoToPosition::Request& req, simple_arm::GoToPosition::Response& res);

};

void ArmMover::clamp_at_boundaries(){

    float min_j1, max_j1, min_j2, max_j2 {0};
    ros::NodeHandle n;

    const std::string& node_name = ros::this_node::getName();

    n.getParam(node_name+locals.params.joint1_min_angle, min_j1);
    n.getParam(node_name+locals.params.joint1_max_angle, max_j1);
    n.getParam(node_name+locals.params.joint2_min_angle, min_j2);
    n.getParam(node_name+locals.params.joint2_max_angle, max_j2);

    if(joint1_angle.data < min_j1 || joint1_angle.data > max_j1){
        joint1_angle.data = std::min(std::max((float)joint1_angle.data, min_j1), max_j1);
        ROS_WARN("Joint1 angle is out of bounds, valid range (%1.2f,%1.2f), clamping to: %1.2f", min_j1, max_j1, joint1_angle.data);
    }
    if(joint2_angle.data < min_j2 || joint2_angle.data > max_j2){
        joint2_angle.data = std::min(std::max((float)joint2_angle.data, min_j2), max_j2);
        ROS_WARN("Joint2 angle is out of bounds, valid range (%1.2f,%1.2f), clamping to: %1.2f", min_j2, max_j2, joint2_angle.data);
    }
}

bool ArmMover::safe_move_request_handler(simple_arm::GoToPosition::Request& req, simple_arm::GoToPosition::Response& res){
    joint1_angle.data = (float)req.joint_1;
    joint2_angle.data = (float)req.joint_2;
    ROS_INFO("GoToPositionRequest received - j1:%1.2f, j2:%1.2f", joint1_angle.data, joint2_angle.data);

    clamp_at_boundaries();

    joint1_pub.publish(joint1_angle);
    joint2_pub.publish(joint2_angle);

    ros::Duration(3).sleep();

    res.msg_feedback = "Joint angles set - j1: " + std::to_string(joint1_angle.data) + " , j2: " + std::to_string(joint1_angle.data);
    ROS_INFO_STREAM(res.msg_feedback);
    return true;
}

ArmMover::ArmMover(int argc, char **argv) {
    ros::init(argc, argv, locals.nodes.arm_mover);
    ros::NodeHandle n;

    joint1_pub = n.advertise<std_msgs::Float64>(locals.topics.joint1_cmd, 10);
    joint2_pub = n.advertise<std_msgs::Float64>(locals.topics.joint2_cmd, 10);

    ros::ServiceServer service = n.advertiseService(locals.services.safe_move, &ArmMover::safe_move_request_handler, this);
    ROS_INFO("Ready to send joint commands");
    ros::spin();
}

int main(int argc, char** argv){
    ArmMover arm_mover(argc, argv);
    return 0;
}
