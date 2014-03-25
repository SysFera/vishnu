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



struct HostT {
  int id;
  std::string name;
  double cpu;
  float freeCpu;
  size_t vmCpuUsage;
  size_t memory;
  size_t freeMemory;
  size_t vmMemoryUsage;
  size_t reserved_cpu;
  size_t reserved_memory;
  size_t runningVms;
  int state;

  bool operator < (const HostT & host) {
    if(freeMemory == host.freeMemory) {
      return freeCpu < host.freeCpu;
    }
    return freeMemory < host.freeMemory;
  }
};
typedef std::vector<HostT> HostPoolT;

class OneHostPool {
public:
  enum HostStateT {
    NEW       = 0,  /**< Initial state for enabled hosts. */
    MONITORING = 1, /**< The host is being monitored. */
    MONITORED  = 2, /**< The host has been successfully monitored. */
    ERROR      = 3, /**< An error ocurrer while monitoring the host. */
    DISABLED   = 4  /**< The host is disabled won't be monitored. */
  } HostState;

  OneHostPool(const std::string &rpcUrl, const std::string &authChain);
  void updatePool(void);

private:

  HostPoolT mhostPool;
  std::string mrpcUrl;
  std::string mauthChain;
  std::string mhostPoolFilename;

  xercesc::DOMNodeList* initializeXmlElts(const std::string& xmlFileName,
                                          xercesc::XercesDOMParser*& parser,
                                          const std::string& TAG);
  void releaseXmlElts(xercesc::XercesDOMParser* parser);
  void parseResult(const std::string& output);
  void parseHostInfo(xercesc::DOMNode* node, HostT& host);
};

#endif // ONEHOSTPOOL_HPP
