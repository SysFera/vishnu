-- This script is for initialization of the VISHNU Oracle 11g database
-- Script name          : oracle_create.sql
-- Script owner         : SysFera SAS

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 27/01/11
-- Revision author      : Benjamin Isnard <benjamin.isnard@sysfera.com>
-- Revision comment     : initial version

-- Table for Vishnu system

CREATE TABLE vishnu (
  vishnuid INTEGER   NOT NULL ,
  updatefreq INTEGER    ,
  formatiduser VARCHAR(255)    ,
  formatidjob VARCHAR(255)    ,
  formatidfiletransfer VARCHAR(255)    ,
  formatidmachine VARCHAR(255)    ,
  usercpt INTEGER    ,
  jobcpt INTEGER    ,
  fileSubcpt INTEGER    ,
  machinecpt INTEGER      ,
PRIMARY KEY(vishnuid));

CREATE SEQUENCE s_vishnu;

CREATE OR REPLACE TRIGGER AINC_vishnu
BEFORE INSERT  ON vishnu
FOR EACH ROW 
BEGIN 
  IF (:NEW.vishnuid IS NULL) THEN 
    SELECT s_vishnu.NEXTVAL INTO :NEW.vishnuid FROM DUAL; 
  END IF; 
END; 

/

-- Table for Users

CREATE TABLE users (
  numuserid INTEGER   NOT NULL ,
  vishnu_vishnuid INTEGER   NOT NULL ,
  userid VARCHAR(255)    ,
  pwd VARCHAR(255)    ,
  firstname VARCHAR(255)    ,
  lastname VARCHAR(255)    ,
  privilege INTEGER    ,
  email VARCHAR(255)    ,
  passwordstate INTEGER      ,
  status INTEGER      ,
PRIMARY KEY(numuserid),
  FOREIGN KEY(vishnu_vishnuid)
    REFERENCES vishnu(vishnuid));

CREATE SEQUENCE s_users;

CREATE OR REPLACE TRIGGER AINC_users
BEFORE INSERT  ON users
FOR EACH ROW 
BEGIN 
  IF (:NEW.numuserid IS NULL) THEN 
    SELECT s_users.NEXTVAL INTO :NEW.numuserid FROM DUAL; 
  END IF; 
END;

/

-- Table for User options

CREATE TABLE optionu (
  numoptionid INTEGER   NOT NULL ,
  optionid INTEGER    ,
  description VARCHAR(255)    ,
  defaultvalue INTEGER      ,
PRIMARY KEY(numoptionid));

CREATE SEQUENCE s_optionu;

CREATE OR REPLACE TRIGGER AINC_optionu
BEFORE INSERT  ON optionu
FOR EACH ROW 
BEGIN 
  IF (:NEW.numoptionid IS NULL) THEN 
    SELECT s_optionu.NEXTVAL INTO :NEW.numoptionid FROM DUAL; 
  END IF; 
END; 

/

-- Table for Client machines

CREATE TABLE clmachine (
  numclmachineid INTEGER   NOT NULL ,
  sshkey VARCHAR(255)    ,
  name VARCHAR(255)      ,
PRIMARY KEY(numclmachineid));

CREATE SEQUENCE s_clmachine;

CREATE OR REPLACE TRIGGER AINC_clmachine
BEFORE INSERT  ON clmachine
FOR EACH ROW 
BEGIN 
  IF (:NEW.numclmachineid IS NULL) THEN 
    SELECT s_clmachine.NEXTVAL INTO :NEW.numclmachineid FROM DUAL; 
  END IF; 
END; 

/

-- Table for Machines (servers)

CREATE TABLE machine (
  nummachineid INTEGER   NOT NULL ,
  vishnu_vishnuid INTEGER   NOT NULL ,
  name VARCHAR(255)    ,
  site VARCHAR(255)    ,
  diskspace INTEGER    ,
  memory INTEGER    ,
  network INTEGER    ,
  machineid VARCHAR(255)      ,
  status INTEGER      ,
  sshpublickey VARCHAR2   ,
PRIMARY KEY(nummachineid),
  FOREIGN KEY(vishnu_vishnuid)
    REFERENCES vishnu(vishnuid));

CREATE SEQUENCE s_machine;

CREATE OR REPLACE TRIGGER AINC_machine
BEFORE INSERT  ON machine
FOR EACH ROW 
BEGIN 
  IF (:NEW.nummachineid IS NULL) THEN 
    SELECT s_machine.NEXTVAL INTO :NEW.nummachineid FROM DUAL; 
  END IF; 
END; 

/

-- Table for Machine states

CREATE TABLE state (
  numstateid INTEGER   NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  numberofjob INTEGER    ,
  memory INTEGER    ,
  diskspace INTEGER    ,
  cpuload INTEGER    ,
  time TIMESTAMP      ,
PRIMARY KEY(numstateid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid) ON DELETE CASCADE);

CREATE SEQUENCE s_state;

CREATE OR REPLACE TRIGGER AINC_state
BEFORE INSERT  ON state
FOR EACH ROW 
BEGIN 
  IF (:NEW.numstateid IS NULL) THEN 
    SELECT s_state.NEXTVAL INTO :NEW.numstateid FROM DUAL; 
  END IF; 
END; 

/

-- Table for Machine CPU Information

CREATE TABLE cpu (
  cpuid INTEGER   NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  model VARCHAR(255)    ,
  frequency INTEGER    ,
  core INTEGER    ,
  cache INTEGER      ,
PRIMARY KEY(cpuid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid) ON DELETE CASCADE);

CREATE SEQUENCE s_cpu;

CREATE OR REPLACE TRIGGER AINC_cpu
BEFORE INSERT  ON cpu
FOR EACH ROW 
BEGIN 
  IF (:NEW.cpuid IS NULL) THEN 
    SELECT s_cpu.NEXTVAL INTO :NEW.cpuid FROM DUAL; 
  END IF; 
END; 

/

-- Table for Machine descriptions

CREATE TABLE description (
  numdescriptionid INTEGER   NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  lang VARCHAR(255)    ,
  description VARCHAR(255)      ,
PRIMARY KEY(numdescriptionid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid) ON DELETE CASCADE);

CREATE SEQUENCE s_description;

CREATE OR REPLACE TRIGGER AINC_description
BEFORE INSERT  ON description
FOR EACH ROW 
BEGIN 
  IF (:NEW.numdescriptionid IS NULL) THEN 
    SELECT s_description.NEXTVAL INTO :NEW.numdescriptionid FROM DUAL; 
  END IF; 
END; 

/

-- Table for Vishnu Sessions

CREATE TABLE vsession (
  numsessionid INTEGER   NOT NULL ,
  clmachine_numclmachineid INTEGER   NOT NULL ,
  users_numuserid INTEGER   NOT NULL ,
  vsessionid VARCHAR(255)    ,
  lastconnect TIMESTAMP    ,
  creation TIMESTAMP    ,
  closure TIMESTAMP    ,
  sessionkey VARCHAR(255)    ,
  state INTEGER    ,
  closepolicy INTEGER    ,
  timeout INTEGER      ,
PRIMARY KEY(numsessionid),
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid) ON DELETE CASCADE,
  FOREIGN KEY(clmachine_numclmachineid)
    REFERENCES clmachine(numclmachineid));

CREATE SEQUENCE s_vsession;

CREATE OR REPLACE TRIGGER AINC_vsession
BEFORE INSERT  ON vsession
FOR EACH ROW 
BEGIN 
  IF (:NEW.numsessionid IS NULL) THEN 
    SELECT s_vsession.NEXTVAL INTO :NEW.numsessionid FROM DUAL; 
  END IF; 
END; 

/

-- Table for User Local accounts

CREATE TABLE account (
  numaccountid INTEGER   NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  users_numuserid INTEGER   NOT NULL ,
  aclogin VARCHAR(255)    ,
  sshpathkey VARCHAR(255)    ,
  home VARCHAR(255)      ,
PRIMARY KEY(numaccountid),
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid) ON DELETE CASCADE,
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid) ON DELETE CASCADE);

CREATE SEQUENCE s_account;

CREATE OR REPLACE TRIGGER AINC_account
BEFORE INSERT  ON account
FOR EACH ROW 
BEGIN 
  IF (:NEW.numaccountid IS NULL) THEN 
    SELECT s_account.NEXTVAL INTO :NEW.numaccountid FROM DUAL; 
  END IF; 
END; 

/

-- Table for User Option values

CREATE TABLE optionvalue (
  numoptionvalueid INTEGER   NOT NULL ,
  users_numuserid INTEGER   NOT NULL ,
  optionu_numoptionid INTEGER   NOT NULL ,
  value INTEGER      ,
PRIMARY KEY(numoptionvalueid),
  FOREIGN KEY(optionu_numoptionid)
    REFERENCES optionu(numoptionid),
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid) ON DELETE CASCADE);

CREATE SEQUENCE s_optionvalue;

CREATE OR REPLACE TRIGGER AINC_optionvalue
BEFORE INSERT  ON optionvalue
FOR EACH ROW 
BEGIN 
  IF (:NEW.numoptionvalueid IS NULL) THEN 
    SELECT s_optionvalue.NEXTVAL INTO :NEW.numoptionvalueid FROM DUAL; 
  END IF; 
END; 

/

-- Table for Machine Tresholds

CREATE TABLE threshold (
  thresholdid INTEGER   NOT NULL ,
  users_numuserid INTEGER   NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  typet INTEGER    ,
  value INTEGER      ,
PRIMARY KEY(thresholdid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid) ON DELETE CASCADE,
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid));

CREATE SEQUENCE s_threshold;

CREATE OR REPLACE TRIGGER AINC_threshold
BEFORE INSERT  ON threshold
FOR EACH ROW 
BEGIN 
  IF (:NEW.thresholdid IS NULL) THEN 
    SELECT s_threshold.NEXTVAL INTO :NEW.thresholdid FROM DUAL; 
  END IF; 
END; 

/

-- Table for Commands (Vishnu log)

CREATE TABLE command (
  numcommandid INTEGER   NOT NULL ,
  vsession_numsessionid INTEGER   NOT NULL ,
  starttype TIMESTAMP    ,
  endtime TIMESTAMP    ,
  description VARCHAR(255)    ,
  ctype INTEGER      ,
PRIMARY KEY(numcommandid),
  FOREIGN KEY(vsession_numsessionid)
    REFERENCES vsession(numsessionid) ON DELETE CASCADE);

CREATE SEQUENCE s_command;

CREATE OR REPLACE TRIGGER AINC_command
BEFORE INSERT  ON command
FOR EACH ROW 
BEGIN 
  IF (:NEW.numcommandid IS NULL) THEN 
    SELECT s_command.NEXTVAL INTO :NEW.numcommandid FROM DUAL; 
  END IF; 
END; 

/

-- Table for File transfers

CREATE TABLE filetransfer (
  numfiletransferid INTEGER   NOT NULL ,
  command_numcommandid INTEGER   NOT NULL ,
  filetransferid VARCHAR(255)    ,
  statust INTEGER    ,
  source VARCHAR(255)    ,
  destination VARCHAR(255)    ,
  client VARCHAR(255)    ,
  filepath VARCHAR(255)    ,
  destinationpath VARCHAR(255)    ,
  globaltransferid INTEGER      ,
PRIMARY KEY(numfiletransferid),
  FOREIGN KEY(command_numcommandid)
    REFERENCES command(numcommandid));

CREATE SEQUENCE s_filetransfer;

CREATE OR REPLACE TRIGGER AINC_filetransfer
BEFORE INSERT  ON filetransfer
FOR EACH ROW 
BEGIN 
  IF (:NEW.numfiletransferid IS NULL) THEN 
    SELECT s_filetransfer.NEXTVAL INTO :NEW.numfiletransferid FROM DUAL; 
  END IF; 
END; 

/

-- Table for submitted files

CREATE TABLE filesub (
  numfileid INTEGER   NOT NULL ,
  command_numcommandid INTEGER   NOT NULL ,
  fileid VARCHAR(255)    ,
  name VARCHAR(255)    ,
  content VARCHAR(255)      ,
PRIMARY KEY(numfileid),
  FOREIGN KEY(command_numcommandid)
    REFERENCES command(numcommandid));

CREATE SEQUENCE s_filesub;

CREATE OR REPLACE TRIGGER AINC_filesub
BEFORE INSERT  ON filesub
FOR EACH ROW 
BEGIN 
  IF (:NEW.numfileid IS NULL) THEN 
    SELECT s_filesub.NEXTVAL INTO :NEW.numfileid FROM DUAL; 
  END IF; 
END;

/

-- Table for jobs

CREATE TABLE job (
  numjobid INTEGER   NOT NULL ,
  command_numcommandid INTEGER   NOT NULL ,
  jobid VARCHAR(255)    ,
  state INTEGER    ,
  path VARCHAR(255)    ,
  outputpath VARCHAR(255)    ,
  errorpath VARCHAR(255)    ,
  submitdir VARCHAR(255)      ,
PRIMARY KEY(numjobid),
  FOREIGN KEY(command_numcommandid)
    REFERENCES command(numcommandid));

CREATE SEQUENCE s_job;

CREATE OR REPLACE TRIGGER AINC_job
BEFORE INSERT  ON job
FOR EACH ROW 
BEGIN 
  IF (:NEW.numjobid IS NULL) THEN 
    SELECT s_job.NEXTVAL INTO :NEW.numjobid FROM DUAL; 
  END IF; 
END; 

/

-- CREATE Role Vishnu Database Administrator

CREATE ROLE vishnu_db_admin;

GRANT CONNECT, RESOURCE TO vishnu_db_admin;
GRANT SELECT, INSERT, UPDATE, DELETE ON vishnu TO vishnu_db_admin;

-- CREATE Role Vishnu Standard User

CREATE ROLE vishnu_user;

GRANT CONNECT TO vishnu_user;
GRANT SELECT ON vishnu TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON users TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON machine TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON clmachine TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON optionu TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON state TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON cpu TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON description TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON vsession TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON account TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON optionvalue TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON threshold TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON command TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON filetransfer TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON filesub TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON job TO vishnu_user;

-- END OF THE SCRIPT

