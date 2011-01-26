CREATE SEQUENCE GlobalSequence;

CREATE TABLE optionu (
  numoptionid INTEGER   NOT NULL ,
  optionid INTEGER    ,
  description VARCHAR(255)    ,
  defaultvalue INTEGER      ,
PRIMARY KEY(numoptionid));


CREATE OR REPLACE TRIGGER AINC_optionu
BEFORE INSERT  ON optionu
FOR EACH ROW 
BEGIN 
  IF (:NEW.numoptionid IS NULL) THEN 
    SELECT GlobalSequence.NEXTVAL INTO :NEW.numoptionid FROM DUAL; 
  END IF; 
END; 

/



CREATE TABLE clmachine (
  numclmachineid INTEGER   NOT NULL ,
  sshkey VARCHAR(255)    ,
  name VARCHAR(255)      ,
PRIMARY KEY(numclmachineid));


CREATE OR REPLACE TRIGGER AINC_clmachine
BEFORE INSERT  ON clmachine
FOR EACH ROW 
BEGIN 
  IF (:NEW.numclmachineid IS NULL) THEN 
    SELECT GlobalSequence.NEXTVAL INTO :NEW.numclmachineid FROM DUAL; 
  END IF; 
END; 

/



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


CREATE OR REPLACE TRIGGER AINC_vishnu
BEFORE INSERT  ON vishnu
FOR EACH ROW 
BEGIN 
  IF (:NEW.vishnuid IS NULL) THEN 
    SELECT GlobalSequence.NEXTVAL INTO :NEW.vishnuid FROM DUAL; 
  END IF; 
END; 

/

COMMENT ON COLUMN vishnu.updatefreq IS 'update frequency';


CREATE TABLE machine (
  nummachineid INTEGER   NOT NULL ,
  vishnu_vishnuid INTEGER   NOT NULL ,
  name VARCHAR(255)    ,
  site VARCHAR(255)    ,
  diskspace INTEGER    ,
  memory INTEGER    ,
  network INTEGER    ,
  machineid VARCHAR(255)      ,
PRIMARY KEY(nummachineid),
  FOREIGN KEY(vishnu_vishnuid)
    REFERENCES vishnu(vishnuid));


CREATE OR REPLACE TRIGGER AINC_machine
BEFORE INSERT  ON machine
FOR EACH ROW 
BEGIN 
  IF (:NEW.nummachineid IS NULL) THEN 
    SELECT GlobalSequence.NEXTVAL INTO :NEW.nummachineid FROM DUAL; 
  END IF; 
END; 

/



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
PRIMARY KEY(numuserid),
  FOREIGN KEY(vishnu_vishnuid)
    REFERENCES vishnu(vishnuid));



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
    REFERENCES machine(nummachineid));


CREATE OR REPLACE TRIGGER AINC_state
BEFORE INSERT  ON state
FOR EACH ROW 
BEGIN 
  IF (:NEW.numstateid IS NULL) THEN 
    SELECT GlobalSequence.NEXTVAL INTO :NEW.numstateid FROM DUAL; 
  END IF; 
END; 

/



CREATE TABLE cpu (
  cpuid INTEGER   NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  model VARCHAR(255)    ,
  frequency INTEGER    ,
  core INTEGER    ,
  cache INTEGER      ,
PRIMARY KEY(cpuid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid));


CREATE OR REPLACE TRIGGER AINC_cpu
BEFORE INSERT  ON cpu
FOR EACH ROW 
BEGIN 
  IF (:NEW.cpuid IS NULL) THEN 
    SELECT GlobalSequence.NEXTVAL INTO :NEW.cpuid FROM DUAL; 
  END IF; 
END; 

/



CREATE TABLE description (
  numdescriptionid INTEGER   NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  lang VARCHAR(255)    ,
  description VARCHAR(255)      ,
PRIMARY KEY(numdescriptionid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid));


CREATE OR REPLACE TRIGGER AINC_description
BEFORE INSERT  ON description
FOR EACH ROW 
BEGIN 
  IF (:NEW.numdescriptionid IS NULL) THEN 
    SELECT GlobalSequence.NEXTVAL INTO :NEW.numdescriptionid FROM DUAL; 
  END IF; 
END; 

/



CREATE TABLE vsession (
  numsessionid INTEGER   NOT NULL ,
  clmachine_numclmachineid INTEGER   NOT NULL ,
  users_numuserid INTEGER   NOT NULL ,
  vsessionid INTEGER    ,
  lastconnect TIMESTAMP    ,
  creation TIMESTAMP    ,
  closure TIMESTAMP    ,
  sessionkey VARCHAR(255)    ,
  state INTEGER    ,
  closepolicy INTEGER    ,
  timeout INTEGER      ,
PRIMARY KEY(numsessionid),
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid),
  FOREIGN KEY(clmachine_numclmachineid)
    REFERENCES clmachine(numclmachineid));



CREATE TABLE account (
  numaccountid INTEGER   NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  users_numuserid INTEGER   NOT NULL ,
  aclogin VARCHAR(255)    ,
  sshpathkey VARCHAR(255)    ,
  home VARCHAR(255)      ,
PRIMARY KEY(numaccountid),
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid));


CREATE OR REPLACE TRIGGER AINC_account
BEFORE INSERT  ON account
FOR EACH ROW 
BEGIN 
  IF (:NEW.numaccountid IS NULL) THEN 
    SELECT GlobalSequence.NEXTVAL INTO :NEW.numaccountid FROM DUAL; 
  END IF; 
END; 

/



CREATE TABLE optionvalue (
  numoptionvalueid INTEGER   NOT NULL ,
  users_numuserid INTEGER   NOT NULL ,
  optionu_numoptionid INTEGER   NOT NULL ,
  value INTEGER      ,
PRIMARY KEY(numoptionvalueid),
  FOREIGN KEY(optionu_numoptionid)
    REFERENCES optionu(numoptionid),
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid));


CREATE OR REPLACE TRIGGER AINC_optionvalue
BEFORE INSERT  ON optionvalue
FOR EACH ROW 
BEGIN 
  IF (:NEW.numoptionvalueid IS NULL) THEN 
    SELECT GlobalSequence.NEXTVAL INTO :NEW.numoptionvalueid FROM DUAL; 
  END IF; 
END; 

/



CREATE TABLE threshold (
  thresholdid INTEGER   NOT NULL ,
  users_numuserid INTEGER   NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  typet INTEGER    ,
  value INTEGER      ,
PRIMARY KEY(thresholdid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid),
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid));


CREATE OR REPLACE TRIGGER AINC_threshold
BEFORE INSERT  ON threshold
FOR EACH ROW 
BEGIN 
  IF (:NEW.thresholdid IS NULL) THEN 
    SELECT GlobalSequence.NEXTVAL INTO :NEW.thresholdid FROM DUAL; 
  END IF; 
END; 

/



CREATE TABLE command (
  numcommandid INTEGER   NOT NULL ,
  vsession_numsessionid INTEGER   NOT NULL ,
  starttype TIMESTAMP    ,
  endtime TIMESTAMP    ,
  description VARCHAR(255)    ,
  ctype INTEGER      ,
PRIMARY KEY(numcommandid),
  FOREIGN KEY(vsession_numsessionid)
    REFERENCES vsession(numsessionid));


CREATE OR REPLACE TRIGGER AINC_command
BEFORE INSERT  ON command
FOR EACH ROW 
BEGIN 
  IF (:NEW.numcommandid IS NULL) THEN 
    SELECT GlobalSequence.NEXTVAL INTO :NEW.numcommandid FROM DUAL; 
  END IF; 
END; 

/



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


CREATE OR REPLACE TRIGGER AINC_filetransfer
BEFORE INSERT  ON filetransfer
FOR EACH ROW 
BEGIN 
  IF (:NEW.numfiletransferid IS NULL) THEN 
    SELECT GlobalSequence.NEXTVAL INTO :NEW.numfiletransferid FROM DUAL; 
  END IF; 
END; 

/

COMMENT ON COLUMN filetransfer.source IS 'id of the source machine';
COMMENT ON COLUMN filetransfer.destination IS 'id of the destination machine';
COMMENT ON COLUMN filetransfer.client IS 'id of the client';


CREATE TABLE fileSub (
  numfileid INTEGER   NOT NULL ,
  command_numcommandid INTEGER   NOT NULL ,
  fileid VARCHAR(255)    ,
  name VARCHAR(255)    ,
  content VARCHAR(255)      ,
PRIMARY KEY(numfileid),
  FOREIGN KEY(command_numcommandid)
    REFERENCES command(numcommandid));


CREATE OR REPLACE TRIGGER AINC_fileSub
BEFORE INSERT  ON fileSub
FOR EACH ROW 
BEGIN 
  IF (:NEW.numfileid IS NULL) THEN 
    SELECT GlobalSequence.NEXTVAL INTO :NEW.numfileid FROM DUAL; 
  END IF; 
END; 

/



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


CREATE OR REPLACE TRIGGER AINC_job
BEFORE INSERT  ON job
FOR EACH ROW 
BEGIN 
  IF (:NEW.numjobid IS NULL) THEN 
    SELECT GlobalSequence.NEXTVAL INTO :NEW.numjobid FROM DUAL; 
  END IF; 
END; 

/




