#ifndef DAEMON_CLEANER_HPP
#define DAEMON_CLEANER_HPP
/**
 * \file daemon_cleaner.hpp
 * \brief This file defines a few functions to close properly session
 * and clean the corresponding file.
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 * \author Haikel Guemar (haikel.guemar@sysfera.com)
 */

/* Posix headers */
#include <cerrno>
#include <fcntl.h>
#include <cstdio>
#include <cstring>
#include <csignal>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/* C standard library */
#include <cstdlib>
/* C++ standard library */
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
/* Boost headers */
#include <boost/scoped_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
namespace bfs = boost::filesystem;
namespace bi = boost::interprocess;
namespace btt = boost::this_thread;
namespace bpt = boost::posix_time;
bool pid_exists(const std::string& pid);
void deleter(char* dietConfig,int ac,char* av[]);
void cleaner(char* dietConfig,int ac,char* av[]);
#endif
