#!/bin/sh

script_dir="$( cd "$( dirname "$0" )" && pwd )"
catkin_ws_dir="$( cd "$script_dir/../.." && pwd )"

cd "$catkin_ws_dir" || exit

xterm -e " source devel/setup.sh; roslaunch my_robot world.launch " &
sleep 5
xterm -e " source devel/setup.sh; rosrun teleop_twist_keyboard teleop_twist_keyboard.py " &
sleep 2
xterm -e " source devel/setup.sh; roslaunch my_robot mapping.launch " &
sleep 2
xterm -e " source devel/setup.sh; rosrun rqt_image_view rqt_image_view "

