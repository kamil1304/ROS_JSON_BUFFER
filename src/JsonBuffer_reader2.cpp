#include "ros/ros.h"
#include "json_buffer/ReturnJson.h"
#include <cstdlib>
#include <ctime>

using namespace std;
#define NUMBEROFMESSAGES 1

int main(int argc, char **argv)
{
    srand(time(0));

    ros::init(argc, argv, "read_json2");

    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<json_buffer::ReturnJson>("return_json");

    while(true){
        json_buffer::ReturnJson srv;
        srv.request.number = NUMBEROFMESSAGES;

        if (client.call(srv))
        {
            for(int i=0; i<srv.response.jsonFile.size(); ++i)

                cout<<i<<" Transport: "<< srv.response.jsonFile.at(i)<<endl;
        }
        else
        {
            cout<<"Failed to call service read_json"<<endl;
            ros::Duration( 0.5).sleep();

        }

        ros::Duration( (rand()%1000)*0.001).sleep();

    }

    return 0;
}
