#include "ReflectorLoggerController.h"



ReflectorLoggerController::ReflectorLoggerController(IReflectorLogger *reflectorLogger) : reflectorLogger(reflectorLogger)
{
  TGHandler::instance()->talkerUpdated.connect(
  mem_fun(*this, &ReflectorLoggerController::onTalkerUpdated));
  return;
}

ReflectorLoggerController::~ReflectorLoggerController(void)
{

}

void ReflectorLoggerController::onTalkerUpdated(uint32_t tg, ReflectorClient *old_talker, 
                                                            ReflectorClient *new_talker)
{
    if (old_talker != nullptr)
  {
    if(reflectorLogger)
      reflectorLogger->talkerStop(tg,old_talker);
  }
  if (new_talker != nullptr)
  {
    if(reflectorLogger)
      reflectorLogger->talkerStart(tg,new_talker);
  }  
}
