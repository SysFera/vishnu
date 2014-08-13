
-- table filetransfer
ALTER TABLE filetransfer DROP COLUMN transferid;
ALTER TABLE filetransfer CHANGE numfiletransferid transferid BIGINT NOT NULL AUTO_INCREMENT;

-- table machine
ALTER TABLE machine DROP COLUMN machineid;
ALTER TABLE machine CHANGE nummachineid machineid BIGINT NOT NULL AUTO_INCREMENT;


-- foreign keys
ALTER TABLE job DROP FOREIGN KEY machine_nummachineid;
ALTER TABLE description DROP FOREIGN KEY machine_nummachineid;
ALTER TABLE account DROP FOREIGN KEY machine_nummachineid;
ALTER TABLE account CHANGE machine_nummachineid machine_machineid BIGINT NOT NULL;