//
// Created by pavanmv on 08/07/21.
//

#include "ros/ros.h"
#include "target_chaser/ChaseTarget.h"
#include <sensor_msgs/Image.h>
#include "local_vars.h"

class TargetLocator{
private:
    ros::ServiceClient client;
    ros::Subscriber sub;

public:
    TargetLocator(int argc, char** argv);
    ~TargetLocator() = default;

    void locate_white_ball(const sensor_msgs::Image);
};

void TargetLocator::locate_white_ball(const sensor_msgs::Image img) {
    int x, y, count, x_target {0};
    for(size_t i{0}; i<img.data.size(); i+=3){
        if(
                img.data.at(i)==255 &&
                img.data.at(i+1)==255 &&
                img.data.at(i+1)==255 &&
                i%3==0
        ){
            x += (i/3)%img.width;
            y += (i/3)/img.height;
            count++;
        }
    }
    if(count==0)
        x_target=0;
    else
        x_target = x/count - img.width/2;
}

TargetLocator::TargetLocator(int argc, char** argv){
    ros::init(argc, argv, local_vars.nodes.locate_it);
    ros::NodeHandle n;
    client = n.serviceClient<target_chaser::ChaseTarget>(local_vars.services.chase_step);

    sub = n.subscribe(local_vars.topics.image_raw, 10, &TargetLocator::locate_white_ball, this);
    ros::spin();
}

int main(int argc, char** argv){
    TargetLocator target_locator(argc, argv);
    return 0;
}
