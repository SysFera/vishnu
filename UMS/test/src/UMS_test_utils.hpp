#ifndef UMS_TEST_UTILS_HPP
#define UMS_TEST_UTILS_HPP

#include <fstream>
#include <sstream>

/**
 * \brief To put the a string content on a file
 * \param path  the file path
 * \param content the file content
 * \return true if all names are found within the directory long content
 */
void
putOnFile(const std::string& path, const std::string& content) {
  std::ofstream ofile(path.c_str());
  ofile << content;
  ofile.close();
}

/**
 * \brief To change the content of a file by replacing a specific values
 * \param path  the file path
 * \param oldValue the value which will be replaced
 * \param newValue the new value
 */
void
replaceAllOccurences(const std::string& path,
                     const std::string& oldValue,
                     const std::string& newValue) {

  std::string fileContent;

  std::ifstream ifs (path.c_str());
  if (ifs.is_open()) {
    std::ostringstream oss;
    oss << ifs.rdbuf();
    fileContent = oss.str();
    ifs.close();
  }
  size_t pos = fileContent.find(oldValue);
  while(pos!=std::string::npos) {
    fileContent.replace(pos, oldValue.size(), newValue, 0, newValue.size());
    pos = fileContent.find(oldValue, pos+newValue.size());
  }
  putOnFile(path, fileContent);
}

/**
 * \brief To add a line on a file
 * \param path  the file path
 * \param content the line which will be added
 * \param the old file content
 */
void
addLineToFile(const std::string& path,
              const std::string& line,
              std::string& oldfileContent) {

  std::ifstream ifs (path.c_str());
  if (ifs.is_open()) {
    std::ostringstream oss;
    oss << ifs.rdbuf();
    oldfileContent = oss.str();
    ifs.close();
  }
  putOnFile(path, line);
}
#endif