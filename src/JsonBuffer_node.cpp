#include "ros/ros.h"
#include "std_msgs/String.h"
#include "json.hpp"
#include <queue>
#include "json_buffer/ReturnJson.h"

using namespace std;
using json = nlohmann::json;

#define BUFFERSIZE 100

json stringToJson(std::string input);
std::vector<std::string> splitString(std::string& s, char delimiter);
void displayJson(json j);

// Listener -  class that runs callbacks - receive data from nodes, put them in a queue
// and waits for other nodes to send request for a json data in a queue
//
class Listener
{
public:
    Listener(queue<string> &json_queue):my_queue_(json_queue){}

    queue<string> my_queue_;
    void chatterCallback(const std_msgs::String::ConstPtr& msg);
    bool sendJson(json_buffer::ReturnJson::Request  &req,
                  json_buffer::ReturnJson::Response &res);
};

// receives data from nodes
void Listener::chatterCallback(const std_msgs::String::ConstPtr& msg)
{

    if(my_queue_.size()<BUFFERSIZE){

        json j =stringToJson(std::string(msg->data.c_str()));

        std::string s = j.dump();
        my_queue_.push(s);
    }

}
// waits for requests and sends json data further, additionally removes them from queue
bool Listener::sendJson(json_buffer::ReturnJson::Request  &req,
                        json_buffer::ReturnJson::Response &res)
{

    std::string json_string;
    cout<<"My size of queue: "<<my_queue_.size()<<endl;

    if(my_queue_.size()!=0){

        int i =req.number;
        while(my_queue_.size()!=0 && i>0 )
        {
            json_string=my_queue_.front();
            res.jsonFile.push_back( json_string);
            my_queue_.pop();
            --i;
        }
    }else{
        json_string="queue empty";
        res.jsonFile.push_back( json_string);
    }


    return true;
}



int main(int argc, char **argv)
{

    ros::init(argc, argv, "listener");
    queue<string> my_queue;

    ros::NodeHandle n;
    Listener listener(my_queue);
    ros::Subscriber sub = n.subscribe("chatter", BUFFERSIZE, &Listener::chatterCallback, &listener);
    ros::ServiceServer service = n.advertiseService("return_json", &Listener::sendJson, &listener);

    ros::spin();

    return 0;
}

void displayJson(json j){

    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        std::cout << *it << '\n';
    }
}

std::vector<std::string> splitString(std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}


json stringToJson(std::string input)
{
    json j;
    auto data = splitString(input,',');

    std::string::size_type sz;
    j["pi"]=std::stod (data.at(0),&sz);

    j["happy"] = data.at(1)=="1"? true : false;

    j["name"]=data.at(2);

    j["fruits"]={data.at(3), data.at(4)};

    return j;
}


