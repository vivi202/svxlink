#ifndef IREFLECTOR_LOGGER_INCLUDED
#define IREFLECTOR_LOGGER_INCLUDED

#include "../../ReflectorClient.h"
#include "../../TGHandler.h"

class IReflectorLogger
{  
    public:
        virtual void talkerStart(uint32_t tg,ReflectorClient* const talker) = 0;
        virtual void talkerStop(uint32_t tg,ReflectorClient* const talker) = 0;
};


#endif