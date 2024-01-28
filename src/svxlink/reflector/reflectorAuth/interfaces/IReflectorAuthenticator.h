#ifndef IREFLECTOR_AUTHENTICATOR
#define IREFLECTOR_AUTHENTICATOR
#include <string>
class IReflectorAuthenticator
{
public:
    virtual std::string lookupUserKey(const std::string& callsign) = 0;
    virtual bool canTalk(const std::string& callsign) = 0;
};

#endif /*IREFLECTOR_AUTHENTICATOR*/