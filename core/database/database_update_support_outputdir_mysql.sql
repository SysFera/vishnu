-- This script is for update of the structure of the Job table in the VISHNU database
-- Script name          : database_update_support_outputdir_mysql.sql
-- Script owner         : SysFera SA

-- REVISIONS
-- Revision nb          : 1.2.9
-- Revision date        : 03/05/12
-- Revision author      : Rodrigue Chakode <Rodrigue.Chakode@sysfera.com>
-- Revision comment     : initial version for upgrade from release 1.2.8 to 1.2.9


ALTER TABLE job ADD COLUMN outputDir VARCHAR(255) DEFAULT '';
