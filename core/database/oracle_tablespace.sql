create tablespace vishnu_data datafile '/usr/lib/oracle/xe/oradata/XE/vishnu.dbf' 
	SIZE 100M;

create temporary tablespace vishnu_tmp tempfile '/usr/lib/oracle/xe/oradata/XE/vishnu_tmp.dbf'
	SIZE 20M;
