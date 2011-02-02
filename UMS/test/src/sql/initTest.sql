INSERT INTO vishnu("updatefreq", "formatiduser", "formatidjob", "formatidtransfer", "formatidmachine", "usercpt", "jobcpt", "transfercpt", "machinecpt")
       values('100', 'user_$CPT', 'job_$CPT', 'transfer_$CPT', 'machine_$CPT', '2', '2', '2', '2');

INSERT INTO users("userid", "password", "firstname", "lastname", "email", "privilege", "passwordstate", "numvishnuid")
       values('admin_1', 'admin', 'jean', 'dupont', 'admin@admin.com', 'A', '1', '1');
INSERT INTO users("userid", "password", "firstname", "lastname", "email", "privilege", "passwordstate", "numvishnuid")
       values('user_1', 'user', 'jeannot', 'dupont', 'user@user.com', 'U', '1', '1');

