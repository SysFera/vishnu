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
}

xercesc::DOMNodeList* OneCloudInstance::initializeXmlElts(const std::string& xmlFileName,
                                                          xercesc::XercesDOMParser*& parser,
                                                          const std::string& TAG)
{
  xercesc::XMLPlatformUtils::Initialize();
  parser = new xercesc::XercesDOMParser();
  parser->setDoNamespaces(true);
  parser->parse(xmlFileName.c_str());

  xercesc::DOMDocument* xmlDoc = parser->getDocument();
  return xmlDoc->getElementsByTagName(xercesc::XMLString::transcode(TAG.c_str()));
}

void OneCloudInstance::releaseXmlElts(xercesc::XercesDOMParser* parser)
{
  delete parser;
  xercesc::XMLPlatformUtils::Terminate();
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
  vishnu::saveInFile(mhostPoolFilename, content);

  try {
    xercesc::XercesDOMParser* hostPoolParser;
    xercesc::DOMNodeList* xmlHosts;

    xmlHosts = initializeXmlElts(mhostPoolFilename, hostPoolParser, "HOST");
    HostT host;

    size_t hostCount = 0;
    if (xmlHosts) {
      mhostPool.clear();
      hostCount = xmlHosts->getLength();
      for(size_t hostIndex = 0; hostIndex <  hostCount; ++hostIndex){
        parseHostInfo(xmlHosts->item(hostIndex), host);
        mhostPool.push_back(host);
      }
    }
    releaseXmlElts(hostPoolParser);
  } catch (const xercesc::XMLException& ex) {
    char* message = xercesc::XMLString::transcode(ex.getMessage());
    LOG(boost::str(boost::format("[ERROR] %1%") %message), 4);
    xercesc::XMLString::release(&message);
  } catch (const xercesc::DOMException& ex) {
    char* message = xercesc::XMLString::transcode(ex.msg);
    LOG(boost::str(boost::format("[ERROR] %1%") %message), 4);
    xercesc::XMLString::release(&message);
  } catch (...) {
    LOG(boost::str(boost::format("[ERROR] Unable to access host pool file: %1%") %mhostPoolFilename), 4);
  }

  vishnu::deleteFile(mhostPoolFilename.c_str());
}


void OneCloudInstance::parseRpcVmInfoResult(const std::string& content, VmT& vm)
{
  vishnu::saveInFile(mhostPoolFilename, content);

  try {
    xercesc::XercesDOMParser* parser;
    xercesc::DOMNodeList* vmInfo;

    vmInfo = initializeXmlElts(mhostPoolFilename, parser, "VM");
    if (vmInfo != NULL && vmInfo->getLength() == 1) {
      parseVmInfo(vmInfo->item(0), vm);
    }
    releaseXmlElts(parser);
  } catch (const xercesc::XMLException& ex) {
    char* message = xercesc::XMLString::transcode(ex.getMessage());
    LOG(boost::str(boost::format("[ERROR] %1%") %message), 4);
    xercesc::XMLString::release(&message);
  } catch (const xercesc::DOMException& ex) {
    char* message = xercesc::XMLString::transcode(ex.msg);
    LOG(boost::str(boost::format("[ERROR] %1%") %message), 4);
    xercesc::XMLString::release(&message);
  } catch (...) {
    LOG(boost::str(boost::format("[ERROR] Unable to access host pool file: %1%") %mhostPoolFilename), 4);
  }

  vishnu::deleteFile(mhostPoolFilename.c_str());
}

void OneCloudInstance::parseHostInfo(xercesc::DOMNode* node, HostT& host)
{
  xercesc::DOMNodeList*  hostNodes = node->getChildNodes();
  size_t hostEltCount =   hostNodes->getLength();
  for (size_t hindex = 0; hindex < hostEltCount; ++hindex) {
    xercesc::DOMNode* hostNode =  hostNodes->item(hindex);

    if( hostNode->getNodeType()
        && hostNode->getNodeType() == xercesc::DOMNode::ELEMENT_NODE ) {

      xercesc::DOMElement* hostElt = dynamic_cast<xercesc::DOMElement*>(hostNode);
      std::string name = xercesc::XMLString::transcode(hostElt->getNodeName());
      std::string text =  xercesc::XMLString::transcode(hostElt->getTextContent());

      if(name == "ID") {
        host.id = atoi(text.c_str());
      } else if(name == "NAME") {
        host.name = text;
      } else if (name == "STATE") {
        host.state = static_cast<int>(atoi(text.c_str()));
      } else if (name == "HOST_SHARE") {

        xercesc::DOMNodeList* hostAttrList = hostNode->getChildNodes();
        size_t hostAttrCount = hostAttrList->getLength();
        for(size_t attr = 0; attr < hostAttrCount; ++attr) {
          xercesc::DOMNode* hostAttrNode = hostAttrList->item(attr);

          if(hostAttrNode->getNodeType()
             && hostAttrNode->getNodeType() == xercesc::DOMNode::ELEMENT_NODE) {

            xercesc::DOMElement* hostAttrElt = dynamic_cast<xercesc::DOMElement*>(hostAttrNode);
            std::string name = xercesc::XMLString::transcode(hostAttrElt->getNodeName());
            std::string text =  xercesc::XMLString::transcode(hostAttrElt->getTextContent());

            if (name == "MEM_USAGE") {
              host.reservedMemory = boost::lexical_cast<double>(text);
            } else if (name == "MAX_MEM") {
              host.maxMemory = boost::lexical_cast<double>(text);
            } else if(name == "FREE_MEM") {
              host.freeMemory = boost::lexical_cast<double>(text);
            } else if(name == "USED_MEM") {
              host.usedMemory = boost::lexical_cast<double>(text);
            } else if (name == "CPU_USAGE") {
              host.reservedCpu = boost::lexical_cast<double>(text);
            } else if (name == "MAX_CPU") {
              host.maxCpu = boost::lexical_cast<double>(text);
            } else if(name == "FREE_CPU"){
              host.freeCpu = boost::lexical_cast<double>(text);
            } else if(name == "USED_CPU") {
              host.usedCpu = boost::lexical_cast<double>(text);
            } else if(name == "RUNNING_VMS") {
              host.runningVms = atoi(text.c_str());
            }
          }
        }
      }  // HOST_SHARE
    }
  }
}

void OneCloudInstance::parseVmInfo(xercesc::DOMNode* node, VmT& vm)
{
  xercesc::DOMNodeList*  vmNodes = node->getChildNodes();
  for (size_t eltIndex = 0, eltCount = vmNodes->getLength();
       eltIndex < eltCount;
       ++eltIndex)
  {
    xercesc::DOMNode* vmNode =  vmNodes->item(eltIndex);
    xercesc::DOMElement* vmElt = dynamic_cast<xercesc::DOMElement*>(vmNode);
    std::string vmEltName = xercesc::XMLString::transcode(vmElt->getNodeName());
    std::string vmEltText =  xercesc::XMLString::transcode(vmElt->getTextContent());

    if(vmEltName == "ID") {
      vm.id = atoi(vmEltText.c_str());
    } else if(vmEltName == "NAME") {
      vm.name = vmEltText;
    } else if (vmEltName == "STATE") {
      vm.state = static_cast<int>(atoi(vmEltText.c_str()));
    } else if (vmEltName == "TEMPLATE") {
      xercesc::DOMNodeList* vmAttrList = vmNode->getChildNodes();
      for (size_t vmAttr = 0, vmAttrCount = vmAttrList->getLength();
           vmAttr < vmAttrCount;
           ++vmAttr)
      {
        xercesc::DOMNode* vmAttrNode = vmAttrList->item(vmAttr);
        xercesc::DOMElement* vmAttrElt = dynamic_cast<xercesc::DOMElement*>(vmAttrNode);
        std::string vmAttrName = xercesc::XMLString::transcode(vmAttrElt->getNodeName());
        std::string vmAttrText =  xercesc::XMLString::transcode(vmAttrElt->getTextContent());

        if (vmAttrName == "NIC") {
          xercesc::DOMNodeList* nicAttrList = vmAttrNode->getChildNodes();

          for (size_t nicAttr = 0, nicAttrCount = nicAttrList->getLength();
               nicAttr < nicAttrCount;
               ++nicAttr)
          {
            xercesc::DOMNode* nicAttrNode = nicAttrList->item(nicAttr);
            xercesc::DOMElement* nicAttrElt = dynamic_cast<xercesc::DOMElement*>(nicAttrNode);
            std::string nicAttrName = xercesc::XMLString::transcode(nicAttrElt->getNodeName());
            std::string nicAttrText =  xercesc::XMLString::transcode(nicAttrElt->getTextContent());

            if (nicAttrName == "IP") {
              vm.ipAddr = nicAttrText;
            }
          }
        } else if (vmAttrName == "CPU") {
          vm.cpu = boost::lexical_cast<double>(vmAttrText);
        } else if(vmAttrName == "MEMORY") {
          vm.memory = boost::lexical_cast<double>(vmAttrText);
        }
      }
    }  // HOST_SHARE
  }
}

void
OneCloudInstance::retrieveCloudInfo(metasched_cloud_t& cloudInfo)
{
  cloudInfo.host_number = mhostPool.size();
  cloudInfo.vm_number   = 0;
  cloudInfo.load_cpu    = 0;
  cloudInfo.load_memory = 0;
  cloudInfo.host_ready  = 0;
  cloudInfo.highest_mean_resource_le_50_p = 0;

  double cpuLoad = 0;
  double memLoad = 0;

  HostPoolT::iterator host;
  HostPoolT::iterator lastHost = mhostPool.end();
  for(host=mhostPool.begin(); host!=lastHost; ++host) {
    switch(host->state) {
      case MONITORING_MONITORED:
      case MONITORED:
        ++cloudInfo.host_ready;
        cloudInfo.vm_number += host->runningVms;
        cloudInfo.load_cpu += host->usedCpu;
        cloudInfo.load_memory += host->usedMemory;

        // compute cpu load
        cpuLoad = computeLoad(host->usedCpu, host->maxCpu);
        if (cpuLoad< 50) {
          ++cloudInfo.cpu_number_hosts_le_50p;
          if (cpuLoad > cloudInfo.highest_mean_resource_le_50_p) {
            cloudInfo.highest_mean_resource_le_50_p = cpuLoad;
          }
        }

        // compute memory load
        memLoad = computeLoad(host->usedMemory, host->maxMemory);
        if ( memLoad< 50) {
          ++cloudInfo.mem_number_hosts_le_50p;
          if (memLoad > cloudInfo.highest_mean_resource_le_50_p) {
            cloudInfo.highest_mean_resource_le_50_p = memLoad;
          }
        }
        break;
      default:
        break;
    }
  }
}


TMS_Data::Queue_ptr OneCloudInstance::getQueueInfo(void)
{
  metasched_cloud_t cloudInfo;
  retrieveCloudInfo(cloudInfo);
  TMS_Data::Queue_ptr queue = new TMS_Data::Queue();
  queue->setDescription(create_cloud_json_object(cloudInfo).encode());
}
