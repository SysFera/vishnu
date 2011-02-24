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
  vishnuid BINARY_FLOAT   NOT NULL ,
  updatefreq BINARY_FLOAT    ,
  formatiduser VARCHAR2(255)    ,
  formatidjob VARCHAR2(255)    ,
  formatidfiletransfer VARCHAR2(255)    ,
  formatidmachine VARCHAR2(255)    ,
  usercpt BINARY_FLOAT    ,
  jobcpt BINARY_FLOAT    ,
  fileSubcpt BINARY_FLOAT    ,
  machinecpt BINARY_FLOAT      ,
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
  numuserid BINARY_FLOAT   NOT NULL ,
  vishnu_vishnuid BINARY_FLOAT   NOT NULL ,
  userid VARCHAR2(255)    ,
  pwd VARCHAR2(255)    ,
  firstname VARCHAR2(255)    ,
  lastname VARCHAR2(255)    ,
  privilege BINARY_FLOAT    ,
  email VARCHAR2(255)    ,
  passwordstate BINARY_FLOAT      ,
  status BINARY_FLOAT      ,
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
  numoptionid BINARY_FLOAT   NOT NULL ,
  optionid BINARY_FLOAT    ,
  description VARCHAR2(255)    ,
  defaultvalue BINARY_FLOAT      ,
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
  numclmachineid BINARY_FLOAT   NOT NULL ,
  sshkey VARCHAR2(255)    ,
  name VARCHAR2(255)      ,
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
  nummachineid BINARY_FLOAT   NOT NULL ,
  vishnu_vishnuid BINARY_FLOAT   NOT NULL ,
  name VARCHAR2(255)    ,
  site VARCHAR2(255)    ,
  diskspace BINARY_FLOAT    ,
  memory BINARY_FLOAT    ,
  network BINARY_FLOAT    ,
  machineid VARCHAR2(255)      ,
  status BINARY_FLOAT      ,
  sshpublickey VARCHAR2(1000)   ,
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
  numstateid BINARY_FLOAT   NOT NULL ,
  machine_nummachineid BINARY_FLOAT   NOT NULL ,
  numberofjob BINARY_FLOAT    ,
  memory BINARY_FLOAT    ,
  diskspace BINARY_FLOAT    ,
  cpuload BINARY_FLOAT    ,
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
  cpuid BINARY_FLOAT   NOT NULL ,
  machine_nummachineid BINARY_FLOAT   NOT NULL ,
  model VARCHAR2(255)    ,
  frequency BINARY_FLOAT    ,
  core BINARY_FLOAT    ,
  cache BINARY_FLOAT      ,
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
  numdescriptionid BINARY_FLOAT   NOT NULL ,
  machine_nummachineid BINARY_FLOAT   NOT NULL ,
  lang VARCHAR2(255)    ,
  description VARCHAR2(255)      ,
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
  numsessionid BINARY_FLOAT   NOT NULL ,
  clmachine_numclmachineid BINARY_FLOAT   NOT NULL ,
  users_numuserid BINARY_FLOAT   NOT NULL ,
  vsessionid VARCHAR2(255)    ,
  lastconnect TIMESTAMP    ,
  creation TIMESTAMP    ,
  closure TIMESTAMP    ,
  sessionkey VARCHAR2(255)    ,
  state BINARY_FLOAT    ,
  closepolicy BINARY_FLOAT    ,
  timeout BINARY_FLOAT      ,
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
  numaccountid BINARY_FLOAT   NOT NULL ,
  machine_nummachineid BINARY_FLOAT   NOT NULL ,
  users_numuserid BINARY_FLOAT   NOT NULL ,
  aclogin VARCHAR2(255)    ,
  sshpathkey VARCHAR2(255)    ,
  home VARCHAR2(255)      ,
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
  numoptionvalueid BINARY_FLOAT   NOT NULL ,
  users_numuserid BINARY_FLOAT   NOT NULL ,
  optionu_numoptionid BINARY_FLOAT   NOT NULL ,
  value BINARY_FLOAT      ,
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
  thresholdid BINARY_FLOAT   NOT NULL ,
  users_numuserid BINARY_FLOAT   NOT NULL ,
  machine_nummachineid BINARY_FLOAT   NOT NULL ,
  typet BINARY_FLOAT    ,
  value BINARY_FLOAT      ,
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
  numcommandid BINARY_FLOAT   NOT NULL ,
  vsession_numsessionid BINARY_FLOAT   NOT NULL ,
  starttime TIMESTAMP    ,
  endtime TIMESTAMP    ,
  description VARCHAR2(1000)    ,
  ctype BINARY_FLOAT      ,
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
  numfiletransferid BINARY_FLOAT   NOT NULL ,
  command_numcommandid BINARY_FLOAT   NOT NULL ,
  filetransferid VARCHAR2(255)    ,
  statust BINARY_FLOAT    ,
  source VARCHAR2(255)    ,
  destination VARCHAR2(255)    ,
  client VARCHAR2(255)    ,
  filepath VARCHAR2(255)    ,
  destinationpath VARCHAR2(255)    ,
  globaltransferid BINARY_FLOAT      ,
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
  numfileid BINARY_FLOAT   NOT NULL ,
  command_numcommandid BINARY_FLOAT   NOT NULL ,
  fileid VARCHAR2(255)    ,
  name VARCHAR2(255)    ,
  content VARCHAR2(255)      ,
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
  numjobid BINARY_FLOAT   NOT NULL ,
  command_numcommandid BINARY_FLOAT   NOT NULL ,
  jobid VARCHAR2(255)    ,
  state BINARY_FLOAT    ,
  path VARCHAR2(255)    ,
  outputpath VARCHAR2(255)    ,
  errorpath VARCHAR2(255)    ,
  submitdir VARCHAR2(255)      ,
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

-- END OF THE SCRIPT

