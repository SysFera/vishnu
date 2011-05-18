/**
 * \file SSHResult.hpp
 * \brief This file implements the SSH result function
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
 * \date 11/05/2011
 */
#ifndef __SSHRESULT__HH__
#define __SSHRESULT__HH__

#include <string>

using namespace std;

/**
 * \class SSHResult
 * \brief The results of a SSH execution of a command
 */
class SSHResult {
public:
  /**
   * \brief Constructor
   */
  SSHResult();
  /**
   * \brief Destructor
   */
  ~SSHResult();
  /**
   * \brief To get the command status
   */
  int
  getStatus();
  /**
   * \brief To set the command status
   */
  void
  setStatus(int st);
  /**
   * \brief To get the standard output of the command
   */
  string
  getOutput();
  /**
   * \brief To set the standard output of the command
   */
  void
  setOutput(string out);
  /**
   * \brief To get the error output of the command
   */
  string
  getError();
  /**
   * \brief To set the error output of the command
   */
  void
  setError(string err);
protected:
private:
  /**
   * \brief Status from waitpid
   */
  int    mstatus;
  /**
   * \brief Output of execution
   */
  string mout;
  /**
   * \brief Error of execution
   */
  string merr;
};

#endif // sshresult
