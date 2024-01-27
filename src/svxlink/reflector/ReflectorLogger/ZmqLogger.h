#ifndef REFLECTOR_LOGGER_ZMQ_LOGGER
#define REFLECTOR_LOGGER_ZMQ_LOGGER
#include "interfaces/IReflectorLogger.h"
class ZmqLogger : public IReflectorLogger
{
private:
    /* data */
public:
    ZmqLogger(/* args */);
    ~ZmqLogger();
    void talkerStart(uint32_t tg,ReflectorClient* const talker);
    void talkerStop(uint32_t tg,ReflectorClient* const talker);
};



#endif