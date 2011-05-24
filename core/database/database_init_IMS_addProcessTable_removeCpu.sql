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

<<<<<<< HEAD
=======
/* Keep the drop state before create table state */
drop table state;


CREATE TABLE state (
  numstateid SERIAL  NOT NULL ,
  machine_nummachineid INTEGER   NOT NULL ,
  memory REAL    ,
  diskspace REAL    ,
  cpuload REAL    ,
  time TIMESTAMP      ,
PRIMARY KEY(numstateid),
  FOREIGN KEY(machine_nummachineid)
    REFERENCES machine(nummachineid) ON DELETE CASCADE);

GRANT SELECT, INSERT, UPDATE, DELETE ON state TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON state TO "vishnu_user";
GRANT ALL ON SEQUENCE state_numstateid_seq TO vishnu_user;

>>>>>>> 4 services more or less implemented for metric server

>>>>>>> e5171d9... Fix conflict
drop table cpu;
ALTER TABLE state DROP COLUMN numberofjob;
