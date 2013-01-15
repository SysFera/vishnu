#ifndef __POSIXPARSER__HH__
#define __POSIXPARSER__HH__


#include <string>


class Definition {
  public:
    std::string key;
    std::string value;
};

bool ParseCommand(char* Command);
bool ParseString();

#endif
