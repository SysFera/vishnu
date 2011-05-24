/**
 * \file get_file_info.cpp
 * This file defines the VISHNU  FMS displayer functions 
 * \author Daouda Traore (daouda.traore@sysfera.com)
 */


#ifndef _FMS_DISPLAYER_
#define _FMS_DISPLAYER_

#include "api_fms.hpp"

/**
 * \brief Helper function to display the information of a file
 * \param os: The output stream in which the list will be printed 
 * \param fstat: The inode information
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, FileStat& fileStat);

/**
 * \brief Helper function to display the information of a file
 * \param os: The output stream in which the list will be printed 
 * \param fileTransfer: The content of a file transfer 
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, FileTransfer& fileTransfer);

/**
 * \brief Helper function to display the information of a file
 * \param os: The output stream in which the list will be printed 
 * \param fileTransferList: The file transfer list
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, FileTransferList& fileTransferlist);

/**
 * \brief Helper function to display the information of a file
 * \param os: The output stream in which the list will be printed 
 * \param stringList: The content of the directory 
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, StringList& stringList);

#endif
