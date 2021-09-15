#Udacity Robotics Software Nano Degree

## Final Project: Home Service Robot

#### Problem Statement: 
Create a Robot that travels to a fixed pickup point, simulate the load pickup using virtual marker and 
finally travel to a fixed drop point and simulate the load drop.

#### Packages used:

* `turtlebot_gazebo`: This provides a Gazebo simulation of the TurtleBot equipped with 2-D lidar and RGB camera .
* `turtlebot_teleop`: To manually control the TurtleBot using keyboard  
* `gmapping`: Performs SLAM and can build a 2-D map of the environment. Used along with teleop to build the map of user environemnt.
* `turtlebot_rviz_launchers`: Rviz package with preconfigured settings to show the robot, map, trajectories etc.
* `AMCL`: Adaptive Monte carlo localization package, used along with generated map for localizing robot while navigation.
* `turtlebot_rviz_launchers`: Rviz package with preconfigured settings to show the robot, map, trajectories etc.
* `actionlib <move_base_msgs>`: Used to publish a pre-defined goal to robot.
* `visualization_msgs`: Once subscribed from Rviz, publishes virtual markers to simulate load pickup and drop.
* `odom <nav_msgs> `: Subscribed to this topic to get the current position in map and accordingly simulate load pick up / drop.
