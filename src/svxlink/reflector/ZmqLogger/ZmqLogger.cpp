#include "ZmqLogger.h"

std::string ZmqLogger::BuildTgEvent(uint32_t tg, ReflectorClient *client,std::string type)
{
    Json::Value event;
    event["type"]=type;
    event["callsign"]=client->callsign();
    event["tg"]=tg;
    Json::StreamWriterBuilder wbuilder;
    wbuilder["indentation"]="";
    std::string eventJsonString = Json::writeString(wbuilder, event);
    return eventJsonString;
}

ZmqLogger::ZmqLogger(std::string newBindUrl) : bindUrl(std::move(newBindUrl))
{
    std::cout<<"ZmqSocket listening: "<<bindUrl<<std::endl;
    publisher=zmq::socket_t(zmqCtx,ZMQ_PUB);
    publisher.bind(bindUrl);  
}

ZmqLogger::~ZmqLogger()
{
}

void ZmqLogger::talkerStop(uint32_t tg, ReflectorClient *const talker)
{
   std::string eventJsonString=BuildTgEvent(tg,talker,"TALKER_STOP");
   zmq::message_t msg(eventJsonString.begin(),eventJsonString.end());
   publisher.send(msg,zmq::send_flags::none);
}

void ZmqLogger::talkerStart(uint32_t tg, ReflectorClient *const talker)
{
    std::string eventJsonString=BuildTgEvent(tg,talker,"TALKER_START");
    zmq::message_t msg(eventJsonString.begin(),eventJsonString.end());
    publisher.send(msg,zmq::send_flags::none);
}