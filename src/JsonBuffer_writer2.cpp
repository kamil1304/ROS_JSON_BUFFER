#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

std::vector<std::string> names={"Anna","Marry","Greg","James","John", "Mark","Eleonora","Cristine","Kamil","Lockland"};
std::vector<std::string> fruits={"apple","orange","kiwi","banana","strawberry","pear","blueberry","cherry","mango","mandarine","melon","grapes"};


std::string createMessage(){

    std::string output;
    output.append("3.141");
    output.append(",");


    int randomval = rand() % 2;
    output.append(std::to_string(randomval));
    output.append(",");

    output.append( names.at(rand() % names.size()) );
    output.append(",");

    output.append( fruits.at(rand() % fruits.size()) );
    output.append(",");
    output.append( fruits.at(rand() % fruits.size()) );

    return output;
}

int main(int argc, char **argv)
{
    srand(time(0));

    ros::init(argc, argv, "talker2");
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

    ros::Rate loop_rate(10);

    int count = 0;
    while (ros::ok())
    {

        std_msgs::String msg;

        std::stringstream ss;
        std::string  message = createMessage();
        ss << message<< count;
        msg.data = ss.str();

        ROS_INFO("%s", msg.data.c_str());

        chatter_pub.publish(msg);

        ros::spinOnce();
        ros::Duration( (rand()%1000)*0.001).sleep();
        //  loop_rate.sleep();
        ++count;
    }


    return 0;
}
