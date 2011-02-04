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
PRIMARY KEY(numuserid),
  FOREIGN KEY(vishnu_vishnuid)
    REFERENCES vishnu(vishnuid));



CREATE TABLE state (
  numstateid SERIAL  NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  numberofjob INTEGER    ,
  memory INTEGER    ,
  diskspace INTEGER    ,
  cpuload INTEGER    ,
  time TIMESTAMP      ,
PRIMARY KEY(numstateid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid));



CREATE TABLE cpu (
  cpuid SERIAL  NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  model VARCHAR(255)    ,
  frequency INTEGER    ,
  core INTEGER    ,
  cache INTEGER      ,
PRIMARY KEY(cpuid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid));



CREATE TABLE description (
  numdescriptionid SERIAL  NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  lang VARCHAR(255)    ,
  description VARCHAR(255)      ,
PRIMARY KEY(numdescriptionid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid));



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
PRIMARY KEY(numsessionid),
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid),
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
    REFERENCES users(numuserid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid));



CREATE TABLE optionvalue (
  numoptionvalueid SERIAL  NOT NULL ,
  users_numuserid INTEGER   NOT NULL ,
  optionu_numoptionid INTEGER   NOT NULL ,
  value INTEGER      ,
PRIMARY KEY(numoptionvalueid),
  FOREIGN KEY(optionu_numoptionid)
    REFERENCES optionu(numoptionid),
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid));



CREATE TABLE threshold (
  thresholdid SERIAL  NOT NULL ,
  users_numuserid INTEGER   NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  typet INTEGER    ,
  value INTEGER      ,
PRIMARY KEY(thresholdid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid),
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid));



CREATE TABLE command (
  numcommandid SERIAL  NOT NULL ,
  vsession_numsessionid INTEGER   NOT NULL ,
  starttype TIMESTAMP    ,
  endtime TIMESTAMP    ,
  description VARCHAR(255)    ,
  ctype INTEGER      ,
PRIMARY KEY(numcommandid),
  FOREIGN KEY(vsession_numsessionid)
    REFERENCES vsession(numsessionid));



CREATE TABLE filetransfer (
  numfiletransferid SERIAL  NOT NULL ,
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




