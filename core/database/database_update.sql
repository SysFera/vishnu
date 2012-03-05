-- This script is for update of the VISHNU database content
-- Script name          : database_update.sql
-- Script owner         : SysFera SA

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 22/02/11
-- Revision author      : Benjamin Isnard <benjamin.isnard@sysfera.com>
-- Revision comment     : initial version for upgrade from release 1.1.0 to 1.2.0

-- Creates VISHNU record
UPDATE vishnu SET updatefreq=100 WHERE vishnuid='1';

-- Creates required user options
INSERT INTO optionu (optionid, description, defaultvalue) VALUES (3, 'VISHNU_TRANSFER_CMD', 0);
