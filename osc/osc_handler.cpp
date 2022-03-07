#include "osc_handler.h"
#include "lo/lo.h"
#include "lo/lo_cpp.h"

#include <string>
#include <gflags/gflags.h>

// NULL is same as localhost for liblo
#define BROADCAST_HOST "196.168.1.255"
// #define DEFAULT_HOST "localhost"
#define OSC_PATH "/link2osc/update"

DEFINE_string(osc_port, "4560", "OSC port");
DEFINE_string(osc_host, BROADCAST_HOST, "OSC port");


lo_address addr;

void init_lo_addr()
{
  static bool was_init = false;
  if (was_init) return;

  addr = lo_address_new(FLAGS_osc_host.c_str(), FLAGS_osc_port.c_str());
}

void osc_reportStatus(std::string playingState, int16_t numPeers, double tempo, long long timeAtBeat, double beatAtTime)
{
  lo_send(addr, OSC_PATH, "sidhd", playingState, numPeers, tempo, timeAtBeat, beatAtTime);
}


// TODO: get host/port from cmdline args
void osc_send(std::string response)
{
  init_lo_addr();
  printf("OSC: (%s:%s/%s):\n", lo_address_get_hostname(addr), lo_address_get_port(addr), OSC_PATH);
  printf("%s\n", response.c_str());

  // 's' means 1 string arg
  lo_send(addr, OSC_PATH, "s", "TEST");
  lo_send(addr, OSC_PATH, "s", response.c_str()); // TODO: send args seperately? send beats?
}
