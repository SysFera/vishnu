#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>

extern "C" {
#include "slurm-submit.h"
}

using namespace std;

int main(int argc, char *argv[])
{
  int pipefd[2];
  if(pipe(pipefd)) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  int pipeOutfd[2];
  if(pipe(pipeOutfd)) {
    perror("pipe");
    exit(EXIT_FAILURE);
    close(pipefd[0]); 
    close(pipefd[1]); 
  }

  int pipeJobIdfd[2];
  if(pipe(pipeJobIdfd)) {
    perror("pipe");
    exit(EXIT_FAILURE);
    close(pipefd[0]);
    close(pipefd[1]);
    close(pipeJobIdfd[0]);
    close(pipeJobIdfd[1]);
  }


  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  uint32_t  jobId ;  
  std::stringstream os;
  if(pid==0) { /* Child writes to pipe */
    close(pipefd[0]); /* Close unused read end */
    close(pipeOutfd[0]); /* Close unused read end */
    
    dup2(pipefd[1], 2);
    dup2(pipeOutfd[1], 1);
    
    close(pipefd[1]);
    close(pipeOutfd[1]);

    submit_response_msg_t *resp;
    slurm_submit(argc, argv, &resp);
    jobId = resp->job_id;
    os << jobId;
    char* jobIdStr = const_cast<char*>(strdup(os.str().c_str()));
    close(pipeJobIdfd[0]);
    write(pipeJobIdfd[1], jobIdStr, strlen(jobIdStr));
    close(pipeJobIdfd[1]);
    exit(EXIT_SUCCESS);
  } else { /* Parent reads from pipe */

    close(pipefd[1]); /* Close unused write end */
    close(pipeOutfd[1]); /* Close unused write end */
    close(pipeJobIdfd[1]); /* Close unused write end */ 
    char c;
   
    std::string errorMsg;
    std::string output;
    while(read(pipeOutfd[0], &c, 1))
            output +=c;
     close(pipeOutfd[0]);

    while(read(pipefd[0], &c, 1))
      errorMsg +=c;

    if(output.size()!=0) {
      std::cout <<  output << std::endl;
    }

    if(errorMsg.size()!=0) {
      size_t size = errorMsg.size();
      string toFind("error:");
      size_t pos = errorMsg.find(toFind);
      if(pos==string::npos){
        std::cout << "TEST*****: " << errorMsg << std::endl;
      } else {
        std::cout << "TEST*****: " << errorMsg.substr(pos+toFind.size()+1, size-pos-1) << std::endl;
      }
    }
    close(pipefd[0]);
    
    std::string jobIdStr;
    while(read(pipeJobIdfd[0], &c, 1))
            jobIdStr +=c;
    close(pipeJobIdfd[0]);
    wait(NULL);                /* Wait for child */
    cout << "++++++++++JobIdValue = " << jobIdStr << std::endl;
    exit(EXIT_SUCCESS);
  }
}

