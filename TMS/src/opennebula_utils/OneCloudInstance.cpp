/*                                                                             #
# File        : OneHostPool.hpp                                                #
# Creation    : 26/03/2014                                                     #
# Author      : Rodrigue Chakode <rodrigue.chakode@sysfera.com>                #
# Description : Class and header to request data from OpenNebula's XML-RPC API #
*/


#include "OneCloudInstance.hpp"
#include "OneRPCManager.hpp"
#include "utilServer.hpp"
#include "tmsUtils.hpp"
#include <boost/scoped_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include "Logger.hpp"

OneCloudInstance::OneCloudInstance(const std::string& rpcUrl, const std::string &authChain)
  : mrpcUrl(rpcUrl),
    mauthChain(authChain),
    mhostPoolFilename(boost::filesystem::unique_path("/tmp/one.hoostpool%%%%%%.xml").string())
{
  xmlXPathInit();
}

OneCloudInstance::~OneCloudInstance() {
  xmlCleanupParser();
}

void OneCloudInstance::updatePool(void)
{
  OneRPCManager rpcManager(mrpcUrl);
  rpcManager.setMethod("one.hostpool.info");
  rpcManager.addParam(mauthChain);

  rpcManager.execute();

  if (rpcManager.lastCallSucceeded()) {
    parseRpcHostPoolResult(rpcManager.getStringResult());
  } else {
    LOG(boost::str(boost::format("[ERROR] %1%") % rpcManager.getStringResult()), 4);
  }
}


int OneCloudInstance::loadVmInfo(int id, VmT& vm)
{

  int retCode = -1;
  OneRPCManager rpcManager(mrpcUrl);
  rpcManager.setMethod("one.vm.info");
  rpcManager.addParam(mauthChain);
  rpcManager.addParam(id);

  rpcManager.execute();

  if (rpcManager.lastCallSucceeded()) {
    parseRpcVmInfoResult(rpcManager.getStringResult(), vm);
    retCode = 0;
  } else {
    LOG(boost::str(boost::format("[ERROR] %1%") %rpcManager.getStringResult()), 4);
  }
  return retCode;
}

void OneCloudInstance::parseRpcHostPoolResult(const std::string& content)
{
  xmlDocPtr doc(0);
  xmlXPathContextPtr xpathCtx(0);
  xmlXPathObjectPtr xpathObj(0);
  HostT host;


  size_t length = content.length() + 1;
  doc = xmlParseMemory(content.c_str(), length);

  xpathCtx = xmlXPathNewContext(doc);
  if (xpathCtx == NULL) {
    goto cleanup;
  }

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"//HOST", xpathCtx);
  if (xpathObj == NULL) {
    goto cleanup;
  }

  if (xpathObj->type != XPATH_NODESET) {
    goto cleanup;
  }

  for (int i = 0; i < xpathObj->nodesetval->nodeNr; ++i) {
    xmlNodePtr node = xpathObj->nodesetval->nodeTab[i];
    parseHostInfo(node, host);
    mhostPool.push_back(host);
  }

cleanup:
  xmlXPathFreeObject(xpathObj);
  xmlXPathFreeContext(xpathCtx);
  xmlFreeDoc(doc);
}


void OneCloudInstance::parseRpcVmInfoResult(const std::string& content, VmT& vm)
{
  xmlDocPtr doc(0);
  xmlXPathContextPtr xpathCtx(0);
  xmlXPathObjectPtr xpathObj(0);

  size_t length = content.length() + 1;
  doc = xmlParseMemory(content.c_str(), length);

  xpathCtx = xmlXPathNewContext(doc);
  if (xpathCtx == NULL) {
    goto cleanup;
  }

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"//VM", xpathCtx);
  if (xpathObj == NULL) {
    goto cleanup;
  }

  if (xpathObj->type != XPATH_NODESET) {
    goto cleanup;
  }

  for (int i = 0; i < xpathObj->nodesetval->nodeNr; ++i) {
    xmlNodePtr node = xpathObj->nodesetval->nodeTab[i];
    parseVmInfo(node, vm);
  }

cleanup:
  xmlXPathFreeObject(xpathObj);
  xmlXPathFreeContext(xpathCtx);
  xmlFreeDoc(doc);
}

void OneCloudInstance::parseHostInfo(xmlNodePtr node, HostT& host)
{
  xmlNodePtr current(0);

  for (current = node->children; current == 0; current = current->next) {
    if (current->type != XML_ELEMENT_NODE) {
      continue;
    }
    if (current->name == (const xmlChar*)"ID") {
      host.id = atoi((const char*)current->content);
    } else if (current->name == (const xmlChar*)"NAME") {
      host.name = atoi((const char*)current->content);
    } else if (current->name == (const xmlChar*)"STATE") {
      host.state = static_cast<int>(atoi((const char*)current->content));
    } else if (current->name == (const xmlChar*)"HOST_SHARE") {
      xmlNodePtr current2(0);
      for (current2 = node->children; current2 == 0; current2 = current2->next) {
        if (current2->type != XML_ELEMENT_NODE) {
          continue;
        }
        if (current2->name == (const xmlChar*)"MEM_USAGE") {
          host.reservedMemory = boost::lexical_cast<double>(current2->content);
        } else if (current2->name == (const xmlChar*)"MAX_MEM") {
          host.maxMemory = boost::lexical_cast<double>(current2->content);
        } else if(current2->name == (const xmlChar*)"FREE_MEM") {
          host.freeMemory = boost::lexical_cast<double>(current2->content);
        } else if(current2->name == (const xmlChar*)"USED_MEM") {
          host.usedMemory = boost::lexical_cast<double>(current2->content);
        } else if (current2->name == (const xmlChar*)"CPU_USAGE") {
          host.reservedCpu = boost::lexical_cast<double>(current2->content);
        } else if (current2->name == (const xmlChar*)"MAX_CPU") {
          host.maxCpu = boost::lexical_cast<double>(current2->content);
        } else if(current2->name == (const xmlChar*)"FREE_CPU"){
          host.freeCpu = boost::lexical_cast<double>(current2->content);
        } else if(current2->name == (const xmlChar*)"USED_CPU") {
          host.usedCpu = boost::lexical_cast<double>(current2->content);
        } else if(current2->name == (const xmlChar*)"RUNNING_VMS") {
          host.runningVms = atoi((const char *)current2->content);
        }
      }
    }
  }
}

void OneCloudInstance::parseVmInfo(xmlNodePtr node, VmT& vm)
{
  xmlNodePtr current(0);

  for (current = node->children; current == 0; current = current->next) {
    if (current->type != XML_ELEMENT_NODE) {
      continue;
    }
    if (current->name == (const xmlChar*)"ID") {
      vm.id = atoi((const char*)current->content);
    } else if (current->name == (const xmlChar*)"NAME") {
      vm.name = atoi((const char*)current->content);
    } else if (current->name == (const xmlChar*)"STATE") {
      vm.state = static_cast<int>(atoi((const char*)current->content));
    } else if (current->name == (const xmlChar*)"TEMPLATE") {
      xmlNodePtr current2(0);
      for (current2 = current->children; current2 == 0; current2 = current2->next) {
        if (current2->type != XML_ELEMENT_NODE) {
          continue;
        }
        if (current2->name == (const xmlChar*)"NIC") {
          xmlNodePtr current3(0);
          for (current3 = current2->children; current3 == 0; current3 = current3->next) {
            if (current3->name == (const xmlChar*)"IP") {
              vm.ipAddr = (const char*)current3->content;
              break;
            }
          }
        } else if (current2->name == (const xmlChar*)"CPU") {
          vm.cpu = boost::lexical_cast<double>(current2->content);
        } else if(current2->name == (const xmlChar*)"MEMORY") {
          vm.memory = boost::lexical_cast<double>(current2->content);
        }
      }
    }
  }
}
