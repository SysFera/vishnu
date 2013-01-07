/**
 * \file tms-posix.cpp
 * \brief This file contains the TMS-Posix scheduler for local batch.
 * \author Olivier Mornard (olivier.mornard@sysfera.com)
 * \date January 2013
 */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <boost/asio.hpp>

#include <stdio.h>

using namespace std;


/**
 * \brief To show how to use tms-posix
 * \fn int usage(char* cmd)
 * \param cmd The name of the program
 * \return Always 1
 */

static void
usage(char* cmd)
{
  cerr << "Usage: " << cmd << " COMMANDE_TYPE[SUBMIT] <BatchType> <JobSerializedPath> <SlaveErrorPath> <JobUpdatedSerializedPath>";
  cerr << " <SubmitOptionsSerializedPath> <job_script_path>" << endl;
  cerr << "\t\t\t\t\t" << " or " << endl;
  cerr << "Usage: " << cmd << " COMMANDE_TYPE[CANCEL] <BatchType> <JobSerializedPath>  <SlaveErrorPath>" << endl;
  exit(EXIT_FAILURE);
}

static int
Daemonize(void)
{
  int maxfd;
  int fd;
  
  switch (fork())
  {
    case -1:
      return -1;
    case 0:
      break;
    default:
      exit(EXIT_SUCCESS);
  }
  
  if (setsid() == -1)
  {
    return -1;
  }
  
  switch (fork())
  {
    case -1:
      return -1;
    case 0:
      break;
    default:
      exit(EXIT_SUCCESS);
  }
  
  (void)umask(0);
  (void)chdir("/");
  maxfd = sysconf(_SC_OPEN_MAX);
  if (maxfd == -1)
  {
    maxfd=128;
  }
  
  for (fd=1; fd<maxfd; fd++)
  {
    close(fd);
  }
  
  close(STDIN_FILENO);
  fd=open("/dev/null",O_RDWR);
  
  if (fd != STDIN_FILENO)
  {
    return -1;
  }
  (void)dup2(STDIN_FILENO,STDOUT_FILENO);
  (void)dup2(STDIN_FILENO,STDERR_FILENO);
  
  return 0;
}

static int
buildEnvironment(void)
{
  //int i;
  //char hostname[HOST_NAME_MAX];
  static const char* libHostname="VISHNU_SUBMIT_MACHINE_NAME";
  static const char* libNodefile="VISHNU_BATCHJOB_NODEFILE";
  static const char* libNumNodes="VISHNU_BATCHJOB_NUM_NODES";
  static const char templateHostname[]="/tmp/NODELIST_XXXXXX";
  char fileHostname[sizeof(templateHostname)];
  int fdHostname;
  boost::system::error_code ec;
  const string hostname = boost::asio::ip::host_name(ec);
  //static const path templateHostname("/tmp/NODELIST_%%%%%%");

   // variable VISHNU_SUBMIT_MACHINE_NAME  
  //(void)gethostname(hostname,sizeof(hostname));
  (void)setenv(libHostname, hostname.c_str(), true);
  
  // variable VISHNU_BATCHJOB_NODEFILE
  strncpy(fileHostname,templateHostname,sizeof(fileHostname));
  //fileHostname = unique_path(templateHostname,ec);
  fdHostname = mkstemp(fileHostname);
  (void)write(fdHostname,hostname.c_str(),strlen(hostname.c_str()));
  (void)close(fdHostname);
  (void)setenv(libNodefile,fileHostname,true);
  
  // variable VISHNU_BATCHJOB_NUM_NODES
  (void)setenv(libNumNodes, "1", true);
  
/**
  for (i=0; environ[i]; i++)
    printf("%s\n",environ[i]);
  **/

  return 0;
}

static bool
GetNextToken(ifstream& file)
{
  string keyname;
  char nextchar;

  // Look for the first char of directive name  
  while (file.get(nextchar))
  {
    if (isalnum(nextchar))
    {
      keyname.push_back(nextchar);
      break;
    }
  }

  if (file.eof())
    return false;
  
  // Scan the directive name
  while (file.get(nextchar))
  {
    if (!isalnum(nextchar))
    {
      break;
    }
    keyname.push_back(nextchar);
  }

  cout << "Directive:" << keyname << endl;

  return true;
}

static bool
GetNextDefinition(ifstream& file)
{
  char nextchar;
  
  if (!file.get(nextchar))
    return false;
cout<<nextchar;
  if (nextchar == '#')
  {
    cout<<"COMMENTS"<<endl;
    if (!file.get(nextchar))
      return false;
cout<<nextchar;
    if (nextchar == '@')
    {
    
      GetNextToken(file);
      
    }
    else file.unget();
  }
  
  while (file.get(nextchar))
  {
cout<<nextchar;
    if (nextchar == '\n')
    {
    cout<<"NEWLINE";
      break;
    }
  }
  return true;
}

static bool
ParseCommand(char* Command)
{
  ifstream file;
  
  file.open(Command);

  while (GetNextDefinition(file));
/***  
  char c;
  while (file.get(c))
  {
    cout.put(c);
  }
**/
  file.close();

  return true;
}

static int
execCommand(char* command,char* fstderr)
{
  char* args[16];
  char commandLine[255];
  pid_t pid;
  ostringstream temp;
  static const char* libBatchId="VISHNU_BATCHJOB_ID";
  static const char* libBatchName="VISHNU_BATCHJOB_NAME";
  int fd;
  
  args[1]=(char *)"/bin/sh";
  args[2]=(char *)"-c";
  strcpy(commandLine,"exec ");
  strncat(commandLine,command,sizeof(commandLine)-strlen(commandLine)-1);
  args[3]=commandLine;
  args[4]=NULL;
  args[0]=args[1];
  
  if ((pid=fork()) == 0)
  {
    pid=getpid();
    temp<<pid;
    (void)setenv(libBatchId,temp.str().c_str(),true);

    fd=open(fstderr,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
    close(STDERR_FILENO);
    dup2(fd,STDERR_FILENO);
    close(fd);

    ParseCommand(command);
    
    execvp(args[1],args+1);
  }
  
  return 0;
}

/**
 * \brief The main function
 * \param argc Number of parameter
 * \param argv List of argument
 * \param envp Array of environment variables
 * \return The result of the diet sed call
 */
int
main(int argc, char* argv[], char* envp[])
{
  char* myName;  // the Name of the program (and not the path)
  string action; // the action to do (submit or cancel)
  string slaveJobFile;
  string optionsPath;
  string jobScriptPath;
  
  if ((myName = strrchr(argv[0], '/')) != NULL)
    myName++;
  else
    myName = argv[0];
  
  (void)buildEnvironment();
  
  if (argc < 2)
    usage(myName);

  action = string(argv[1]);
  if(action.compare("SUBMIT")==0) {
/**    if(argc < 8) {
       usage(myName);
     }
    slaveJobFile = argv[5];
    optionsPath = argv[6];
    jobScriptPath = argv[7];
    ***/
    (void)execCommand(argv[2],argv[3]);
  }
  else if(action.compare("CANCEL")==0) {
    if(argc < 5) {
     usage(myName);
    }
  } else {
    usage(myName);
  }

  return EXIT_SUCCESS;
} // End : main ()