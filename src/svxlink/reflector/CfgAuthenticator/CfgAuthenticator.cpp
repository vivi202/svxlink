#include "CfgAuthenticator.h"

CfgAuthenticator::CfgAuthenticator(Async::Config *cfg): m_cfg(cfg)
{
    std::cout<<"Using CfgAuthenticator"<<std::endl;
}

std::string CfgAuthenticator::lookupUserKey(const std::string &callsign)
{
    std::string auth_group;
    if (!m_cfg->getValue("USERS", callsign, auth_group) || auth_group.empty())
    {
        std::cout << "*** WARNING: Unknown user \"" << callsign << "\""<< std::endl;
        return "";
    }
  std::string auth_key;
  if (!m_cfg->getValue("PASSWORDS", auth_group, auth_key) || auth_key.empty())
  {
    std::cout << "*** ERROR: User \"" << callsign << "\" found in SvxReflector "
        << "configuration but password with groupname \"" << auth_group
        << "\" not found." << std::endl;
    return "";
  }
  return auth_key;
}

bool CfgAuthenticator::canTalk(const std::string &callsign)
{
    std::string auth_group;
    if(!m_cfg->getValue("USERS", callsign, auth_group) || auth_group.empty())
        return false;
    return true;
}

bool CfgAuthenticator::isEnabled(const std::string &callsign)
{
    std::string auth_group,auth_key;
    if(!m_cfg->getValue("USERS", callsign, auth_group) || auth_group.empty())
        return false;
    if(!m_cfg->getValue("PASSWORDS", auth_group, auth_key) || auth_key.empty())
        return false;
    return true;
}
