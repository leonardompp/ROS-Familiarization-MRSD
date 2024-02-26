#include <ros/ros.h>
#include <chatbot_node/reply_msg.h>
#include <message_ui/sent_msg.h>
#include <counter_node/counter.h>
#include <arithmetic_node/arithmetic_reply.h>

int num_reply_msg = 0;
int num_sent_msg = 0;

// TODO: Initialization
ros::Time last_sent_msg_time;
ros::Time last_reply_msg_time;

ros::Subscriber reply_msg_sub;
ros::Subscriber arithmetic_reply_msg_sub;
ros::Subscriber sent_msg_sub;

bool service_callback(counter_node::counter::Request &req, counter_node::counter::Response &resp){
    int16_t request_type = req.req_id;
    switch (request_type) {
        case 0:
            resp.reply = num_sent_msg + num_reply_msg;
            break;
        case 1:
            resp.reply = num_reply_msg;
            break;
        case 2:
            resp.reply = num_sent_msg;
            break;
        case 3:
            resp.reply = (ros::Time::now() - last_reply_msg_time).toSec();
            break;
        case 4:
            resp.reply = (ros::Time::now() - last_sent_msg_time).toSec();
            break;
        default:
            // If there is no such request
            return false;
            break;
    }
    return true;
}

void sent_msg_callback(const message_ui::sent_msg msg)
{
	num_sent_msg++;
	last_sent_msg_time = msg.header.stamp;
}

void reply_msg_callback(const chatbot_node::reply_msg msg)
{
	num_reply_msg++;
	last_reply_msg_time = msg.header.stamp;
}

void arithmetic_reply_msg_callback(const arithmetic_node::arithmetic_reply msg)
{
    num_reply_msg++;
 	last_reply_msg_time = msg.header.stamp;
}

int main(int argc, char **argv) {

  ros::init(argc, argv, "counter_node");
  ros::NodeHandle n;

  reply_msg_sub = n.subscribe("reply_msg", 1000, reply_msg_callback);
  sent_msg_sub = n.subscribe("sent_msg", 1000, sent_msg_callback);
  arithmetic_reply_msg_sub = n.subscribe("arithmetic_reply", 1000, arithmetic_reply_msg_callback);

  // Service handling
  ros::ServiceServer serviceServer = n.advertiseService("message_counter", service_callback);

  ros::Rate loop_rate(20);

  while(ros::ok()) {
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}