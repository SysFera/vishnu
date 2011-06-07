-- This script is for upgrade of the VISHNU PostgreSQL database
-- Script name          : postgre_update.sql
-- Script owner         : SysFera SAS

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 07/06/11
-- Revision author      : Benjamin Isnard <benjamin.isnard@sysfera.com>
-- Revision comment     : upgrade from release 1.1 to release 1.2 of VISHNU DB

-- UPDATES FOR FMS module

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
  errorMsg TEXT,
PRIMARY KEY(numfileTransferid),
    FOREIGN KEY(vsession_numsessionid)
    REFERENCES vsession(numsessionid) ON DELETE CASCADE);

GRANT SELECT, INSERT, UPDATE, DELETE ON fileTransfer TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON fileTransfer TO "vishnu_user";
GRANT ALL ON SEQUENCE filetransfer_numfiletransferid_seq TO "vishnu_user";
GRANT ALL ON SEQUENCE filetransfer_numfiletransferid_seq TO "vishnu_db_admin";

-- UPDATES FOR IMS module

CREATE TABLE process (
  numprocess SERIAL NOT NULL,
  pstatus INTEGER,
  vishnuname VARCHAR(255),
  dietname VARCHAR(255),
  machineid VARCHAR(255),
  uptime TIMESTAMP,
  launchscript TEXT,
  PRIMARY KEY(numprocess)
);
GRANT SELECT, INSERT, UPDATE, DELETE ON process TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON process TO "vishnu_user";
GRANT ALL ON SEQUENCE process_numprocess_seq TO vishnu_user;

drop table cpu;
ALTER TABLE state DROP COLUMN numberofjob;

-- END
