#include "ReflectorAuthController.h"


ReflectorAuthController* ReflectorAuthController::authControllerInstance = nullptr;
ReflectorAuthController::ReflectorAuthController():checkAuthTimer(1000, Async::Timer::TYPE_PERIODIC)
{
  checkAuthTimer.expired.connect(
    sigc::mem_fun(*this, &ReflectorAuthController::checkAuth)
    );
}

void ReflectorAuthController::checkAuth(Async::Timer *t)
{
    for (auto &&client : ReflectorClient::client_map)
    {
         if(!isEnabled(client.second->callsign())){
             client.second->disconnect();
         }
    }
    
}

ReflectorAuthController::~ReflectorAuthController()
{
}

ReflectorAuthController *ReflectorAuthController::GetInstance()
{

    if(authControllerInstance == nullptr){
        authControllerInstance=new ReflectorAuthController();
        return authControllerInstance;
    }

    return authControllerInstance;
}

void ReflectorAuthController::addAuthenticator(std::unique_ptr<IReflectorAuthenticator> &auth)
{
    authenticators.push_back(std::move(auth));
}

std::string ReflectorAuthController::lookupUserKey(const std::string& callsign){
    for (auto const &authenticator : authenticators){
        try{
            auto authKey = authenticator->lookupUserKey(callsign);
            if(!authKey.empty())
                return authKey;
        }
        catch(const std::exception& e){
            std::cerr << e.what() << '\n';
            return "";
        }
        
    }
    return "";
}

bool ReflectorAuthController::canTalk(uint32_t tg,const std::string &callsign)
{
    
    for (auto const &authenticator : authenticators)
    {
        try
        {
            if(authenticator->canTalk(callsign)){
                return true;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }

    return false;
}

bool ReflectorAuthController::isEnabled(const std::string &callsign)
{
   for (auto const &authenticator : authenticators)
    {
        try
        {
            if(authenticator->isEnabled(callsign)){
                return true;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }

    return false;
}
