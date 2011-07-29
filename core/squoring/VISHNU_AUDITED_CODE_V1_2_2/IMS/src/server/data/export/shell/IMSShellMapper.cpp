/**
 * \file IMSMapper.ccp
 * \brief Implementation of the IMS mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#include "IMSShellMapper.hpp"
#include "IMS_Data.hpp"
#include "IMS_Data_forward.hpp"
#include "UserServer.hpp"
#include "SessionServer.hpp"
#include "MachineServer.hpp"
#include "LocalAccountServer.hpp"
#include "ConfigurationServer.hpp"
#include "QueryServer.hpp"
#include "CommandServer.hpp"
#include "utilServer.hpp"

using namespace vishnu;

IMSShellMapper::IMSShellMapper(){
};

IMSShellMapper::~IMSShellMapper(){
};

IMSShellMapper::IMSShellMapper(MapperRegistry* reg, string na):IMSMapper(reg, na) {
}
