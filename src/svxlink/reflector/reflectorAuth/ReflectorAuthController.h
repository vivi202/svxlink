#ifndef REFLECTOR_AUTH_CONTROLLER
#define REFLECTOR_AUTH_CONTROLLER
#include <vector>
#include <memory>
#include "interfaces/IReflectorAuthenticator.h"
//This class should be a singleton to avoid tight coupling with Reflector client
class AuthController
{
  private:
    std::vector<std::unique_ptr<IReflectorAuthenticator>> authenticators;
  public:
    AuthController();
    ~AuthController();
    void addAuthenticator(std::unique_ptr<IReflectorAuthenticator> &auth);
    std::string lookupUserKey(const std::string& callsign);
};



#endif