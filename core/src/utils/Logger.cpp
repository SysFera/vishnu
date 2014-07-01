/*
 * Logger.cpp
# ------------------------------------------------------------------------ #
# Copyright (c) 2010-2012 Rodrigue Chakode (rodrigue.chakode@ngrt4n.com)   #
# Last Update : 22-12-2012                                                 #
#                                                                          #
# This file is part of NGRT4N (http://ngrt4n.com).                         #
#                                                                          #
# NGRT4N is free software: you can redistribute it and/or modify           #
# it under the terms of the GNU General Public License as published by     #
# the Free Software Foundation, either version 3 of the License, or        #
# (at your option) any later version.                                      #
#                                                                          #
# NGRT4N is distributed in the hope that it will be useful,                #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
# GNU General Public License for more details.                             #
#                                                                          #
# You should have received a copy of the GNU General Public License        #
# along with NGRT4N.  If not, see <http://www.gnu.org/licenses/>.          #
#--------------------------------------------------------------------------#
 */

#include "Logger.hpp"
#include <string.h>
#include <ostream>
#include <iostream>
#include <libio.h>

Logger::Logger(const std::string& programName, int facility)
  : mlogSeverity(LOG_DEBUG),
    mprogramName(programName){
  openlog(mprogramName.c_str(), LOG_PID, facility);
}

Logger::~Logger() {
  closelog();
}

int Logger::sync() {
  if (mbuffer.length()) {
    syslog(mlogSeverity, "%s", mbuffer.c_str());
    mbuffer.erase();
    mlogSeverity = LOG_DEBUG; // default to debug for each message
  }
  return 0;
}

int Logger::overflow(int c) {
  if (c != EOF) {
    mbuffer += static_cast<char>(c);
  } else {
    sync();
  }
  return c;
}

std::ostream& operator<< (std::ostream& logStream, const LogPriority& logPriority) {
  static_cast<Logger*>(logStream.rdbuf())->mlogSeverity = (int)logPriority;
  return logStream;
}


/**
 * @brief Add entry to log
 * @param msg The message to log
 * @param level The severity
 */
void
vishnu::log(const std::string& msg, int level)
{
  std::clog << static_cast<LogPriority>(level) << msg << std::endl;
  if (level <= LogErr){
      std::cerr << msg << std::endl;
  }
}
