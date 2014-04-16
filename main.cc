#include <cstdint>
#include <cstdlib>
#include <list>
#include <string>
#include <thread>
#include "config.h"
#include "env.h"
#include "Client.h"
#include "Network.h"
#include "Robot.h"

#include <iostream> // debug

std::string config::camAddr;
std::string config::dir;
std::string config::serialDev;
std::string config::netAddr;
std::string config::tmpdir;
uint16_t config::maxClients;
uint16_t config::tcpPort;
uint16_t config::udpPort;

std::string env::move;
std::list<Client *> env::cliQueue; 
Client *env::client;
std::mutex env::mtx;
struct botaxes env::posAxes;
struct botaxes env::mvAxes[2];
int8_t env::flag;

int32_t
main()
{
     std::string cmd;

     config::camAddr = "192.168.0.35";
     config::dir = "";
     config::netAddr = "*";
     config::serialDev = "/dev/ttyUSB0";
     config::tmpdir = "/tmp/hercule/";
     config::maxClients = 2;
     config::tcpPort = 4242;
     config::udpPort = 42117;

     env::client = NULL;
     env::flag = -1;

     cmd = "mkdir -p " + config::tmpdir;
     std::system(cmd.data());
     cmd = "rm -f " + config::tmpdir + "*.h2kc";
     std::system(cmd.data());
 
     std::thread robot(Robot::thread);
     std::thread netTCP(Network::tcpThread);
     std::thread netUDP(Network::udpThread);

     robot.join();
     netTCP.join();
     netUDP.join();

     return EXIT_SUCCESS;
}
