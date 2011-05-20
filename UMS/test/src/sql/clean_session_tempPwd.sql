DELETE FROM vsession;
DELETE FROM account;
DELETE FROM users    WHERE userid='toto_1';
DELETE FROM users    WHERE userid='toto_1';
DELETE FROM machine  WHERE machineid='machine_2';
UPDATE users SET passwordstate=0 WHERE userid='admin_1';
-- FIXME REMOVE THIS FILE!!!