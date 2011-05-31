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

GRANT SELECT, INSERT, UPDATE, DELETE ON fileTransfer TO "vishnu_db_admin";
