//
// Created by pavanmv on 04/07/21.
//

#ifndef SIMPLE_ARM_LOCAL_VARS_H
#define SIMPLE_ARM_LOCAL_VARS_H

#include <string>

struct Nodes{
    std::string chase_it {"chase_it"};
};

struct Topics{
    std::string velocity_cmd {"/cmd_vel"};
    std::string joint2_cmd {"/simple_arm/joint_2_position_controller/command"};
    std::string joint_states {"/simple_arm/joint_states"};
    std::string image_raw {"rgb_camera/image_raw"};
};

struct Services{
    std::string chase_step {"/target_chaser/chase_step"};
};

struct Parameters{
    std::string joint1_min_angle {"/min_joint_1_angle"};
    std::string joint1_max_angle {"/max_joint_1_angle"};
    std::string joint2_min_angle {"/min_joint_2_angle"};
    std::string joint2_max_angle {"/max_joint_2_angle"};
};

struct LocalVars{
    struct Topics topics;
    struct Nodes nodes;
    struct Services services;
    struct Parameters params;
};

struct LocalVars local_vars;

#endif //SIMPLE_ARM_LOCAL_VARS_H

