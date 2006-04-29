/**
@file	 LocalRx.h
@brief   A receiver class to handle local receivers
@author  Tobias Blomberg
@date	 2004-03-21

This file contains a class that handle local receivers. A local receiver is
a receiver that is directly connected to the sound card on the computer where
the SvxLink core is running.

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


#ifndef LOCAL_RX_INCLUDED
#define LOCAL_RX_INCLUDED


/****************************************************************************
 *
 * System Includes
 *
 ****************************************************************************/

#include <sys/time.h>


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

extern "C" {
#include "fidlib.h"
};

#include "Rx.h"


/****************************************************************************
 *
 * Forward declarations
 *
 ****************************************************************************/

namespace Async
{
  class Config;
  class AudioIO;
};

class ToneDurationDet;
class Squelch;


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

class DtmfDecoder;
class ToneDetector;


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
@brief	A class to handle local receivers
@author Tobias Blomberg
@date   2004-03-21

This class handle local receivers. A local receiver is a receiver that is
directly connected to the sound card on the computer where the SvxLink core
is running.
*/
class LocalRx : public Rx
{
  public:
    /**
     * @brief 	Default constuctor
     */
    explicit LocalRx(Async::Config &cfg, const std::string& name);
  
    /**
     * @brief 	Destructor
     */
    ~LocalRx(void);
  
    /**
     * @brief 	Initialize the receiver object
     * @return 	Return \em true on success, or \em false on failure
     */
    bool initialize(void);
    
    /**
     * @brief 	Mute the receiver
     * @param 	do_mute Set to \em true to mute or \em false to unmute
     */
    void mute(bool do_mute);
    
    /**
     * @brief 	Check the squelch status
     * @return	Return \em true if the squelch is open or else \em false
     */
    bool squelchIsOpen(void) const;
    
    /**
     * @brief 	Call this function to add a tone detector to the RX
     * @param 	fq The tone frequency to detect
     * @param 	bw The bandwidth of the detector
     * @param 	thresh The detection threshold in dB SNR
     * @param 	required_duration The required time in milliseconds that
     *	      	the tone must be active for activity to be reported.
     * @return	Return \em true if the Rx is capable of tone detection or
     *	      	\em false if it's not.
     */
    bool addToneDetector(float fq, int bw, float thresh, int required_duration);

    /**
     * @brief 	Read the current signal strength
     * @return	Returns the signal strength
     */
    float signalStrength(void) const;
    
    /**
     * @brief 	Reset the receiver object to its default settings
     */
    void reset(void);
    

  protected:
    
  private:
    static const int  	      	NPOLES = 4;
    static const int  	      	NZEROS = 4;
    
    Async::AudioIO    	      	*audio_io;
    bool      	      	      	is_muted;
    DtmfDecoder       	      	*dtmf_dec;
    ToneDetector      	      	*ctcss_det;
    Squelch   	      	      	*squelch;
    float     	      	      	xv[NZEROS+1];
    float     	      	      	yv[NPOLES+1];
    std::list<ToneDurationDet*> tone_detectors;

    FidFilter 	      	      	*hpff;
    FidRun    	      	      	*hpff_run;
    FidFunc   	      	      	*hpff_func;
    void      	      	      	*hpff_buf;
    double    	      	      	last_siglev;
    float     	      	      	siglev_offset;
    float     	      	      	siglev_slope;
    FidFilter 	      	      	*deemph;
    FidRun    	      	      	*deemph_run;
    FidFunc   	      	      	*deemph_func;
    void      	      	      	*deemph_buf;
    
    int audioRead(short *samples, int count);
    void resetHighpassFilter(void);
    void highpassFilter(short *samples, int count);

};  /* class LocalRx */


//} /* namespace */

#endif /* LOCAL_RX_INCLUDED */



/*
 * This file has not been truncated
 */

