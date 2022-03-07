#include <string>
#include <chrono>

void osc_reportStatus(std::string playingState, size_t numPeers, double tempo, long long timeAtBeat, double beatAtTime);
void osc_send(std::string response);
