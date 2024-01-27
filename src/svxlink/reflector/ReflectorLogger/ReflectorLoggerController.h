/**
@file   ReflectorLogger.h
@brief  Log reflector activity
@author Vivi202 
@date   2024-01-27

ReflectorLogger expands svxrflector capabilities by loggin events to a
sql relational db.

\verbatim
<A brief description of the program or library this file belongs to>
Copyright (C) 2003-2024 Vivi202

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



#ifndef REFLECTOR_LOGGER_CONTROLLER_INCLUDED
#define REFLECTOR_LOGGER_CONTROLLER_INCLUDED


/****************************************************************************
 *
 * System Includes
 *
 ****************************************************************************/
#include <stdint.h>
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
#include "../ReflectorClient.h"
#include "../TGHandler.h"
#include "interfaces/IReflectorLogger.h"
/****************************************************************************
 *
 * Forward declarations
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Namespace
 *
 ****************************************************************************/




/****************************************************************************
 *
 * Forward declarations of classes inside of the declared namespace
 *
 ****************************************************************************/



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
@brief  ReflectorLogger Class
@author vivi202
@date   2024-01-24

A_detailed_class_description

\include MyNamespaceTemplate_demo.cpp
*/
class ReflectorLoggerController : public sigc::trackable
{
  public:
    /**
     * @brief   Default constructor
     */
    ReflectorLoggerController(IReflectorLogger* reflectorLogger);

    /**
     * @brief   Disallow copy construction
     */
    ReflectorLoggerController(const ReflectorLoggerController&) = delete;

    /**
     * @brief   Disallow copy assignment
     */
    ReflectorLoggerController& operator=(const ReflectorLoggerController&) = delete;

    /**
     * @brief   Destructor
     */
    ~ReflectorLoggerController(void);
    std::unique_ptr<IReflectorLogger> reflectorLogger;
    
  protected:

  private:

  void onTalkerUpdated(uint32_t tg, ReflectorClient* old_talker,
                         ReflectorClient *new_talker);
};  /* class Template */


#endif /* REFLECTORLOG_INCLUDED */

/*
 * This file has not been truncated
 */
