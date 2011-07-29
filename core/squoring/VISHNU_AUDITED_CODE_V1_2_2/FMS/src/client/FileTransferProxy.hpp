
/**
 * \file FileTransferProxy.hpp
 * \brief Contains a declaration of all File transfer services
 */

#ifndef FILETRANSFERPROXY
#define FILETRANSFERPROXY

#include "FMS_Data_forward.hpp"
#include "FMS_Data.hpp"

/**
 * \brief A proxy class to handle file transfer
 */

class FileTransferProxy{

  public:
    /**
     * \brief A constructor with one parameter
     * \param sessionKey The session key
     */ 
    explicit FileTransferProxy(const std::string& sessionKey);
    /**
     * \brief Another constructor with parameters
     * \param sessionKey The session key
     * \param srcFilePath The source file path
     * \param destFilePath The destination file path
     */ 
    FileTransferProxy(const std::string& sessionKey,
        const std::string& srcFilePath,
        const std::string& destFilePath);
    /**
     * \brief Add a new copy file thread 
     * \param options the copy options
     * \return 0 if the function succeeds or an error code otherwise
     */
    int addCpThread(const FMS_Data::CpFileOptions& options);

    /**
     * \brief Add a new move file thread 
     * \param options the move file options
     * \return 0 if the function succeeds or an error code otherwise
     */
    int addMvThread(const FMS_Data::CpFileOptions& options);

    /**
     * \brief Add a new asynchronous copy file thread 
     * \param options the copy options
     * \return 0 if the function succeeds or an error code otherwise
     */
    int addCpAsyncThread(const FMS_Data::CpFileOptions& options);

    /**
     * \brief Add a new asynchronous move file thread 
     * \param options the move options
     * \return 0 if the function succeeds or an error code otherwise
     */
    int addMvAsyncThread(const FMS_Data::CpFileOptions& options);
    /**
     * \brief Stop a file transfer
     * \param options The stop options
     * \return 0 if the function succeeds or an error code otherwise
     */


    int stopThread(const FMS_Data::StopTransferOptions& options);
    /**
     * \brief Get The file transfer information
     * \return The file transfer information
     */
    const FMS_Data::FileTransfer& getFileTransfer() const;
    /**
     * \brief The default destructor
     */
    ~FileTransferProxy(); 

  private:
    /**
     * \brief The session key
     */
    std::string msessionKey;
    /**
     * \brief The source file path
     */
    std::string msrcFilePath;
    /**
     * \brief The destination file path
     */
    std::string mdestFilePath;
    /**
     * \brief The file transfer information
     */
    FMS_Data::FileTransfer mtransferInfo;
};

#endif
