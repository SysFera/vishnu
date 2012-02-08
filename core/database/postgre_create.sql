-- This script is for initialization of the VISHNU PostgreSQL database
-- Script name          : postgre_create.sql
-- Script owner         : SysFera SAS

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 27/01/11
-- Revision author      : Benjamin Isnard <benjamin.isnard@sysfera.com>
-- Revision comment     : initial version

-- Revision nb          : 1.2
-- Revision date        : 07/06/11
-- Revision author      : Benjamin Isnard <benjamin.isnard@sysfera.com>
-- Revision comment     : release of FMS and IMS modules

-- Revision nb          : 1.2.8
-- Revision date        : 02/02/12
-- Revision author      : Kevin Coulomb <kevin.coulomb@sysfera.com>
-- Revision comment     : Add auth System

CREATE TABLE optionu (
  numoptionid SERIAL  NOT NULL ,
  optionid INTEGER    ,
  description VARCHAR(255)    ,
  defaultvalue INTEGER      ,
PRIMARY KEY(numoptionid));

CREATE TABLE clmachine (
  numclmachineid SERIAL  NOT NULL ,
  sshkey VARCHAR(255)    ,
  name VARCHAR(255)      ,
PRIMARY KEY(numclmachineid));



CREATE TABLE vishnu (
  vishnuid SERIAL  NOT NULL ,
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




CREATE TABLE machine (
  nummachineid SERIAL  NOT NULL ,
  vishnu_vishnuid INTEGER   NOT NULL ,
  name VARCHAR(255)    ,
  site VARCHAR(255)    ,
  diskspace INTEGER    ,
  memory INTEGER    ,
  network INTEGER    ,
  machineid VARCHAR(255)      ,
  status INTEGER      ,
  sshpublickey TEXT ,
PRIMARY KEY(nummachineid),
  FOREIGN KEY(vishnu_vishnuid)
    REFERENCES vishnu(vishnuid));



CREATE TABLE users (
  numuserid SERIAL  NOT NULL ,
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



CREATE TABLE state (
  numstateid SERIAL  NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  memory integer    ,
  diskspace integer    ,
  cpuload integer    ,
  time TIMESTAMP      ,
PRIMARY KEY(numstateid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid) ON DELETE CASCADE);




CREATE TABLE description (
  numdescriptionid SERIAL  NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  lang VARCHAR(255)    ,
  description VARCHAR(255)      ,
PRIMARY KEY(numdescriptionid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid) ON DELETE CASCADE);



CREATE TABLE vsession (
  numsessionid SERIAL NOT NULL ,
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
  authId VARCHAR(255)      ,
PRIMARY KEY(numsessionid),
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid) ON DELETE CASCADE,
  FOREIGN KEY(clmachine_numclmachineid)
    REFERENCES clmachine(numclmachineid));



CREATE TABLE account (
  numaccountid SERIAL  NOT NULL ,
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



CREATE TABLE optionvalue (
  numoptionvalueid SERIAL  NOT NULL ,
  users_numuserid INTEGER   NOT NULL ,
  optionu_numoptionid INTEGER   NOT NULL ,
  value INTEGER      ,
PRIMARY KEY(numoptionvalueid),
  FOREIGN KEY(optionu_numoptionid)
    REFERENCES optionu(numoptionid),
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid) ON DELETE CASCADE);



CREATE TABLE threshold (
  thresholdid SERIAL  NOT NULL ,
  users_numuserid INTEGER   NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  typet INTEGER    ,
  value INTEGER      ,
PRIMARY KEY(thresholdid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid) ON DELETE CASCADE,
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid));



CREATE TABLE command (
  numcommandid SERIAL  NOT NULL ,
  vsession_numsessionid INTEGER   NOT NULL ,
  starttime TIMESTAMP    ,
  endtime TIMESTAMP    ,
  description TEXT    ,
  ctype INTEGER      ,
  status INTEGER     ,
  vishnuobjectid VARCHAR(255),
PRIMARY KEY(numcommandid),
  FOREIGN KEY(vsession_numsessionid)
    REFERENCES vsession(numsessionid) ON DELETE CASCADE);

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


CREATE TABLE fileSub (
  numfileid SERIAL  NOT NULL ,
  command_numcommandid INTEGER   NOT NULL ,
  fileid VARCHAR(255)    ,
  name VARCHAR(255)    ,
  content VARCHAR(255)      ,
PRIMARY KEY(numfileid),
  FOREIGN KEY(command_numcommandid)
    REFERENCES command(numcommandid));

CREATE TABLE job (
  numjobid SERIAL  NOT NULL ,
  vsession_numsessionid INTEGER   NOT NULL ,
  submitMachineId VARCHAR(255),
  submitMachineName VARCHAR(255),
  jobId VARCHAR(255),
  batchJobId VARCHAR(255),
  batchType INTEGER,
  jobName VARCHAR(255),
  jobPath VARCHAR(255),
  outputPath VARCHAR(255),
  errorPath VARCHAR(255),
  scriptContent TEXT,
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

CREATE TABLE filetransfer (
  numfileTransferid SERIAL  NOT NULL ,
  vsession_numsessionid INTEGER   NOT NULL ,
  transferId VARCHAR(255),
  status INTEGER,
  userId VARCHAR(255),
  clientMachineName VARCHAR(255),
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


CREATE TABLE authsystem (
  numauthsystemid SERIAL  NOT NULL ,
  vishnu_vishnuid INTEGER   NOT NULL ,
  authsystemid VARCHAR(255)    ,
  name VARCHAR(255)    ,
  uri VARCHAR(255)    ,
  authlogin VARCHAR(255)    ,
  authpassword VARCHAR(255)    ,
  userpwdencryption INTEGER    ,
  authtype INTEGER    ,
  status INTEGER      ,
PRIMARY KEY(numauthsystemid)  ,
  FOREIGN KEY(vishnu_vishnuid)
    REFERENCES vishnu(vishnuid));

CREATE TABLE authaccount (
  authaccountid SERIAL  NOT NULL ,
  authsystem_authsystemid INTEGER   NOT NULL ,
  users_numuserid INTEGER   NOT NULL ,
  aclogin VARCHAR(255)      ,
PRIMARY KEY(authaccountid)    ,
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid) ON DELETE CASCADE,
  FOREIGN KEY(authsystem_authsystemid)
    REFERENCES authsystem(numauthsystemid) ON DELETE CASCADE);

CREATE TABLE ldapauthsystem (
  ldapauthsystid SERIAL  NOT NULL ,
  authsystem_authsystemid INTEGER   NOT NULL ,
  ldapbase VARCHAR(255)      ,
PRIMARY KEY(ldapauthsystid)  ,
  FOREIGN KEY(authsystem_authsystemid)
    REFERENCES authsystem(numauthsystemid) ON DELETE CASCADE);

-- Role Creation;

CREATE USER vishnu_user WITH PASSWORD 'vishnu_user';
CREATE USER vishnu_db_admin;

grant connect
on database vishnu
to vishnu_user;

grant connect
on database vishnu
to vishnu_db_admin;


GRANT ALL ON vishnu TO "vishnu_db_admin";
GRANT SELECT ON vishnu TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON users TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON machine TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON clmachine TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON optionu TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON state TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON description TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON vsession TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON account TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON optionvalue TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON threshold TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON command TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON filetransfer TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON filesub TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON job TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON process TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON authaccount TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON authsystem TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON ldapauthsystem TO "vishnu_db_admin";


--CREATE ROLE vishnu_user;

--Grant on right on table

GRANT SELECT, INSERT, UPDATE, DELETE ON vishnu TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON users TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON machine TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON clmachine TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON optionu TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON state TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON description TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON vsession TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON account TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON optionvalue TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON threshold TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON command TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON filetransfer TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON filesub TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON job TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON process TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON authaccount TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON authsystem TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON ldapauthsystem TO "vishnu_user";

--Grant on sequences

GRANT ALL ON SEQUENCE account_numaccountid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE clmachine_numclmachineid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE command_numcommandid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE description_numdescriptionid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE filesub_numfileid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE filetransfer_numfiletransferid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE job_numjobid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE machine_nummachineid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE optionu_numoptionid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE optionvalue_numoptionvalueid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE state_numstateid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE threshold_thresholdid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE users_numuserid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE vishnu_vishnuid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE vsession_numsessionid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE process_numprocess_seq TO vishnu_user;
GRANT ALL ON SEQUENCE authaccount TO "vishnu_user";
GRANT ALL ON SEQUENCE authsystem TO "vishnu_user";
GRANT ALL ON SEQUENCE ldapauthsystem TO "vishnu_user";


