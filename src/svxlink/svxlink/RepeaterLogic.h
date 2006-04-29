/**
@file	 RepeaterLogic.h
@brief   A_brief_description_for_this_file
@author  Tobias Blomberg / SM0SVX
@date	 2004-04-24

A_detailed_description_for_this_file

\verbatim
<A brief description of the program or library this file belongs to>
Copyright (C) 2004  Tobias Blomberg / SM0SVX

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
\endverbatim
*/

/** @example RepeaterLogic_demo.cpp
An example of how to use the RepeaterLogic class
*/


#ifndef REPEATER_LOGIC_INCLUDED
#define REPEATER_LOGIC_INCLUDED


/****************************************************************************
 *
 * System Includes
 *
 ****************************************************************************/

#include <string>


/****************************************************************************
 *
 * Project Includes
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Local Includes
 *
 ****************************************************************************/

#include "Logic.h"


/****************************************************************************
 *
 * Forward declarations
 *
 ****************************************************************************/

namespace Async
{
  class Config;
  class Timer;
};


/****************************************************************************
 *
 * Namespace
 *
 ****************************************************************************/

//namespace MyNameSpace
//{


/****************************************************************************
 *
 * Forward declarations of classes inside of the declared namespace
 *
 ****************************************************************************/

class Module;
  

/****************************************************************************
 *
 * Defines & typedefs
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Exported Global Variables
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Class definitions
 *
 ****************************************************************************/

/**
@brief	A_brief_class_description
@author Tobias Blomberg
@date   2004-04-24

A_detailed_class_description

\include RepeaterLogic_demo.cpp
*/
class RepeaterLogic : public Logic
{
  public:
    /**
     * @brief 	Default constuctor
     */
    RepeaterLogic(Async::Config& cfg, const std::string& name);
  
    /**
     * @brief 	Destructor
     */
    ~RepeaterLogic(void);
  
    /**
     * @brief 	A_brief_member_function_description
     * @param 	param1 Description_of_param1
     * @return	Return_value_of_this_member_function
     */
    bool initialize(void);
    
    virtual void processEvent(const std::string& event, const Module *module=0);
    virtual void playFile(const std::string& path);
    virtual void playSilence(int length);
    virtual void playTone(int fq, int amp, int len);
    virtual void moduleTransmitRequest(bool do_transmit);
    virtual bool activateModule(Module *module);
    virtual void dtmfDigitDetected(char digit);


  protected:
    virtual void allTxSamplesFlushed(void);
    virtual void remoteLogicTransmitRequest(bool do_tx);


  private:
    bool      	    repeater_is_up;
    Async::Timer    *up_timer;
    int      	    idle_timeout;
    Async::Timer    *idle_sound_timer;
    int       	    idle_sound_interval;
    bool      	    repeating_enabled;
    bool      	    preserve_idle_state;
    struct timeval  sql_open_timestamp;
    int      	    required_sql_open_duration;
    char      	    open_on_dtmf;
    bool      	    activate_on_sql_close;
    bool            no_repeat;
    
    int audioReceived(short *samples, int count);
    void idleTimeout(Async::Timer *t);
    void setIdle(bool idle);
    void setUp(bool up);
    void squelchOpen(bool is_open);
    //void txTimeout(void);
    void detectedTone(float fq);
    void playIdleSound(Async::Timer *t);

};  /* class RepeaterLogic */


//} /* namespace */

#endif /* REPEATER_LOGIC_INCLUDED */



/*
 * This file has not been truncated
 */

