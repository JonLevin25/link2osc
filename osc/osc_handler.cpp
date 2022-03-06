#include "osc_handler.h"
#include "lo/lo.h"
#include "lo/lo_cpp.h"

// NULL is same as localhost for liblo
#define BROADCAST_HOST "196.168.1.255"
// #define DEFAULT_HOST "localhost"
#define DEFAULT_HOST BROADCAST_HOST
#define DEFAULT_PORT "7770"
#define OSC_PATH "/link2osc/update"

lo_address addr = lo_address_new(DEFAULT_HOST, DEFAULT_PORT);

// TODO: get host/port from cmdline args
void osc_send(std::string response)
{
  printf("OSC: (%s:%s/%s):\n", lo_address_get_hostname(addr), lo_address_get_port(addr), OSC_PATH);
  printf("%s\n", response.c_str());

  // 's' means 1 string arg
  lo_send(addr, OSC_PATH, "s", "TEST");
  lo_send(addr, OSC_PATH, "s", response.c_str()); // TODO: send args seperately? send beats?
}
