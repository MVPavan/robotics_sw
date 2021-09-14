#!/bin/sh

script_dir="$( cd "$( dirname "$0" )" && pwd )"
catkin_ws_dir="$( cd "$script_dir/../.." && pwd )"

cd "$catkin_ws_dir" || exit

xterm -e " source devel/setup.sh; roslaunch turtlebot_gazebo turtlebot_world.launch " &
sleep 5
#xterm -e " source devel/setup.sh; roslaunch turtlebot_gazebo gmapping_demo.launch " &
#sleep 2
xterm -e " source devel/setup.sh; roslaunch turtlebot_gazebo amcl_demo.launch " &
sleep 2
xterm -e " source devel/setup.sh; roslaunch turtlebot_rviz_launchers view_navigation.launch " &
sleep 2
#xterm -e " source devel/setup.sh; roslaunch turtlebot_teleop keyboard_teleop.launch " &
#sleep 2


sleep 10
xterm -e " source devel/setup.sh; rosrun add_markers add_markers " &
sleep 15
xterm -e " source devel/setup.sh; rosrun pick_objects pick_objects " &
#sleep 2
#xterm -e " source devel/setup.sh; rosrun rqt_image_view rqt_image_view "
