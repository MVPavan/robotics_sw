//
// Created by pavanmv on 04/07/21.
//

#include "ros/ros.h"
#include "simple_arm/GoToPosition.h"
#include <sensor_msgs/JointState.h>
#include <sensor_msgs/Image.h>
#include "local_vars.h"

class LookAway{
private:
    std::vector<double> joints_last_position {0,0};
    bool moving_state{false}, uniform_image{true};
    ros::ServiceClient client;
    ros::Subscriber sub1,sub2;
    const double tolerance = 0.0005;
public:
    LookAway(int argc, char** argv);
    ~LookAway() = default;
    void joint_states_callback(const sensor_msgs::JointState& js);
    void look_away_callback(const sensor_msgs::Image& img);
    void move_arm_center();
};

void LookAway::move_arm_center(){
    ROS_INFO_STREAM("Moving the arm to the center");
    simple_arm::GoToPosition srv;
    srv.request.joint_1 = 1.57;
    srv.request.joint_2 = 1.57;

    if(!client.call(srv))
        ROS_ERROR("Failed to call service safe_move");
}

void LookAway::joint_states_callback(const sensor_msgs::JointState& js) {
    if(fabs(js.position[0]-joints_last_position[0]) >= tolerance){
        moving_state = true;
        joints_last_position = js.position;
    }
    else
        moving_state = false;
}

void LookAway::look_away_callback(const sensor_msgs::Image& img) {
    uniform_image = true;
    for(int i{0}; i< img.step*img.height; i++){
        if(img.data[i]-img.data[0]!=0){
            uniform_image= false;
            break;
        }
    }
    if(uniform_image && !moving_state)
        move_arm_center();
}

LookAway::LookAway(int argc, char **argv) {
    ros::init(argc,argv,local_vars.nodes.look_away);
    ros::NodeHandle n;

    client = n.serviceClient<simple_arm::GoToPosition>(local_vars.services.safe_move);

    sub1 = n.subscribe(local_vars.topics.joint_states, 10, &LookAway::joint_states_callback, this);
    sub2 = n.subscribe(local_vars.topics.image_raw, 10, &LookAway::look_away_callback, this);

    ros::spin();
}

int main(int argc, char** argv){
    LookAway look_away(argc, argv);
    return 0;
}