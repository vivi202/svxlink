/**
@file	 NetRx.cpp
@brief   Contains a class that connect to a remote receiver via IP
@author  Tobias Blomberg
@date	 2006-04-14

\verbatim
<A brief description of the program or library this file belongs to>
Copyright (C) 2003 Tobias Blomberg / SM0SVX

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



/****************************************************************************
 *
 * System Includes
 *
 ****************************************************************************/

#include <iostream>
#include <cassert>


/****************************************************************************
 *
 * Project Includes
 *
 ****************************************************************************/

#include <AsyncConfig.h>


/****************************************************************************
 *
 * Local Includes
 *
 ****************************************************************************/

#include "NetRxMsg.h"
#include "NetRx.h"



/****************************************************************************
 *
 * Namespaces to use
 *
 ****************************************************************************/

using namespace std;
using namespace Async;
using namespace NetRxMsg;



/****************************************************************************
 *
 * Defines & typedefs
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Local class definitions
 *
 ****************************************************************************/

class ToneDet
{
  public:
    int fq, bw, required_duration;
    ToneDet(int fq, int bw, int required_duration)
      : fq(fq), bw(bw), required_duration(required_duration) {}
    
};


/****************************************************************************
 *
 * Prototypes
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Exported Global Variables
 *
 ****************************************************************************/




/****************************************************************************
 *
 * Local Global Variables
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Public member functions
 *
 ****************************************************************************/

NetRx::NetRx(Config &cfg, const string& name)
  : Rx(cfg, name), is_muted(true), tcp_con(0), recv_cnt(0), recv_exp(0),
    squelch_open(false), last_signal_strength(0.0)
{
} /* NetRx::NetRx */


NetRx::~NetRx(void)
{
  delete tcp_con;
  
  list<ToneDet*>::iterator it;
  for (it=tone_detectors.begin(); it!=tone_detectors.end(); ++it)
  {
    delete *it;
  }
  tone_detectors.clear();
  
} /* NetRx::~NetRx */


bool NetRx::initialize(void)
{
  string host;
  if (!cfg().getValue(name(), "HOST", host))
  {
    cerr << "*** ERROR: Config variable " << name() << "/HOST not set\n";
    return false;
  }

  string tcp_port(NET_RX_DEFAULT_TCP_PORT);
  cfg().getValue(name(), "TCP_PORT", tcp_port);
  
  string udp_port(NET_RX_DEFAULT_UDP_PORT);
  cfg().getValue(name(), "UDP_PORT", udp_port);
  
  tcp_con = new TcpClient(host, atoi(tcp_port.c_str()));
  tcp_con->connected.connect(slot(this, &NetRx::tcpConnected));
  tcp_con->disconnected.connect(slot(this, &NetRx::tcpDisconnected));
  tcp_con->dataReceived.connect(slot(this, &NetRx::tcpDataReceived));
  recv_exp = sizeof(Msg);
  tcp_con->connect();
  
  return true;
  
} /* NetRx:initialize */


void NetRx::mute(bool do_mute)
{
  cout << "NetRx::mute: do_mute=" << (do_mute ? "true" : "false") << "\n";
  
  if (do_mute == is_muted)
  {
    return;
  }
  
  MsgMute *msg = new MsgMute(do_mute);
  sendMsg(msg);
  
  is_muted = do_mute;
  
  if (do_mute)
  {
    squelch_open = false;
    last_signal_strength = 0.0;
    last_sql_rx_id = 0;
  }
    
} /* NetRx::mute */


bool NetRx::addToneDetector(int fq, int bw, int required_duration)
{
  cout << "addToneDetector: fq=" << fq << " bw=" << bw
       << " required_duration=" << required_duration << endl;
       
  MsgAddToneDetector *msg = new MsgAddToneDetector(fq, bw, required_duration);
  sendMsg(msg);
  
  ToneDet *det = new ToneDet(fq, bw, required_duration);
  tone_detectors.push_back(det);
  
  return true;

} /* NetRx::addToneDetector */


void NetRx::reset(void)
{
  list<ToneDet*>::iterator it;
  for (it=tone_detectors.begin(); it!=tone_detectors.end(); ++it)
  {
    delete *it;
  }
  tone_detectors.clear();
  
  MsgReset *msg = new MsgReset;
  sendMsg(msg);

  is_muted = true;
  squelch_open = false;
  last_signal_strength = 0;
  last_sql_rx_id = 0;

} /* NetRx::reset */




/****************************************************************************
 *
 * Protected member functions
 *
 ****************************************************************************/


/*
 *------------------------------------------------------------------------
 * Method:    
 * Purpose:   
 * Input:     
 * Output:    
 * Author:    
 * Created:   
 * Remarks:   
 * Bugs:      
 *------------------------------------------------------------------------
 */






/****************************************************************************
 *
 * Private member functions
 *
 ****************************************************************************/

void NetRx::tcpConnected(void)
{
  cout << name() << ": Connected to remote receiver at "
       << tcp_con->remoteHost() << ":" << tcp_con->remotePort() << "\n";
  
  MsgMute *msg = new MsgMute(is_muted);
  sendMsg(msg);

  list<ToneDet*>::iterator it;
  for (it=tone_detectors.begin(); it!=tone_detectors.end(); ++it)
  {
    MsgAddToneDetector *msg =
      new MsgAddToneDetector((*it)->fq, (*it)->bw, (*it)->required_duration);
    sendMsg(msg);
  }

} /* NetRx::tcpConnected */


void NetRx::tcpDisconnected(TcpConnection *con,
      	      	      	    TcpConnection::DisconnectReason reason)
{
  cout << name() << ": Disconnected from remote receiver at "
       << con->remoteHost() << ":" << con->remotePort() << "\n";
  
  if (squelch_open)
  {
    setSquelchState(false);
    squelch_open = false;
  }
  
} /* NetRx::tcpDisconnected */


int NetRx::tcpDataReceived(TcpConnection *con, void *data, int size)
{
  //cout << "NetRx::tcpDataReceived: size=" << size << endl;
  
  //Msg *msg = reinterpret_cast<Msg*>(data);
  //cout << "Received a TCP message with type " << msg->type()
  //     << " and size " << msg->size() << endl;
  
  int orig_size = size;
  
  char *buf = static_cast<char*>(data);
  while (size > 0)
  {
    int read_cnt = min(size, recv_exp-recv_cnt);
    memcpy(recv_buf+recv_cnt, buf, read_cnt);
    size -= read_cnt;
    recv_cnt += read_cnt;
    buf += read_cnt;
    
    if (recv_cnt == recv_exp)
    {
      if (recv_exp == sizeof(Msg))
      {
      	Msg *msg = reinterpret_cast<Msg*>(recv_buf);
	if (recv_exp == static_cast<int>(msg->size()))
	{
	  handleMsg(msg);
	  recv_cnt = 0;
	  recv_exp = sizeof(Msg);
	}
	else
	{
      	  recv_exp = msg->size();
	}
      }
      else
      {
      	Msg *msg = reinterpret_cast<Msg*>(recv_buf);
      	handleMsg(msg);
	recv_cnt = 0;
	recv_exp = sizeof(Msg);
      }
    }
  }
  
  return orig_size;
  
} /* NetRx::tcpDataReceived */


void NetRx::handleMsg(Msg *msg)
{
  switch (msg->type())
  {
    case MsgHeartbeat::TYPE:
    {
      msg = reinterpret_cast<MsgHeartbeat*>(msg);
      break;
    }
    case MsgAuth::TYPE:
    {
      msg = reinterpret_cast<MsgAuth*>(msg);
      break;
    }
    case MsgSquelch::TYPE:
    {
      if (!is_muted)
      {
	MsgSquelch *sql_msg = reinterpret_cast<MsgSquelch*>(msg);
	squelch_open = sql_msg->isOpen();
	last_signal_strength = sql_msg->signalStrength();
	last_sql_rx_id = sql_msg->sqlRxId();
      	setSquelchState(squelch_open);
      }
      break;
    }
    case MsgDtmf::TYPE:
    {
      if (!is_muted)
      {
      	MsgDtmf *dtmf_msg = reinterpret_cast<MsgDtmf*>(msg);
      	dtmfDigitDetected(dtmf_msg->digit());
      }
      break;
    }
    case MsgTone::TYPE:
    {
      if (!is_muted)
      {
	MsgTone *tone_msg = reinterpret_cast<MsgTone*>(msg);
	toneDetected(tone_msg->toneFq());
      }
      break;
    }
    case MsgAudio::TYPE:
    {
      if (!is_muted)
      {
	MsgAudio *audio_msg = reinterpret_cast<MsgAudio*>(msg);
	audioReceived(audio_msg->samples(), audio_msg->count());
      }
      break;
    }
    default:
      cerr << "*** ERROR: Unknown TCP message received. Type="
      	   << msg->type() << ", Size=" << msg->size() << endl;
      break;
  }
  
} /* NetRx::handleMsg */


void NetRx::sendMsg(Msg *msg)
{
  if ((tcp_con != 0) && (tcp_con->isConnected()))
  {
    cout << tcp_con->write(msg, msg->size()) << " bytes written\n";
  }
  
  delete msg;
  
} /* NetUplink::sendMsg */




/*
 * This file has not been truncated
 */

