INSERT INTO vishnu("updatefreq", "formatiduser", "formatidjob", "formatidfiletransfer", "formatidmachine", "usercpt", "jobcpt", "filesubcpt", "machinecpt") values('100', 'user_$CPT', 'job_$CPT', 'transfer_$CPT', 'machine_$CPT', '2', '2', '2', '2');
INSERT INTO users("userid", "pwd", "firstname", "lastname", "privilege", "email", "passwordstate", status, "vishnu_vishnuid") values('admin_1', '75fvsW7kKijHx.KbF4V5PapqRw6SZcUVNOMawX1dbwoktJS1xvjZPZLwPVtrI2KG0a1lOnTeZlwyfuVMDr4Ay0', 'jean', 'dupont', '1', 'admin@admin.com', '1', '1', '1');
INSERT INTO users("userid", "pwd", "firstname", "lastname", "privilege", "email", "passwordstate", status, "vishnu_vishnuid") values('user_1', 'r0Mk0M4TsQKJE3lhP.yll0fKJgNKbWaAVo5MUuJtRRhQQot6AnOHXWhm1Mra92pgkD6zGRHmS78J/mIsw5mMA1', 'jeannot', 'dupont', '0', 'user@user.com', '1', '1', '1');
INSERT INTO machine("vishnu_vishnuid", "name", "site", "machineid", "status")values('1', 'tiger', 'berlin', 'machine_1', '1');
INSERT INTO account("machine_nummachineid", "user_numuserid", "aclogin", "sshpathkey", "home") values('1', '1', 'admin_1', 'key', 'home');
INSERT INTO account("machine_nummachineid", "user_numuserid", "aclogin", "sshpathkey", "home") values('1', '2', 'admin_1', 'key', 'home');
INSERT INTO account("machine_nummachineid", "user_numuserid", "aclogin", "sshpathkey", "home") values('2', '2', 'admin_1', 'key', 'home');
