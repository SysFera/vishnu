-- This script is for update of the VISHNU database content
-- Script name          : database_update_addvminfo_mysql.sql
-- Script owner         : SysFera SA

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 10/12/12
-- Revision author      : Amine Bsila <amine.bsila@sysfera.com>
-- Revision comment     : initial version for upgrade from release 2.0.x beta to 3.0.beta2

alter table job add vmIp VARCHAR(255) default NULL;
alter table job add vmId  VARCHAR(255) default NULL;