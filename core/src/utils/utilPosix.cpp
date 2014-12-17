#include "utilServer.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/format.hpp>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "DatabaseResult.hpp"
#include "utilVishnu.hpp"
#include "DbFactory.hpp"
#include "SystemException.hpp"
#include "DbFactory.hpp"
#include "Server.hpp"
#include "vishnu_version.hpp"
#include "TMSVishnuException.hpp"
/**
 * \brief Function to parse a system error message
 * \param errorMsg the error message
 * \return the parsed message
 */
std::string
vishnu::parseErrorMessage (const std::string& errorMsg) {
  size_t commandPos;
  std::string result(errorMsg);

  commandPos = result.find(":");

  if (commandPos != std::string::npos) {
    result = result.substr(commandPos + 1);

    size_t endOfLinePos = result.find_last_of("\n");
    if (endOfLinePos != std::string::npos) {
      result.erase(endOfLinePos);
    }
  }

  return result;
}

/**
 * \brief Return the status value from file
 * \param file The status file
 * \return the int value
 */
int
vishnu::getStatusValue (const std::string& file) {

  std::string content = vishnu::get_file_content(file);
  size_t pos = content.find("\n");
  return vishnu::convertToInt(content.substr(0, pos));
}

/**
 *  \brief Function to move the file parameters to a given directory
 *  \param fileparams a list of key=value
 *  \param dir the directory where we create the links
 *  \return the string of the directory to which the link was created
 */
std::string
vishnu::moveFileData(const std::string& fileparam, std::string dir) {
  std::string directory="";
  std::string file="";
  size_t pos = fileparam.find("=");
  if (pos!=std::string::npos) {
    size_t pos1 = fileparam.find(" ", pos);
    if (pos1!=std::string::npos) {
      file = fileparam.substr(pos+1, pos1-pos);
    } else {
      file = fileparam.substr(pos+1);
    }
    size_t pos2 = file.rfind("/");
    if (pos2 != std::string::npos) {
      directory = file.substr(0, pos2);
      std::ostringstream oss;
      oss << "mv " << directory << "/* " << dir;
      std::string msg;
      if (! vishnu::execSystemCommand(oss.str(), msg)) {
        throw SystemException(ERRCODE_INVDATA, boost::str(boost::format("Can  not move the input files: %1%") %  msg));
      }
    }
  }
  return directory;

}


/**
 *  \brief Function to create a working directory. The working directory needs rwxt permissions
 *  \param path the path of the working directory
 *  \param isWorkingdir tell whether the directory will be a working directory
 */
void
vishnu::createDir(const std::string& path, const bool& isWorkingdir) {
  try {
    bfs::create_directories(path);
    if (isWorkingdir && chmod(path.c_str(),  // a working directory has the permissions rwxt
                              S_IRUSR|S_IWUSR|S_IXUSR // RWX for owner
                              |S_IRGRP|S_IWGRP|S_IXGRP // RWX for group
                              |S_IROTH|S_IWOTH|S_IXOTH // RWX for other
                              |S_ISVTX) != 0) {       // Sticky bit
      throw SystemException(ERRCODE_INVDATA, "Unable to set suitable permissions on the working directory "
                            + path) ;
    }
  } catch (bfs::filesystem_error &ex) {
    throw SystemException(ERRCODE_INVDATA, ex.what());
  }
}

int
vishnu::showVersion() {
  using boost::format;
  std::cout << format("vishnu %1%\n%2% %3%\n") % VISHNU_VERSION
    % VISHNU_BATCH_SCHEDULER
    % VISHNU_BATCH_SCHEDULER_VERSION;
  return EXIT_SUCCESS;
}
