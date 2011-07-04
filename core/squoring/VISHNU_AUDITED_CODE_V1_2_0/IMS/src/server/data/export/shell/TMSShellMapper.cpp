/**
 * \file TMSMapper.ccp
 * \brief Implementation of the TMS mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#include "TMSShellMapper.hpp"
#include "TMS_Data.hpp"
#include "TMS_Data_forward.hpp"
#include "UserServer.hpp"
#include "SessionServer.hpp"
#include "MachineServer.hpp"
#include "LocalAccountServer.hpp"
#include "ConfigurationServer.hpp"
#include "QueryServer.hpp"
#include "CommandServer.hpp"
#include "utilServer.hpp"

using namespace vishnu;

TMSShellMapper::TMSShellMapper(){
};

TMSShellMapper::~TMSShellMapper(){
};

TMSShellMapper::TMSShellMapper(MapperRegistry* reg, string na):TMSMapper(reg, na) {
}
