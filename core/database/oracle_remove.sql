-- This script is for suppression of the VISHNU Oracle 11g database
-- Script name          : oracle_remove.sql
-- Script owner         : SysFera SAS

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 27/01/11
-- Revision author      : Benjamin Isnard <benjamin.isnard@sysfera.com>
-- Revision comment     : initial version

-- Drop sequences

DROP SEQUENCE s_vishnu;
DROP SEQUENCE s_users;
DROP SEQUENCE s_optionu;
DROP SEQUENCE s_clmachine;
DROP SEQUENCE s_machine;
DROP SEQUENCE s_state;
DROP SEQUENCE s_cpu;
DROP SEQUENCE s_description;
DROP SEQUENCE s_vsession;
DROP SEQUENCE s_account;
DROP SEQUENCE s_optionvalue;
DROP SEQUENCE s_threshold;
DROP SEQUENCE s_command;
DROP SEQUENCE s_filetransfer;
DROP SEQUENCE s_filesub;
DROP SEQUENCE s_job;

-- Drop tables

DROP TABLE job;
DROP TABLE filesub;
DROP TABLE filetransfer;
DROP TABLE command;
DROP TABLE threshold;
DROP TABLE optionvalue;
DROP TABLE account;
DROP TABLE vsession;
DROP TABLE description;
DROP TABLE cpu;
DROP TABLE state;
DROP TABLE users;
DROP TABLE machine;
DROP TABLE vishnu;
DROP TABLE clmachine;
DROP TABLE optionu;

-- END OF THE SCRIPT
