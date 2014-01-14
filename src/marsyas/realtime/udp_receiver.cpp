/*
** Copyright (C) 2014 George Tzanetakis <gtzan@cs.uvic.ca>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "udp_receiver.h"
#include "packet_queue.h"

#include <oscpack/ip/UdpSocket.h>
#include <oscpack/ip/PacketListener.h>

#include <cassert>
#include <iostream>
using namespace std;

namespace Marsyas {
namespace RealTime {

class UdpReceiver::Implementation : public PacketListener
{
  packet_queue *m_queue;
  SocketReceiveMultiplexer m_mux;

public:
  Implementation( packet_queue *queue ):
    m_queue(queue)
  {
  }

  void run( const std::string & address, int port )
  {
    UdpSocket socket;
    socket.Bind( IpEndpointName( address.c_str(), port ) );
    m_mux.AttachSocketListener( &socket, this );
    m_mux.Run();
    m_mux.DetachSocketListener( &socket, this );
  }

  void stop()
  {
    m_mux.AsynchronousBreak();
  }

private:

  void ProcessPacket( const char *data, int size,
                      const IpEndpointName& )
  {
    m_queue->push(data, size);
  }
};

UdpReceiver::UdpReceiver( const std::string & address, int port,
                          packet_queue * queue ):
  m_address(address),
  m_port(port),
  m_implementation(new Implementation(queue))
{
}

void UdpReceiver::run()
{
  m_implementation->run(m_address, m_port);
}

void UdpReceiver::stop()
{
  m_implementation->stop();
}

}
}
