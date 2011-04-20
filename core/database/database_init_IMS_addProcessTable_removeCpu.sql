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

drop table cpu;
