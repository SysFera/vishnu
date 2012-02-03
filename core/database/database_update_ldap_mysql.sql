-- This script is for update of the VISHNU database content
-- Script name          : database_update_ldap_mysql.sql
-- Script owner         : SysFera SAS

-- REVISIONS
-- Revision nb          : 1.2.7
-- Revision date        : 02/02/12
-- Revision author      : Kevin Coulomb <kevin.coulomb@sysfera.com>
-- Revision comment     : initial version for upgrade from release 1.2.6 to 1.2.7

CREATE TABLE authsystem (
  numauthsystemid SERIAL NOT NULL,
  vishnu_vishnuid INTEGER UNSIGNED  NOT NULL  ,
  authsystemid VARCHAR(255)    ,
  name VARCHAR(255)  ,
  uri VARCHAR(255)  ,
  authlogin VARCHAR(255)  ,
  authpassword VARCHAR(255)  ,
  userpwdencryption INTEGER UNSIGNED  ,
  types INTEGER UNSIGNED  ,
  state INTEGER UNSIGNED  ,
PRIMARY KEY(authsystemid),
  FOREIGN KEY(vishnu_vishnuid)
     REFERENCES vishnu(vishnuid)
);    

CREATE TABLE authaccount (
  authaccountid SERIAL NOT NULL,
  authsystem_authsystemid INTEGER UNSIGNED  NOT NULL  ,
  users_numuserid INTEGER UNSIGNED  NOT NULL  ,
  aclogin VARCHAR(255)  ,
PRIMARY KEY(authaccountid),
  FOREIGN KEY(users_numuserid)
    REFERENCES users(numuserid),
  FOREIGN KEY(authsystem_authsystemid)
    REFERENCES authsystem(authsystemid)
);

CREATE TABLE ldapauthsystem (
  ldapauthsystid SERIAL NOT NULL,
  authsystem_authsystemid INTEGER UNSIGNED  NOT NULL  ,
  ldapbase VARCHAR(255)  ,
PRIMARY KEY(ldapauthsystid),
  FOREIGN KEY(authsystem_authsystemid)
    REFERENCES authsystem(authsystemid)
);


GRANT SELECT, INSERT, UPDATE, DELETE ON authaccount TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON authsystem TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON ldapauthsystem TO "vishnu_db_admin";

GRANT SELECT, INSERT, UPDATE, DELETE ON authaccount TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON authsystem TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON ldapauthsystem TO "vishnu_user";

ALTER TABLE vsession ADD COLUMN authid VARCHAR(255);
