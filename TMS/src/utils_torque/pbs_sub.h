#ifndef PBS_SUB_H
#define PBS_SUB_H

int parse_file(int    argc, 
               char **argv, 
               int    pass);  

int pbs_prepare_script(int    argc, 
                       char **argv, 
                       char **envp, 
                       char script_tmp[],
                       char destination_cpy[],
                       char server_out_cpy[],
                       struct attrl **attrib_cpy) ;

int get_pbs_error_msg(char error_msg[]);

#endif
