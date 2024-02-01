#ifndef REFLECTOR_DB_AUTHENTICATOR
#define REFLECTOR_DB_AUTHENTICATOR
#include "../ReflectorAuth/interfaces/IReflectorAuthenticator.h"
#include <AsyncConfig.h>
#include <iostream>
#include <pqxx/pqxx>
class DbAuthenticator : public IReflectorAuthenticator
{
private:
    Async::Config* m_cfg;
    std::unique_ptr<pqxx::connection>dbConn;
    std::string connString;
    void prepareStatement();
public:
    DbAuthenticator(Async::Config* cfg);
    DbAuthenticator(DbAuthenticator &other) = delete;
    void operator=(const DbAuthenticator &rhs)= delete;
    ~DbAuthenticator();
    void InitDb();
    const std::string GetAuthKey(const std::string &callsign);
    std::string lookupUserKey(const std::string &callsign);
    bool canTalk(const std::string &callsign);
    bool isEnabled(const std::string& callsign);
};

#endif