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