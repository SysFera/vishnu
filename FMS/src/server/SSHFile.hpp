
/**
 * \file SSHFile.hpp
 * This file declares a  server class to handle a a remote file (through ssh)
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef SSHFILE_HH
#define SSHFILE_HH

#include <string>
#include <utility>
#include "SessionServer.hpp"

/* The stat command uses different syntax depending on the system type. */
/* BSD and Mac OS X command differs from the Linux one. */
#ifdef BSD_LIKE_SYSTEM
#define STATCMD "LANG=C stat -f \"%Su %Sg %Lp %u %g %z %a %m %c %HT\" "
#else
/**
 * \brief An alias of stat command 
 */
#define STATCMD " LANG=C stat --format=\"%U %G %a %u %g %s %X %Y %Z %F\" "
#endif

/* Define the command to use through ssh. */
/**
 * \brief An alias of change group command 
 */
#define CHGRPCMD "chgrp "
/**
 * \brief An alias of change mode command 
 */
#define CHMODCMD "chmod "
/**
 * \brief An alias of head command 
 */
#define HEADCMD  "head -"
/**
 * \brief An alias of head command 
 */
#define CATCMD  "cat "
/**
 * \brief An alias of mkdir command 
 */
#define MKDIRCMD "mkdir "
/**
 * \brief An alias of touch command 
 */
#define MKFILECMD "touch "
/**
 * \brief An alias of rm command 
 */
#define RMCMD    "rm -f "
/**
 * \brief An alias of recursive rm command 
 */
#define RMRCMD    "rm -rf "
/**
 * \brief An alias of rmdir command 
 */
#define RMDIRCMD "rmdir "
/**
 * \brief An alias of tail command 
 */
#define TAILCMD  "tail -"
/**
 * \brief An alias of scp command 
 */
#define CPCMD    "scp -o Compression=yes -o StrictHostKeyChecking=no "
/**
 * \brief An alias of mv command 
 */
#define MVCMD    "mv "
/**
 * \brief An alias of simple ls command 
 */
#define LSCMD    "ls  "
/**
 * \brief An alias of ls -a command 
 */
#define LSACMD  "ls -a "
/**
 * \brief An alias of ls -l command 
 */
#define LSLCMD "ls -l "
/**
 * \brief An alias of ls -al command 
 */
#define LSALCMD  "ls -al "

#include "File.hpp"

/**
 *\brief  A class for file representation through SSH. 
 */
class SSHFile : public File {


  private:

    /**
     * \brief A flag to store the file information state
     */

    mutable bool upToDate;
    /**
     * \brief The ssh host
     */


    std::string sshHost;

    /**
     * \brief The ssh user
     */
    std::string sshUser;
    /**
     * \brief The ssh public key
     */
    std::string sshPublicKey;

    /**
     * \brief The ssh private key
     */
    std::string sshPrivateKey;
    /**
     * \brief The ssh password
     */
    std::string sshPassword;
    /**
     * \brief The ssh port
     */
    unsigned int sshPort;
    /**
     * \brief The ssh command path
     */
    std::string sshCommand;
    /**
     * \brief The scp command path
     */
    std::string scpCommand;
    /**
     * \brief the last error during file properties manipulation
     */
    mutable std::string merror;
  public:

    /*ù
     * \brief The dafault constructor
     */

    SSHFile();

    /**
     * \brief Another constructor by value
     * \param sessionServer   the session object server
     * \param path the path of the file
     * \param sshHost the ssh host 
     * \param sshUser the ssh user
     * \param sshPublicKey the ssh public key
     * \param sshPrivateKey the ssh private key
     * \param sshPassword the ssh password
     * \param sshPort the ssh port
     * \param sshCommand the ssh command path
     * \param scpCommand the scp command path
     */

    SSHFile(const SessionServer& sessionServer,
        const std::string& path,
        const std::string& sshHost,
        const std::string& sshUser,
        const std::string& sshPublicKey,
        const std::string& sshPrivateKey,
        const std::string& sshPassword,
        unsigned int sshPort,
        const std::string& sshCommand="/usr/bin/ssh",
        const std::string& scpCommand="/usr/bin/scp");

    /**
     * \brief Another constructor by reference
     * \param file the other file 
     */ 
    explicit  SSHFile(const SSHFile& file);

    /**
     * \brief The default destructor
     */

    virtual ~SSHFile();

    /**
     * \brief The assignment operator
     * \param file the object to assign
     * \return the new assigned file
     */
    SSHFile& operator=(const SSHFile& file);

    /**
     * \brief To check if the file information is up to date
     * \return true if the file is already updated, false otherwise
     */
    virtual bool isUpToDate() const;
    /**
     * \brief To get the file inode information
     */
    virtual void getInfos() const;
    /**
     * \brief Get the last command error message on the file
     * \return the last error message
     */

    std::string getErrorMsg() const;

    /**
     * \brief To update the group of the file
     * \param group the new group
     * \return 0 if the command succeeds, an error code otherwise
     */
    virtual int chgrp(const std::string& group);
    /**
     * \brief To update the new file access permissions
     * \param mode the new file access permissions
     * \return 0 if the command succeeds, an error code otherwise
     */
    virtual int chmod(const mode_t mode);

    /**
     * \brief To get the first lines of the file
     * \param options the options object 
     * \return the first lines of the file
     */
    virtual std::string head(const HeadOfFileOptions& options);

    /**
     * \brief To get the last lines of the file
     * \param options the options object 
     * \return the last lines of the file
     */
    virtual std::string tail(const TailOfFileOptions& options);
    /**
     * \brief To get the content of the file
     * \return the content of the file
     */
    virtual std::string getContent();
    /**
     * \brief To create a new file
     * \param mode the access permission of the file
     * \return 0 if the command succeeds, an error code otherwise
     */ 
    virtual int mkfile(const mode_t mode);
    /**
     * \brief To create a new directory
     * \param mode the access permission of the directory
     * \return 0 if the command succeeds, an error code otherwise
     */ 
    virtual int mkdir(const CreateDirOptions& options);
    /**
     * \brief To remove a file
     * \param isRecursive To remove directory
     * \return 0 if the command succeeds, an error code otherwise
     */ 
    virtual int rm(const FMS_Data::RmFileOptions& options);
    /**
     * \brief To remove an empty directory
     * \return 0 if the command succeeds, an error code otherwise
     */ 
    virtual int rmdir();
    /**
     * \brief To list the content of a directory
     * \param options the list options 
     * \return the content of the directory
     */ 
    virtual std::list<std::string> ls(const LsDirOptions& options) const;
    /**
     * \brief To copy the file 
     * \param path the copy destination
     * \param options the copy options  
     * \return 0 if the command succeeds, an error code otherwise
     */
    virtual int cp(const std::string& path, const CpFileOptions& options);
    /**
     * \brief To move the file 
     * \param path the move destination
     * \param options the move options  
     * \return 0 if the command succeeds, an error code otherwise
     */
    virtual int mv(const std::string& path, const CpFileOptions& options);

};


/**
 * \brief  A class to call command through SSH. 
 */


class SSHExec {
  private:
    /**
     * \brief The ssh command
     */
    std::string sshCommand;
    /**
     * \brief The scp command
     */

    std::string scpCommand;

    /**
     * \brief The ssh server host
     */

    std::string server;

    /**
     * \brief The ssh port
     */
    unsigned int sshPort;
    /**
     * \brief The ssh user name
     */

    std::string userName;
    /**
     * \brief The ssh password
     */

    std::string password;

    /**
     * \brief The ssh publicKey
     */
    std::string publicKey;

    /**
     * \brief The ssh privateKey
     */

    std::string privateKey;
    /**
     * \brief The last execution return value
     */
    mutable int lastExecStatus;

    /**
     * \brief The last execution process identifier
     */
    mutable int mprocessId;

  public:

    /**
     * \brief A constructor by value
     * \param sshCommand the ssh command path
     * \param scpCommand the scp command path
     * \param server the ssh host 
     * \param sshPort the ssh port
     * \param userName the ssh user
     * \param password the ssh password
     * \param publicKey the ssh public key
     * \param privateKey the ssh private key
     */
    SSHExec(const std::string& sshCommand,
        const std::string& scpCommand,
        const std::string& server,
        unsigned int sshPort,
        const std::string& userName,
        const std::string& password,
        const std::string& publicKey,
        const std::string& privateKey);
    /**
     * \brief Get the last execution return value
     * \return the last execution return value
     */ 
    const int& getLastExecStatus() const;
    /**
     * \brief Get the last execution process identifier
     * \return the last execution process identifier
     */
    const int& getProcessId() const;

    /**
     * \brief Update the last execution process identifier
     * \param processId the new execution process identifier
     */

    void setProcessId(const int& processId)const;
    /**
     * \brief perform a command through ssh
     * \param cmd the command to perform
     * \return the command output an error 
     */
    std::pair<std::string, std::string> exec(const std::string& cmd) const;
};

#endif
