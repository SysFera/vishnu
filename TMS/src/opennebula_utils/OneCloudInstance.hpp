/*                                                                             #
# File        : OneHostPool.hpp                                                #
# Creation    : 26/03/2014                                                     #
# Author      : Rodrigue Chakode <rodrigue.chakode@sysfera.com>                #
# Description : Class and header to request data from OpenNebula's XML-RPC API #
*/

#ifndef ONEHOSTPOOL_HPP
#define ONEHOSTPOOL_HPP

#include <vector>
#include <string>
#include <cstring>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/client_simple.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include "TMS_Data/Queue.hpp"

struct HostT {
  int id;
  std::string name;
  double maxCpu;
  double freeCpu;
  double usedCpu;
  double reservedCpu;
  double maxMemory;
  double freeMemory;
  double usedMemory;
  double reservedMemory;
  double runningVms;
  int state;
};
typedef std::vector<HostT> HostPoolT;

struct VmT {
  size_t id;
  std::string name;
  std::string deployId;
  std::string ipAddr;
  size_t memory;
  double cpu;
  int32_t state;
  int32_t lcmState;
};

enum HostStateT {
  INIT                 = 0, // Initial state for enabled hosts
  MONITORING_MONITORED = 1, // Monitoring the host (from monitored)
  MONITORED            = 2, // The host has been successfully monitored
  ERROR                = 3, // An error ocurrer while monitoring the host
  DISABLED             = 4, // The host is disabled won't be monitored
  MONITORING_ERROR     = 5, // Monitoring the host (from error)
  MONITORING_INIT      = 6, // Monitoring the host (from init)
  MONITORING_DISABLED  = 7  // Monitoring the host (from disabled)
};

enum VmStateT {
  VM_INIT      = 0,
  VM_PENDING   = 1,
  VM_HOLD      = 2,
  VM_ACTIVE    = 3, //In this state, the Life Cycle Manager state is relevant
  VM_STOPPED   = 4,
  VM_SUSPENDED = 5,
  VM_DONE      = 6,
  VM_FAILED    = 7,
  VM_POWEROFF  = 8,
  VM_UNDEPLOYED = 9
};

class OneCloudInstance {
public:
  OneCloudInstance(const std::string &rpcUrl, const std::string &authChain);
  void updatePool(void);
  HostPoolT& getHostPool(void) {return mhostPool;}
  int loadVmInfo(int id, VmT& vm);
  TMS_Data::Queue_ptr
  getQueueInfo(void);

private:

  HostPoolT mhostPool;
  std::string mrpcUrl;
  std::string mauthChain;
  std::string mhostPoolFilename;

  xercesc::DOMNodeList* initializeXmlElts(const std::string& xmlFileName,
                                          xercesc::XercesDOMParser*& parser,
                                          const std::string& TAG);
  void releaseXmlElts(xercesc::XercesDOMParser* parser);
  void parseRpcHostPoolResult(const std::string& content);
  void parseRpcVmInfoResult(const std::string& content, VmT& vm);
  void parseHostInfo(xercesc::DOMNode* node, HostT& host);
  void parseVmInfo(xercesc::DOMNode* node, VmT& vm);
  double computeLoad(double load, double maxLoad) {return 100 * load/maxLoad;}
};

#endif // ONEHOSTPOOL_HPP
