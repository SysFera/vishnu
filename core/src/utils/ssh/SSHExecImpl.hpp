/**
 * \file SSHExecImpl.hpp
 * \brief This file implements the remote execution of a function
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
 * \date 11/05/2011
 */
#ifndef __SSHEXECIMPL__HH__
#define __SSHEXECIMPL__HH__

#include <boost/bind.hpp>
#include "SSHResult.hpp"
class SSHExecImpl;
#include "SSHExec.hpp"

/**
 * \class SSHExecImpl
 * \brief A class implementing the remote calls
 */
class SSHExecImpl {
public:
  /**
   * \brief COnstructor
   */
  SSHExecImpl(SSHExec* e);
  /**
   * \brief Destructor
   */
  ~SSHExecImpl();
  /**
   * \brief Functor
   */
  SSHResult*
  execSSH(string s);

private:
  SSHExec* me;  
}; // end class ssh execc impl

#endif // end sshexecimpl
