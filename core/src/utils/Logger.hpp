/*
 * \file Logger.hpp
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

#ifndef LOGGER_H
#define LOGGER_H

#include <syslog.h>
#include <string>
#include <streambuf>

#define LOG(msg, logLevel) vishnu::log(msg, logLevel)

/**
 * \enum LogPriority
 * \brief The priority of the messages
 */
enum LogPriority {
    LogEmerg   = LOG_EMERG,   // system is unusable
    LogAlert   = LOG_ALERT,   // action must be taken immediately
    LogCrit    = LOG_CRIT,    // critical conditions
    LogErr     = LOG_ERR,     // error conditions
    LogWarning = LOG_WARNING, // warning conditions
    LogNotice  = LOG_NOTICE,  // normal, but significant, condition
    LogInfo    = LOG_INFO,    // informational message
    LogDebug   = LOG_DEBUG    // debug-level message
};

std::ostream& operator<< (std::ostream& os, const LogPriority& log_priority);

/**
 * \class Logger
 * \brief Base class for the loggers
 */
class Logger : public std::basic_streambuf<char, std::char_traits<char> > {
public:
    explicit Logger(const std::string& programName, int facility);
    ~Logger();

protected:
    int sync();
    int overflow(int c);

private:
    friend std::ostream& operator<< (std::ostream& os, const LogPriority& logPriority);

    /**
     * @brief Buffer to log
     */
    std::string mbuffer;

    /**
     * @brief Severity, aka syslog priority
     */
    int mlogSeverity;

    /**
     * @brief Sets the component that logs the info
     */
    std::string mprogramName;
};

namespace vishnu {
  /**
   * @brief Add entry to log
   * @param msg The message to log
   * @param level The severity
   */
  void
  log(const std::string& msg, int level);
}

#endif // LOGGER_H
