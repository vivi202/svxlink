#include "DbAuthenticator.h"

void DbAuthenticator::prepareStatement()
{
    dbConn->prepare("getAuthKey",
    "select reflectorUser.password  from reflector_users reflectorUser where callsign = $1 and " 
    "enabled = true and "
    "deleted_at is null");
    dbConn->prepare("canTalk","select reflectorUser.callsign from reflector_users reflectorUser " 
    "where callsign = $1 and "
    "enabled = true and muted != true and deleted_at is null");
    dbConn->prepare("isEnabled","select reflectorUser.callsign from reflector_users reflectorUser " 
    "where callsign = $1 and "
    "enabled = true and deleted_at is null");
}

DbAuthenticator::DbAuthenticator(Async::Config *cfg) : m_cfg(cfg)
{
    connString = cfg->getValue("DbAuthenticator","CONNECT_STRING");
    try
    {
        InitDb();
    }
    catch(const std::exception& e)
    {
        dbConn.reset();
        std::cerr << e.what() << '\n';
    }
    std::cout<<"Using DbAuthenticator"<<std::endl;
}

DbAuthenticator::~DbAuthenticator()
{
    
}

std::string DbAuthenticator::lookupUserKey(const std::string &callsign)
{
    try
    {
        if(dbConn == nullptr)
            InitDb();
        return GetAuthKey(callsign);
    }catch(const pqxx::broken_connection& e){
        std::cerr << e.what() << '\n';
        dbConn.reset();
        InitDb();
        return GetAuthKey(callsign);
    }catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return "";
    }
}

void DbAuthenticator::InitDb()
{
    dbConn = std::make_unique<pqxx::connection>(connString);
    prepareStatement();
}

const std::string DbAuthenticator::GetAuthKey(const std::string &callsign)
{
    pqxx::work tx{*dbConn};
    auto res = tx.exec_prepared1("getAuthKey", callsign);
    std::cout << res.size();
    if (res.size() == 0)
        return "";
    return res.begin().c_str();
}

bool DbAuthenticator::canTalk(const std::string &callsign)
{
    pqxx::work tx{*dbConn};
    auto res = tx.exec_prepared1("canTalk", callsign);
    if(res.size() == 0)
        return false;
    return true;
}

bool DbAuthenticator::isEnabled(const std::string &callsign)
{
    pqxx::work tx{*dbConn};
    auto res = tx.exec_prepared1("isEnabled", callsign);
    if (res.size() == 0)
        return false;
    return true;
}
