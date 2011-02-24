#include "daemon_cleaner.hpp"
#include "sessionUtils.hpp"

bfs::path home_dir;
bfs::path session_dir;
bfs::path proc_dir("/proc");
const char *lockname = "vishnu1";

bool pid_exists(const std::string& pid)
{
  extern bfs::path proc_dir;
  bfs::path token(proc_dir);
  token /= pid;
  return bfs::exists(token);
}




void deleter()
{

  extern const char* lockname;
  extern bfs::path session_dir;

  bi::named_mutex m(bi::open_or_create, lockname);

  {
    bi::scoped_lock<bi::named_mutex> lock(m, bi::try_to_lock_type());
    if (false == lock) {
      std::cout << "cleaning process: already running" << std::endl;
      exit(EXIT_SUCCESS);
    }

    std:: cout << "daemon cleaner initialized"<<std::endl;
    while(true) {

      bfs::directory_iterator it = bfs::directory_iterator(session_dir);

      for (; it != bfs::directory_iterator(); ++it) {
        const bfs::path current_path = it->path();
        std::string pid = (current_path.filename()).string();

        if (!pid_exists(pid)) {
          // close all sessions opened by disconnect mode before deleting file
          /*SessionContainer allSessions=getAllSessions(pid);
          BOOST_FOREACH (const SessionEntry& session, allSessions){
           if(1==session.getClosePolicy()) {

            // close (session.getSessionKey);
           }

          }*/
          bfs::remove(current_path);
        }
      }

      btt::sleep(bpt::seconds(5));
    }


  exit(EXIT_SUCCESS);

  }
}





void cleaner()
{
  extern bfs::path home_dir;
  extern bfs::path session_dir;
  // change current directory
  home_dir = getenv("HOME");
  session_dir = home_dir ;
  session_dir /= ".vishnu";
  session_dir /= "sessions";
  std::cout << "cleaning process: initialized"<< std::endl;
  pid_t pid = fork();

  if (pid < 0) {
    std::cerr << "cleaning process: fork() failed" << std::endl;
  } else if (0 == pid) {
    deleter();
  }

  return;
}

