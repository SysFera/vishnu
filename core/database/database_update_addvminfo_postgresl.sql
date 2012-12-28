-- This script is for update of the VISHNU database content
-- Script name          : database_update_addvminfo_postgresl.sql
-- Script owner         : SysFera SA

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 10/12/12
-- Revision author      : Amine Bsila <amine.bsila@sysfera.com>
-- Revision comment     : initial version for upgrade from release 2.0.x to 2.1.x

alter table job add vmIp VARCHAR(255) default NULL;
alter table job add vmId  VARCHAR(255) default NULL;