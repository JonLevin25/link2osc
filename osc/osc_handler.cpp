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

int generic_handler(const char *path, const char *types, lo_arg ** argv,
                    int argc, lo_message data, void *user_data);

int foo_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, lo_message data, void *user_data);

int blobtest_handler(const char *path, const char *types, lo_arg ** argv,
                     int argc, lo_message data, void *user_data);

void server_error(int num, const char *msg, const char *path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
    fflush(stdout);
}

void osc_init()
{
    /* start a new server on port 7770 */
    lo_server_thread st = lo_server_thread_new("7770", server_error);

    /* add method that will match any path and args */
    lo_server_thread_add_method(st, NULL, NULL, generic_handler, NULL);

    /* add method that will match the path /foo/bar, with two numbers, coerced
     * to float and int */
    lo_server_thread_add_method(st, "/link2osc/beat_at_time", "hd", handleBeatAtTime, NULL);
}

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



int handleBeatAtTime(const char *path, const char *types, lo_arg ** argv,
                int argc, lo_message data, void *user_data)
{
    printf("OSC: handleBeatAtTime: %s <- f:%f, i:%d\n\n", path, argv[0]->f, argv[1]->i);
    long long when;
    double quantum;

    if ((quantum < 2.0) || (quantum > 16.0)) {
      printf("bad-quantum", "Received illegal beat quantum, " + std::to_string(quantum) + "; must be in range 2.0 to 16.0\n");
      return;
    }

      ableton::Link::SessionState sessionState = linkInstance.captureAppSessionState();
      double beat = sessionState.beatAtTime(std::chrono::microseconds(when), quantum);
      std::string response = "beat-at-time { :when " + std::to_string(when) +
        " :quantum " + std::to_string(quantum) +
        " :beat " + std::to_string(beat) + " }\n";
      send(nc, response);
    }
    /* example showing pulling the argument values out of the argv array */
    return 0;
}


// TODO: get host/port from cmdline args
void osc_send(std::string response)
{
  init_lo_addr();
  printf("OSC: (%s:%s/%s):\n", lo_address_get_hostname(addr), lo_address_get_port(addr), OSC_PATH);
  printf("%s\n", response.c_str());

  // 's' means 1 string arg
  lo_send(addr, OSC_PATH, "s", response.c_str()); // TODO: send args seperately? send beats?
}

int foo_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, lo_message data, void *user_data)
{
    /* example showing pulling the argument values out of the argv array */
    printf("foo: %s <- f:%f, i:%d\n\n", path, argv[0]->f, argv[1]->i);
    fflush(stdout);

    return 0;
}


/* catch any incoming messages and display them. returning 1 means that the
 * message has not been fully handled and the server should try other methods */
int generic_handler(const char *path, const char *types, lo_arg ** argv,
                    int argc, lo_message data, void *user_data)
{
    int i;

    printf("unknown osc path: <%s>\n", path);
    for (i = 0; i < argc; i++) {
        printf("arg %d '%c' ", i, types[i]);
        lo_arg_pp((lo_type)types[i], argv[i]);
        printf("\n");
    }
    printf("\n");
    fflush(stdout);

    return 1;
}
