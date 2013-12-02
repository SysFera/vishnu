-- This script migrates the VISHNU database content
-- Script name          : database_init_migration_1.sql
-- Script owner         : SysFera SA

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 02/12/13
-- Revision author      : 
-- Revision comment     : initial version

-- Creates VISHNU record
INSERT INTO optionu (optionid, description, defaultvalue) VALUES (4, 'VISHNU_TRANSFER_TIMEOUT', 0);
