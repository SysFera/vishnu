-- This script is for initialization of the VISHNU PostgreSQL database
-- Script name          : database_init_TMS.sql
-- Script owner         : SysFera SAS

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 27/01/11
-- Revision author      : Benjamin ISNARD <benjamin.isnard@sysfera.com>, 
--                        Daouda TRAORE <daouda.traore@sysfera.com>,
--                        Eugène PAMBA CAPO-CHICHI <eugene.capochichi@sysfera.com>
-- Revision comment     : The purpose of this script is to update the database for old script version used for UMS

ALTER TABLE command ADD COLUMN status integer;
ALTER TABLE command ADD COLUMN vishnuobjectid VARCHAR(255);