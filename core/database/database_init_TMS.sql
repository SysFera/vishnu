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

DROP TABLE job;

CREATE TABLE job (
  numjobid SERIAL  NOT NULL ,
  vsession_numsessionid INTEGER   NOT NULL ,  
  submitMachineId VARCHAR(255),
  submitMachineName VARCHAR(255),
  jobId VARCHAR(255),
  jobName VARCHAR(255),
  jobPath VARCHAR(255),
  outputPath VARCHAR(255),
  errorPath VARCHAR(255),  
  scriptContent VARCHAR(255),
  jobPrio INTEGER,
  nbCpus INTEGER,
  jobWorkingDir VARCHAR(255),
  status INTEGER,
  submitDate TIMESTAMP,
  endDate TIMESTAMP,
  owner VARCHAR(255),
  jobQueue VARCHAR(255),
  wallClockLimit INTEGER,
  groupName VARCHAR(255),
  jobDescription VARCHAR(255),
  memLimit INTEGER,
  nbNodes INTEGER,
  nbNodesAndCpuPerNode VARCHAR(255),
PRIMARY KEY(numjobid),
    FOREIGN KEY(vsession_numsessionid)
    REFERENCES vsession(numsessionid) ON DELETE CASCADE);

GRANT ALL ON SEQUENCE job_numjobid_seq TO vishnu_user;
