
PROMPT 
PROMPT specify password for VISHNU as parameter 1:
DEFINE pass     = &1
PROMPT 
PROMPT specify default tablespeace for VISHNU as parameter 2:
DEFINE tbs      = &2
PROMPT 
PROMPT specify temporary tablespace for VISHNU as parameter 3:
DEFINE ttbs     = &3
PROMPT 

REM =======================================================
REM cleanup section
REM =======================================================

DROP USER vishnu CASCADE;
DROP ROLE vishnu_db_admin;
DROP ROLE vishnu_user;

REM =======================================================
REM create user
REM three separate commands, so the create user command 
REM will succeed regardless of the existence of the 
REM DEMO and TEMP tablespaces 
REM =======================================================

CREATE USER vishnu IDENTIFIED BY &pass;

ALTER USER vishnu DEFAULT TABLESPACE &tbs
              QUOTA UNLIMITED ON &tbs;

ALTER USER vishnu TEMPORARY TABLESPACE &ttbs;

GRANT CREATE SESSION, CREATE VIEW, ALTER SESSION, CREATE SEQUENCE TO vishnu;
GRANT CREATE SYNONYM, CREATE DATABASE LINK, RESOURCE TO vishnu;


-- CREATE Role Vishnu Database Administrator

CREATE ROLE vishnu_db_admin;

GRANT CONNECT, RESOURCE TO vishnu_db_admin;

-- CREATE Role Vishnu Standard User

CREATE ROLE vishnu_user;
GRANT CONNECT TO vishnu_user;

-- ASSIGN Roles to "vishnu" user
GRANT vishnu_user TO vishnu;
GRANT vishnu_db_admin TO vishnu;



