#ifndef REFLECTOR_CFG_AUTHENTICATOR
#define REFLECTOR_CFG_AUTHENTICATOR
#include "../ReflectorAuth/interfaces/IReflectorAuthenticator.h"
#include <AsyncConfig.h>
#include <iostream>
class CfgAuthenticator : public IReflectorAuthenticator
{
private:
    Async::Config* m_cfg;
public:
    CfgAuthenticator(Async::Config* cfg);
    ~CfgAuthenticator() = default;
    std::string lookupUserKey(const std::string& callsign);
    bool canTalk(const std::string& callsign);
    bool isEnabled(const std::string& callsign);
};




#endif /*REFLECTOR_CFG_AUTHENTICATOR*/