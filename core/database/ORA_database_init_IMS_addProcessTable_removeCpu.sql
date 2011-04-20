
-- Table for processes

CREATE TABLE process (
  numprocess NIBARY_FLOAT NOT NULL,
  pstatus INTEGER,
  vishnuname VARCHAR(255),
  dietname VARCHAR(255),
  machineid VARCHAR(255),	
  uptime TIMESTAMP,
  launchscript VARCHAR2(1000),
  PRIMARY KEY(numprocess)
);

CREATE SEQUENCE s_process;
CREATE OR REPLACE TRIGGER AINC_process
BEFORE INSERT ON process
FOR EACH ROW
BEGIN
  IF (:NEW.numprocess IS NULL) THEN
    SELECT s_process.NEXTVAL INTO :NEW.numprocess FROM DUAL;
  END IF;
END;

/

drop table cpu;
