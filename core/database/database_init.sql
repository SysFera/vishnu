-- This script is for initialization of the VISHNU database content
-- Script name          : database_init.sql
-- Script owner         : SysFera SAS

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 22/02/11
-- Revision author      : Benjamin Isnard <benjamin.isnard@sysfera.com>
-- Revision comment     : initial version

-- Creates VISHNU record
INSERT INTO vishnu (vishnuid, updatefreq, usercpt, machinecpt, jobcpt, fileSubcpt) VALUES (1, 0, 0, 0, 0 ,0);

-- Creates required user options
INSERT INTO optionu (optionid, description, defaultvalue) VALUES (1, 'VISHNU_CLOSE_POLICY', 1);
INSERT INTO optionu (optionid, description, defaultvalue) VALUES (2, 'VISHNU_TIMEOUT', 3600);

-- Creates the vishnu default administrator (root)
INSERT INTO users (vishnu_vishnuid, userid, pwd, privilege, passwordstate, status)
VALUES(1, 'root','ztV1aPn8GPzSLab1EL5kBaqyyJLrL3XXxSVgdl1.TPhghpkUXejxSLPntLq8keE/iJZXpXkGo848XzLYJMFvB.', 1, 1, 1);

