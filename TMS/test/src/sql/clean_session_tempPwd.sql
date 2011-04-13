DELETE FROM vsession;
DELETE FROM account;
DELETE FROM users    WHERE userid='toto_1';
DELETE FROM users    WHERE userid='toto_1';
DELETE FROM machine  WHERE machineid='machine_2';
UPDATE users("userid", "pwd", "firstname", "lastname", "privilege", "email", "passwordstate", status, "vishnu_vishnuid") values('admin_1', 'admin', 'jean', 'dupont', '1', 'admin@admin.com', '0', '1', '1');
