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

    void locate_white_ball(const sensor_msgs::Image& img);
    void feed_velocities(float linear_x, float angular_z);
};

void TargetLocator::feed_velocities(float linear_x, float angular_z){
    ROS_INFO_STREAM("Chasing the target .....");
    target_chaser::ChaseTarget srv;
    srv.request.linear_x = linear_x;
    srv.request.angular_z = angular_z;

    if(!client.call(srv))
        ROS_ERROR("Sorry boss! failed to chase target!!");
}

void TargetLocator::locate_white_ball(const sensor_msgs::Image& img) {
    int x{0}, y{0}, bin_size{80}, azimuth_shift{0}, range{0};
    double count{0}, max_ball_image_size{double(3.14*200*200)};
    float  x_factor{0.01}, z_factor{0.1}, linear_x{0.0}, angular_z{0.0};
    bool chase_it_flag{false};
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
    if (count < max_ball_image_size) {
        if (count == 0){
            azimuth_shift = 0;
            ROS_INFO_STREAM("Target Missing ....!!!");
        }
        else{
            azimuth_shift = img.width / 2 - x / count;
            angular_z = azimuth_shift*z_factor/bin_size;
            linear_x = max_ball_image_size*x_factor/count;
            feed_velocities(linear_x=linear_x,angular_z=angular_z);
            chase_it_flag= true;
        }
    }
    else{
        ROS_INFO_STREAM("Target in custody !!!");
    }
    if(!chase_it_flag){
        feed_velocities(linear_x=linear_x,angular_z=angular_z);
    }

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
