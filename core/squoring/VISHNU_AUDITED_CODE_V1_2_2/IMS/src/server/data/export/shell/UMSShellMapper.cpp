/**
 * \file UMSMapper.ccp
 * \brief Implementation of the UMS mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#include "UMSShellMapper.hpp"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
#include "UserServer.hpp"
#include "SessionServer.hpp"
#include "MachineServer.hpp"
#include "LocalAccountServer.hpp"
#include "ConfigurationServer.hpp"
#include "QueryServer.hpp"
#include "CommandServer.hpp"
#include "utilServer.hpp"

using namespace vishnu;

UMSShellMapper::UMSShellMapper(){
};

UMSShellMapper::~UMSShellMapper(){
};

UMSShellMapper::UMSShellMapper(MapperRegistry* reg, string na):UMSMapper(reg, na) {
}
