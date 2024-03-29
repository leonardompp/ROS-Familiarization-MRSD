#include <iostream>
#include <ros/ros.h>
#include <arithmetic_node/arithmetic_reply.h>
#include <message_ui/sent_msg.h>
#include <sstream>

arithmetic_node::arithmetic_reply arithmetic_response;
ros::Time start_time;

void message_callback(const message_ui::sent_msg::ConstPtr& msg){
    // Time received of calculation
    arithmetic_response.time_received = ros::Time::now().toSec() - start_time.toSec();

    std::string request = msg->message;

    // Extract numbers (if any) in string
    std::stringstream ss(request);
    float values[2] {0, 0};

    char operation{};

    ss >> values[0];
    ss >> operation; // Read the sign
    ss >> values[1];


    // Parse the string (could use something like regex, but no time to do it)
    if(operation == '+'){
        arithmetic_response.oper_type = "Add";
        arithmetic_response.answer = values[0] + values[1];
        arithmetic_response.time_answered = ros::Time::now().toSec() - start_time.toSec();
    }
    else if(operation == '-'){
        arithmetic_response.oper_type = "Subtract";
        arithmetic_response.answer = values[0] - values[1];
        arithmetic_response.time_answered = ros::Time::now().toSec() - start_time.toSec();
    }
    else if(operation == '*'){
        arithmetic_response.oper_type = "Multiply";
        arithmetic_response.answer = values[0] * values[1];
        arithmetic_response.time_answered = ros::Time::now().toSec() - start_time.toSec();
    }
    else if(operation=='/'){
        arithmetic_response.oper_type = "Divide";
        arithmetic_response.answer = values[0] / values[1];
        arithmetic_response.time_answered = ros::Time::now().toSec() - start_time.toSec();
    }
    else{
        arithmetic_response.time_received = 0;
        arithmetic_response.oper_type = "";
        arithmetic_response.answer = 0;
        arithmetic_response.time_answered = 0;

    }

    arithmetic_response.process_time = arithmetic_response.time_answered - arithmetic_response.time_received;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "arithmetic_node");

    ros::NodeHandle n;

    start_time = ros::Time::now();

    ros::Subscriber msg_receiver = n.subscribe("/sent_msg", 3, message_callback);
    ros::Publisher msg_publisher = n.advertise<arithmetic_node::arithmetic_reply>("/arithmetic_reply", 3);

    ros::Rate loop_rate(20);

    while(ros::ok()) {
        ros::spinOnce();
        arithmetic_response.header.stamp = ros::Time::now();
        msg_publisher.publish(arithmetic_response);
        loop_rate.sleep();
    }

    return 0;
}