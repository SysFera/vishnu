-- This script is for update of the VISHNU database content
-- Script name          : database_update_ldap_postgre.sql
-- Script owner         : SysFera SAS

-- REVISIONS
-- Revision nb          : 1.2.8
-- Revision date        : 02/02/12
-- Revision author      : Kevin Coulomb <kevin.coulomb@sysfera.com>
-- Revision comment     : initial version for upgrade from release 1.2.7 to 1.2.8


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
    REFERENCES authsystem(authsystemid) ON DELETE CASCADE);

CREATE TABLE ldapauthsystem (
  ldapauthsystid SERIAL  NOT NULL ,
  authsystem_authsystemid INTEGER   NOT NULL ,
  ldapbase VARCHAR(255)      ,
PRIMARY KEY(ldapauthsystid)  ,
  FOREIGN KEY(authsystem_authsystemid)
    REFERENCES authsystem(authsystemid) ON DELETE CASCADE);

GRANT SELECT, INSERT, UPDATE, DELETE ON authaccount TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON authsystem TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON ldapauthsystem TO "vishnu_db_admin";

GRANT SELECT, INSERT, UPDATE, DELETE ON authaccount TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON authsystem TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON ldapauthsystem TO "vishnu_user";

GRANT ALL ON SEQUENCE authaccount TO "vishnu_user";
GRANT ALL ON SEQUENCE authsystem TO "vishnu_user";
GRANT ALL ON SEQUENCE ldapauthsystem TO "vishnu_user";

ALTER TABLE vsession ADD COLUMN authid VARCHAR(255);

