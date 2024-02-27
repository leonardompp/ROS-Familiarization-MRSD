#include <ros/ros.h>
#include <chatbot_node/reply_msg.h>
#include <message_ui/sent_msg.h>
#include <string>

using namespace std;

chatbot_node::reply_msg chatbot_response;
string name;


void message_callback(const message_ui::sent_msg::ConstPtr& msg){
    const string request = msg->message;
    string reply;
    if (request == "Hello"){
        reply = "Hello, " + name;
    }
    else if (request == "What is your name?"){
        reply = "My name is MRSD Siri";
    }
    else if (request == "How are you?"){
        reply = "I am fine, thank you";
    }
    else{
        reply = "";
    }
    chatbot_response.message = reply;
}

int main(int argc, char **argv) {

  ros::init(argc, argv, "chatbot_node");
  ros::NodeHandle n;
  n.getParam("/name", name);

  ros::Subscriber msg_receiver = n.subscribe("/sent_msg", 3, message_callback);
  ros::Publisher msg_publisher = n.advertise<chatbot_node::reply_msg>("/reply_msg", 3);

  ros::Rate loop_rate(20);

  while(ros::ok()) {
    ros::spinOnce();
    chatbot_response.header.stamp = ros::Time::now();
    msg_publisher.publish(chatbot_response);
    loop_rate.sleep();
  }

  return 0;
}
