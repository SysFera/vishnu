#include "daemon_cleaner.hpp"
#include "sessionUtils.hpp"
#include "api_ums.hpp"
#include <fstream>
#include <boost/lexical_cast.hpp>
#ifdef WIN32
#include <windows.h>
#include <process.h>
#endif
//{{RELAX<MISRA_0_1_3> Because these variables are used in this file or other files

/**
 * \brief The home directory
 */
bfs::path home_dir;

/**
 * \brief The session storage  directory
 */
bfs::path session_dir;

/**
 * \brief The VISHNU output files directories
 */

bfs::path vishnu_dir;

/**
 * \brief The daemon locked file
 */

bfs::path daemon_file;
/**
 * \brief The processus information directory
 */

bfs::path proc_dir("/proc");

//}}RELAX<MISRA_0_1_3>

using namespace std;
using namespace vishnu;

/**
 * \brief Check if the pid exists
 * \fn bool pid_exists(const std::string& pid)
 * \param pid The given pid
 * \return True if the pid exists
 */


bool
pid_exists(const std::string& pid){
 
#ifdef __WIN32__
  int ipid = boost::lexical_cast< int >( pid );
  HANDLE process = OpenProcess(SYNCHRONIZE, FALSE,ipid );
  DWORD ret = WaitForSingleObject(process, 0);
  CloseHandle(process);
  return ret == WAIT_TIMEOUT;
#else
  extern bfs::path proc_dir;
  bfs::path token(proc_dir);
  token /= pid;
  return bfs::exists(token);
#endif
}


/**
 * \brief Deletes the file corresponding to the terminal closed
 * \param dietConfig: The VISHNU configuration file
 * \param ac: The number of command parameters
 * \param av: The names of parameters
 */


void 
deleter(char* dietConfig,int ac,char* av[]){

  extern bfs::path daemon_file;
  extern bfs::path session_dir;


  bi::file_lock f_lock(daemon_file.string().c_str());

  {
    bi::scoped_lock<bi::file_lock> lock(f_lock,bi::try_to_lock_type());

    if (false == lock.owns()) {
      exit(EXIT_SUCCESS);
    }


    while(true) {

      bfs::directory_iterator it = bfs::directory_iterator(session_dir);

      for (; it != bfs::directory_iterator(); ++it) {// loop over session directory

        const bfs::path current_path = it->path();

        std::string pid = (current_path.filename()).string();


        if (!pid_exists(pid)) {

          // close all sessions opened by disconnect mode before deleting file

          SessionContainer allSessions=getAllSessions(current_path.string());// get all sessions stored in file

          if (false==allSessions.empty()){ // is there a session?
            
            BOOST_FOREACH (SessionEntry session, allSessions){


              if(session.getClosePolicy()==2) {//that session is open by disconnect mode


                if (vishnuInitialize(dietConfig, ac, av)) {

#ifndef __WIN32__
                  syslog(LOG_ERR,"DIET initialization failed !");
#endif

                  exit (EXIT_FAILURE);

                }

                try{

                close (session.getSessionKey()); // and need to be closed
               
                }
                catch(VishnuException & e ){// if the close command fails
                 
                  if (false==checkBadSessionKeyError(e)){// check if we need to stop the daemon
                    
#ifndef __WIN32__
                     syslog(LOG_ERR, "The file is corrupted");
#endif
                    
                     exit(e.getMsgI());

                  }

                }
                vishnuFinalize();
              }

            }// end of looping over the file

          }

          bfs::remove(current_path);// remove the file 

        }
      }
      // take a pause
      btt::sleep(bpt::seconds(5));//%RELAX<MISRA_0_1_3> Because it used to take a pause
    }

    exit(EXIT_SUCCESS);

  }
}


/**
 *\brief Creates a child process and call the deleter() function
 * \param dietConfig: The VISHNU configuration file
 * \param ac: The number of command parameters
 * \param av: The names of parameters
 */



void 
cleaner(char* dietConfig,int ac,char* av[]){
  // declare all global variables
  extern bfs::path home_dir;
  extern bfs::path session_dir;
  extern bfs::path vishnu_dir;
  extern bfs::path daemon_file;


  home_dir = getenv("HOME"); // set the user home directory
  vishnu_dir = home_dir ;  // set the directory in which will be stored the daemon lock file

  vishnu_dir /= ".vishnu";
  daemon_file=vishnu_dir;
  session_dir /= vishnu_dir;   // set the directory in which will be stored all session files

  daemon_file/="daemon.pid";


  session_dir /= "sessions";


  if (false==bfs::exists(session_dir)){ // make sure session_dir exists 

    bfs::create_directories(session_dir);

  }


  if (false==bfs::exists(daemon_file)){ // make sure daemon_file exists

    bfs::ofstream f (daemon_file);

#ifndef __WIN32__
    f<<getpid();    // record daemon pid
#else
    f<<_getpid();
#endif

    f.close();

  }

#ifndef __WIN32__
  pid_t pid = fork();// create a process resident to delete and close  all sessions corresponding on close terminal

  if (pid < 0) {
    std::cerr << "cleaning process: fork() failed" << std::endl;
  } else if (0 == pid) {
    deleter(dietConfig,ac,av);
  }
#else

  string exepath("deleter.exe");
  TCHAR* cmdlineP;
  string cmdline;
  STARTUPINFO         startup_info;
  PROCESS_INFORMATION process_info;
  memset(&startup_info, 0, sizeof(startup_info)); 
  memset(&process_info, 0, sizeof(process_info)); 
  startup_info.cb = sizeof(startup_info); 
  cmdlineP = GetCommandLine();
  cmdline.append(exepath);
  cmdline.append(" ");
  cmdline.append(dietConfig);
  cmdline.append(" ");
  cmdline.append(cmdlineP);

  if (CreateProcess(const_cast<LPCSTR>(exepath.c_str()), 
    const_cast<LPSTR>(cmdline.c_str()), 0, 0, false, 
    CREATE_DEFAULT_ERROR_MODE, 0, 0, 
    &startup_info, &process_info) == false) 
  { 
    /* CreateProcess failed */ 
    std::cerr << "cleaning process: CreateProcess() failed" << std::endl;
  } 
#endif

  return;
}

