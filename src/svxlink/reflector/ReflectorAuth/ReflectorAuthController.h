#ifndef REFLECTOR_AUTH_CONTROLLER
#define REFLECTOR_AUTH_CONTROLLER
#include <vector>
#include <memory>
#include <sigc++/sigc++.h>
#include <iostream>
#include "interfaces/IReflectorAuthenticator.h"
#include <AsyncAtTimer.h>
#include "../TGHandler.h"
#include "../ReflectorClient.h"
class ReflectorAuthController: public sigc::trackable
{
  public: 
  private:
    ReflectorAuthController();
    static ReflectorAuthController* authControllerInstance;
    std::vector<std::unique_ptr<IReflectorAuthenticator>> authenticators;
    Async::Timer checkAuthTimer;
    void checkAuth(Async::Timer *t);
  public:
    ~ReflectorAuthController();
    static ReflectorAuthController* GetInstance();
    //AuthController should not be cloneable
    ReflectorAuthController(ReflectorAuthController &other) = delete;
    //AuthController should not be assignable
    void operator=(const ReflectorAuthController &rhs)= delete;

    void addAuthenticator(std::unique_ptr<IReflectorAuthenticator> &auth);
    
    std::string lookupUserKey(const std::string& callsign);
    bool canTalk(uint32_t tg, const std::string& callsign); 
    bool isEnabled(const std::string& callsign);
};



#endif