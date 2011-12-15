/**
 * \file FMSDisplayer.hpp
 * This file defines the VISHNU  FMS FMSDisplayer functions 
 * \author Daouda Traore (daouda.traore@sysfera.com)
 */


#ifndef _FMS_DISPLAYER_
#define _FMS_DISPLAYER_

#include "api_fms.hpp"

/**
 * \brief Helper function to display the information of a file
 * \param os The output stream in which the list will be printed 
 * \param fileStat The inode information
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, FileStat& fileStat);

/**
 * \brief Helper function to display the information of a file
 * \param os: The output stream in which the list will be printed 
 * \param fileStatlist: The file info list
 * \return The output stream in which the list of file information has been printed
 */
std::ostream&
operator<<(std::ostream& os, FileStatList& fileStatlist);

/**
 * \brief Helper function to display the information of a file
 * \param os The output stream in which the list will be printed 
 * \param dirEntry The directory entry  information
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, DirEntry& dirEntry);

/**
 * \brief Helper function to display the information of a file
 * \param os: The output stream in which the list will be printed 
 * \param dirEntrylist: The file info list
 * \return The output stream in which the list of file information has been printed
 */
std::ostream&
operator<<(std::ostream& os, DirEntryList& dirEntrylist);






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
 * \param fileTransferlist: The file transfer list
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, FileTransferList& fileTransferlist);

/**
 * \brief function to convert mode value to string
 * \param mode The mode value to convert
 * \return converted mode value
 */
string ConvertModeToString(const mode_t& mode);

#endif
