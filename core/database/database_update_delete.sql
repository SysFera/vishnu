-- This script is for update of the VISHNU database content
-- Script name          : database_update_delete.sql
-- Script owner         : SysFera SA

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 19/04/13
-- Revision author      : Kevin Coulomb <kevin.coulomb@sysfera.com>
-- Revision comment     : initial version for upgrade from release 3.0.beta2 to release 3.0

alter table account add status integer default NULL;
alter table authaccount add status integer default NULL;
alter table account drop column sshpathkey;
alter table machine drop column sshpublickey;