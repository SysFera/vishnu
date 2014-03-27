/*                                                                             #
# File        : OneHostPool.hpp                                                #
# Creation    : 26/03/2014                                                     #
# Author      : Rodrigue Chakode <rodrigue.chakode@sysfera.com>                #
# Description : Class and header to request data from OpenNebula's XML-RPC API #
*/


#include "OneHostPool.hpp"
#include "OneRPCManager.hpp"
#include <boost/scoped_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>


OneHostPool::OneHostPool(const std::string& rpcUrl, const std::string &authChain)
  : mrpcUrl(rpcUrl),
    mauthChain(authChain),
    mhostPoolFilename(boost::filesystem::unique_path("/tmp/one.hoostpool%%%%%%.xml").string())
{
  updatePool();
  std::clog << "Host count: "<<mhostPool.size()<<"\n";
}

xercesc::DOMNodeList* OneHostPool::initializeXmlElts(const std::string& xmlFileName,
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

void OneHostPool::releaseXmlElts(xercesc::XercesDOMParser* parser)
{
  delete parser;
  xercesc::XMLPlatformUtils::Terminate();
}

void OneHostPool::updatePool(void)
{
  OneRPCManager rpcManager(mrpcUrl);

  rpcManager.setMethod("one.hostpool.info");
  rpcManager.addParam(static_cast<xmlrpc_c::value_string>(mauthChain));
  rpcManager.execute();

  if (rpcManager.lastCallSucceeded()) {
    parseResult(rpcManager.getStringResult());
    rmdir(mhostPoolFilename.c_str());
  } else {
    std::clog << rpcManager.getStringResult()<<"\n";
  }
}

void OneHostPool::parseResult(const std::string& output)
{
  std::ofstream file;
  file.open(mhostPoolFilename.c_str(), std::ios::out);
  file << output;
  file.close();

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
    std::clog << message<< "\n";
    xercesc::XMLString::release(&message);
  } catch (const xercesc::DOMException& ex) {
    char* message = xercesc::XMLString::transcode(ex.msg);
    std::clog << message<< "\n";
    xercesc::XMLString::release(&message);
  } catch (...) {
    std::clog << "Unable to access host pool file: "<< mhostPoolFilename<< "\n";
  }
}


void OneHostPool::parseHostInfo(xercesc::DOMNode* node, HostT& host)
{
  xercesc::DOMNodeList*  hostNodes = node->getChildNodes();
  size_t hostEltCount =   hostNodes->getLength();
  for (size_t i = 0; i < hostEltCount; i++) {
    xercesc::DOMNode* hostNode =  hostNodes->item(i);

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
        for(size_t attr = 0; attr < hostAttrCount; attr++) {
          xercesc::DOMNode* hostAttrNode = hostAttrList->item(attr);

          if(hostAttrNode->getNodeType()
             && hostAttrNode->getNodeType() == xercesc::DOMNode::ELEMENT_NODE) {

            xercesc::DOMElement* hostAttrElt = dynamic_cast<xercesc::DOMElement*>(hostAttrNode);
            std::string name = xercesc::XMLString::transcode(hostAttrElt->getNodeName());
            std::string text =  xercesc::XMLString::transcode(hostAttrElt->getTextContent());

            if (name == "MEM_USAGE") {
              host.vmMemoryUsage = boost::lexical_cast<size_t>(text);
            } else if (name == "CPU_USAGE") {
              host.vmCpuUsage = boost::lexical_cast<size_t>(text);
            } else if (name == "MAX_MEM") {
              host.memory = boost::lexical_cast<size_t>(text);
            } else if (name == "MAX_CPU") {
              host.cpu = boost::lexical_cast<size_t>(text);
            } else if(name == "FREE_MEM") {
              host.freeMemory = boost::lexical_cast<size_t>(text);
            } else if(name == "FREE_CPU"){
              host.freeCpu = boost::lexical_cast<size_t>(text);
            } else if(name == "RUNNING_VMS") {
              host.runningVms = atoi(text.c_str());
            }
          }
        }
      }  // HOST_SHARE
    }
  }
}

