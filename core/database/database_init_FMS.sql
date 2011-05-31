-- This script is for initialization of the VISHNU PostgreSQL database
-- Script name          : database_init_FMS.sql
-- Script owner         : SysFera SAS

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 27/01/11
-- Revision author      : Benjamin ISNARD <benjamin.isnard@sysfera.com>,
--                        Daouda TRAORE <daouda.traore@sysfera.com>,
--                        Ibrahima Cisse <ibrahima.cisse@sysfera.com>
-- Revision comment     : The purpose of this script is to update the database for old script version used for UMS

ALTER TABLE command ADD COLUMN status integer;
ALTER TABLE command ADD COLUMN vishnuobjectid VARCHAR(255);

DROP TABLE fileTransfer;

CREATE TABLE fileTransfer (
  numfileTransferid SERIAL  NOT NULL ,
  vsession_numsessionid INTEGER   NOT NULL ,
  transferId VARCHAR(255),
  status INTEGER,
  userId VARCHAR(255),
  clientMachineId VARCHAR(255),
  sourceMachineId VARCHAR(255),
  destinationMachineId VARCHAR(255),
  sourceFilePath VARCHAR(255),
  destinationFilePath VARCHAR(255),
  fileSize INTEGER,
  startTime TIMESTAMP,
  trCommand INTEGER,
  processId INTEGER,
PRIMARY KEY(numfileTransferid),
    FOREIGN KEY(vsession_numsessionid)
    REFERENCES vsession(numsessionid) ON DELETE CASCADE);

GRANT ALL ON SEQUENCE fileTransfer_numfileTransferid_seq TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON fileTransfer TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON fileTransfer TO "vishnu_user";
