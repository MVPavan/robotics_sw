//
// Created by pavanmv on 04/07/21.
//

#ifndef SIMPLE_ARM_LOCAL_VARS_H
#define SIMPLE_ARM_LOCAL_VARS_H

#include <string>

struct Nodes{
    std::string simple_mover {"simple_mover"};
    std::string arm_mover {"arm_mover"};
};

struct Topics{
    std::string joint1_cmd {"/simple_arm/joint_1_position_controller/command"};
    std::string joint2_cmd {"/simple_arm/joint_2_position_controller/command"};
};

struct Services{
    std::string safe_move {"/arm_mover/safe_move"};
};

struct Parameters{
    std::string joint1_min_angle {"/min_joint_1_angle"};
    std::string joint1_max_angle {"/max_joint_1_angle"};
    std::string joint2_min_angle {"/min_joint_2_angle"};
    std::string joint2_max_angle {"/max_joint_2_angle"};
};

struct Locals{
    struct Topics topics;
    struct Nodes nodes;
    struct Services services;
    struct Parameters params;
};

struct Locals locals;

#endif //SIMPLE_ARM_LOCAL_VARS_H

