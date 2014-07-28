-- This script is for initialization of the VISHNU database content
-- Script name          : database_init.sql
-- Script owner         : SysFera SA

-- Last update          : 24/07/12

-- Creates the vishnu default administrator (root)
INSERT INTO users (userid, pwd, privilege, passwordstate, status)
VALUES('root','8vU7h/n6KOW8reLF1Lt2/5gzjZ.HvGK3A9doVMbmPtaYKkkCoWrMKiPa7s.fEigSTS5gQmX5F8BlW2XotCeHa0', 1, 1, 1);

