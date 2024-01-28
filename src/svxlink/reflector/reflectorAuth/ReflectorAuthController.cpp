#include "ReflectorAuthController.h"
#include <iostream>

AuthController::AuthController(/* args */)
{
}

AuthController::~AuthController()
{
}

void AuthController::addAuthenticator(std::unique_ptr<IReflectorAuthenticator> &auth)
{
    authenticators.push_back(std::move(auth));
}

std::string AuthController::lookupUserKey(const std::string& callsign){
    for (auto const &authenticator : authenticators){
        try{
            return authenticator->lookupUserKey(callsign);
        }
        catch(const std::exception& e){
            std::cerr << e.what() << '\n';
            return "";
        }
        
    }
    
}
