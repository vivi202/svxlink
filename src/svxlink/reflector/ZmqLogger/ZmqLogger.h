#ifndef REFLECTOR_LOGGER_ZMQ_LOGGER
#define REFLECTOR_LOGGER_ZMQ_LOGGER

#include<zmq.hpp>
#include <zmq_addon.hpp>
#include "../ReflectorLogger/interfaces/IReflectorLogger.h"

class ZmqLogger : public IReflectorLogger
{
private:
    std::string bindUrl;
    zmq::context_t zmqCtx;//Create Zmq Default context
    zmq::socket_t publisher;
    std::string BuildTgEvent(uint32_t tg,ReflectorClient* client,std::string type);
public:
    ZmqLogger(std::string bindUrl = "tcp://127.0.0.1:5678");
    ~ZmqLogger();
    void talkerStart(uint32_t tg,ReflectorClient* const talker);
    void talkerStop(uint32_t tg,ReflectorClient* const talker);
};



#endif