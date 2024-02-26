#include <iostream>
#include <ros/ros.h>
#include <arithmetic_node/arithmetic_reply.h>
#include <message_ui/sent_msg.h>

arithmetic_node::arithmetic_reply arithmetic_response;

void message_callback(const message_ui::sent_msg::ConstPtr& msg){
    ;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "arithmetic_node");

    ros::NodeHandle n;

    ros::Subscriber msg_receiver = n.subscribe("/sent_msg", 3, message_callback);
    ros::Publisher msg_publisher = n.advertise<arithmetic_node::arithmetic_reply>("/arithmetic_reply", 3);

    ros::Rate loop_rate(20);

    while(ros::ok()) {
        ros::spinOnce();
        msg_publisher.publish(arithmetic_response);
        loop_rate.sleep();
    }

    return 0;
}