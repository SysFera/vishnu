-- This script is for initialization of the VISHNU database content
-- Script name          : database_init.sql
-- Script owner         : SysFera SA

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 22/02/11
-- Revision author      : Benjamin Isnard <benjamin.isnard@sysfera.com>
-- Revision comment     : initial version

-- Creates required user options
INSERT INTO optionu (optionid, description, defaultvalue) VALUES (1, 'VISHNU_CLOSE_POLICY', 1);
INSERT INTO optionu (optionid, description, defaultvalue) VALUES (2, 'VISHNU_TIMEOUT', 3600);
INSERT INTO optionu (optionid, description, defaultvalue) VALUES (3, 'VISHNU_TRANSFER_CMD', 0);
INSERT INTO optionu (optionid, description, defaultvalue) VALUES (4, 'VISHNU_TRANSFER_TIMEOUT', 0);

-- Creates the vishnu default administrator (root)
INSERT INTO users (userid, pwd, privilege, passwordstate, status)
VALUES('root','8vU7h/n6KOW8reLF1Lt2/5gzjZ.HvGK3A9doVMbmPtaYKkkCoWrMKiPa7s.fEigSTS5gQmX5F8BlW2XotCeHa0', 1, 1, 1);

