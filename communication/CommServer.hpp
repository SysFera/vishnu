/**
 * \file CommServer.hpp
 * This file defines some utilitaries functions that use both the communication layer, the database layer, the core layer
 * \author Kevin Coulomv (kevin.coulomb@sysfera.com)
 */

#ifndef __COMMSERVER__HH__
#define __COMMSERVER__HH__

#include "ExecConfiguration.hpp"

int
unregisterSeD(std::string type, ExecConfiguration config);

void
validateUri(const std::string & uri);

int
registerSeD(std::string type, ExecConfiguration config, std::string& cfg, std::vector<std::string>& services);


#endif
