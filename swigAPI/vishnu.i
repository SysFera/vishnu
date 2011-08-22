/* ----------------------------------------------------
 * LICENCE
 *
 * vishnu.i
 *
 * Header SWIG file for the VISHNU modules for all the data types
 */

%module VISHNU

 // Include for exception handling
%include "exception.i"


 // Added to make the mapping between std::exception and the python Exception class
 // Otherwise the VishnuException class inherits the _object class and the vishnu exception
 // Are not seen as real python exception (if not catched -> finally pops bad exception)
#ifdef SWIGPYTHON
%exceptionclass VishnuException;
#endif SWIGPYTHON

// this includes the typemaps for STL strings
%include "std_string.i"
%include "std_except.i"
%include "std_vector.i"
// fix compilation with GCC 4.6
%{
#include <cstddef>
%}


// Keep in separated files and before includes, all module need their own eobject in java
%include "ecore/EObject.hpp"
%include "ecorecpp/mapping/type_traits.hpp"
%include "ecorecpp/mapping/out_ptr.hpp"
%include "ecorecpp/mapping/EList.hpp"
%include "ecore_forward.hpp"


// All EMF includes (same as in UMS_Data.hpp)
%include "UMS_Data_forward.hpp"
%include "UMS_Data/ConnectOptions.hpp"
%include "UMS_Data/Session.hpp"
%include "UMS_Data/ListSessions.hpp"
%include "UMS_Data/ListSessionOptions.hpp"
%include "UMS_Data/User.hpp"
%include "UMS_Data/ListUsers.hpp"
%include "UMS_Data/Command.hpp"
%include "UMS_Data/ListCommands.hpp"
%include "UMS_Data/ListCmdOptions.hpp"
%include "UMS_Data/LocalAccount.hpp"
%include "UMS_Data/ListLocalAccounts.hpp"
%include "UMS_Data/ListLocalAccOptions.hpp"
%include "UMS_Data/Machine.hpp"
%include "UMS_Data/ListMachines.hpp"
%include "UMS_Data/ListMachineOptions.hpp"
%include "UMS_Data/Configuration.hpp"
%include "UMS_Data/OptionValue.hpp"
%include "UMS_Data/ListOptionsValues.hpp"
%include "UMS_Data/ListOptOptions.hpp"


// All EMF includes (same as in TMS_Data.hpp)
%include "TMS_Data_forward.hpp"
%include "TMS_Data/Job.hpp"
%include "TMS_Data/JobResult.hpp"
%include "TMS_Data/ListJobResults.hpp"
%include "TMS_Data/ListJobs.hpp"
%include "TMS_Data/ListJobsOptions.hpp"
%include "TMS_Data/ListProgression.hpp"
%include "TMS_Data/ListQueues.hpp"
%include "TMS_Data/ProgressOptions.hpp"
%include "TMS_Data/Progression.hpp"
%include "TMS_Data/Queue.hpp"
%include "TMS_Data/SubmitOptions.hpp"

// All EMF includes (same as in IMS_Data.hpp)
%include "IMS_Data_forward.hpp"
%include "IMS_Data/ExportOp.hpp"
%include "IMS_Data/CurMetricOp.hpp"
%include "IMS_Data/Metric.hpp"
%include "IMS_Data/ListMetric.hpp"
%include "IMS_Data/ListProcesses.hpp"
%include "IMS_Data/ListSysInfo.hpp"
%include "IMS_Data/ListThreshold.hpp"
%include "IMS_Data/MetricHistOp.hpp"
%include "IMS_Data/Process.hpp"
%include "IMS_Data/ProcessOp.hpp"
%include "IMS_Data/RestartOp.hpp"
%include "IMS_Data/SysInfoOp.hpp"
%include "IMS_Data/SystemInfo.hpp"
%include "IMS_Data/Threshold.hpp"
%include "IMS_Data/ThresholdOp.hpp"

// All EMF includes (same as in FMS_Data.hpp)
%include "FMS_Data_forward.hpp"
%include "FMS_Data/CpFileOptions.hpp"
%include "FMS_Data/FileStat.hpp"
%include "FMS_Data/FileStatList.hpp"
%include "FMS_Data/FileTransfer.hpp"
%include "FMS_Data/FileTransferList.hpp"
%include "FMS_Data/HeadOfFileOptions.hpp"
%include "FMS_Data/LsDirOptions.hpp"
%include "FMS_Data/LsTransferOptions.hpp"
%include "FMS_Data/StopTransferOptions.hpp"
%include "FMS_Data/StringList.hpp"
%include "FMS_Data/TailOfFileOptions.hpp"
%include "FMS_Data/RmFileOptions.hpp"

%{
#define SWIG_FILE_WITH_INIT
#include "UMS_Data.hpp"
#include "TMS_Data.hpp"
#include "IMS_Data.hpp"
#include "FMS_Data.hpp"
#include "VishnuException.hpp"
#include "UserException.hpp"
#include "SystemException.hpp"
#include "UMSVishnuException.hpp"
#include "TMSVishnuException.hpp"
#include "IMSVishnuException.hpp"
#include "FMSVishnuException.hpp"
#ifdef COMPILE_UMS
#include "api_ums.hpp"
#endif
#ifdef COMPILE_TMS
#include "api_tms.hpp"
#endif
#ifdef COMPILE_IMS
#include "api_ims.hpp"
#endif
#ifdef COMPILE_FMS
#include "api_fms.hpp"
#endif
%}


// TODO make copy of object when using the push_back() method of EList
// because this causes a segfault due to double free
%template(EListPtr) ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList< ::ecore::EObject > >;

%template(ESessionList) ::ecorecpp::mapping::EList<::UMS_Data::Session>;
%template(EUserList) ::ecorecpp::mapping::EList<::UMS_Data::User>;
%template(EMachineList) ::ecorecpp::mapping::EList<::UMS_Data::Machine>;
%template(ELocalAccList) ::ecorecpp::mapping::EList<::UMS_Data::LocalAccount>;
%template(ECommandList) ::ecorecpp::mapping::EList<::UMS_Data::Command>;
%template(EOptionList) ::ecorecpp::mapping::EList<::UMS_Data::OptionValue>;

%template(EJobList) ::ecorecpp::mapping::EList<::TMS_Data::Job>;
%template(EProgressionList) ::ecorecpp::mapping::EList<::TMS_Data::Progression>;
%template(EQueueList) ::ecorecpp::mapping::EList<::TMS_Data::Queue>;
%template(EJobResultList) ::ecorecpp::mapping::EList<::TMS_Data::JobResult>;

%template(EMetricList) ::ecorecpp::mapping::EList<::IMS_Data::Metric>;
%template(EProcessesList) ::ecorecpp::mapping::EList<::IMS_Data::Process>;
%template(ESysInfoList) ::ecorecpp::mapping::EList<::IMS_Data::SystemInfo>;
%template(EThresholdList) ::ecorecpp::mapping::EList<::IMS_Data::Threshold>;

%template(EFileTransferList) ::ecorecpp::mapping::EList<::FMS_Data::FileTransfer>;
%template(EFileStatList) ::ecorecpp::mapping::EList<::FMS_Data::FileStat>;
%template(EStringList) std::vector< ::ecore::EString >;

//  ***************************** UMS MODULE  *****************************

#ifdef SWIGPYTHON

// Remove output parameters from the command
//%typemap(in, numinputs=0) UMS_Data::Session& session(UMS_Data::Session temp) {
//  $1 = &temp;
//}
//
//
//// Add the output parameters to the result
//%typemap(argout) UMS_Data::Session& session {
//  PyObject *o = PyString_FromString($1->getSessionKey().c_str());
//  $result = SWIG_Python_AppendOutput($result, o);
//}


%typemap(in, numinputs=0) std::string& sshPublicKey (std::string temp) {
  $1 = &temp;
}

%typemap(argout) std::string& sshPublicKey {
  PyObject *o = PyString_FromString($1->c_str());
  std::cout << "Valeur de l'objet" << $1->c_str() << std::endl;
  $result = SWIG_Python_AppendOutput($result, o);
}


// Exception rule for user exception
%typemap (throws) UserException{
    SWIG_Python_Raise(SWIG_NewPointerObj((new UMSVishnuException(static_cast< const UMSVishnuException& >(_e))),SWIGTYPE_p_UserException,SWIG_POINTER_OWN), "UserException", SWIGTYPE_p_UserException); SWIG_fail;
}

#endif

#ifdef SWIGJAVA
%include "various.i"
// Use a specific typemap for strings passed by reference
%include "string.i"
%apply std::string &INOUT { std::string& tmpPassword };
%apply std::string &INOUT { std::string& sshPublicKey };

// Exception rule for system exception
%typemap (throws) SystemException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/vishnu/internal/InternalUMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Exception rule for user exception
%typemap (throws) UserException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/vishnu/internal/InternalUMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Add throw to method declaration
%javaexception ("InternalUMSException") { $action }

#endif

// Remove the parameters of vishnuInitialize
%typemap(in, numinputs=0) int argc {
  $1 = 0;
}
%typemap(in, numinputs=0) char** argv {
  $1 = NULL;
}

#ifdef COMPILE_UMS
%include "api_ums.hpp"
#endif

 // Added to make the mapping between std::exception and the python Exception class
 // Otherwise the VishnuException class inherits the _object class and the vishnu exception
 // Are not seen as real python exception (if not catched -> finally pops bad exception)
 // Keep there and not before, otherwize non exception classes also inherith from Exception and not _object in python
//#ifdef SWIGPYTHON
//%clearexceptionclass;
//%exceptionclass;
//#endif SWIGPYTHON


#ifdef SWIGPYTHON
%include "VishnuException.hpp"
%include "UserException.hpp"
%include "SystemException.hpp"
%include "UMSVishnuException.hpp"
%include "TMSVishnuException.hpp"
%include "IMSVishnuException.hpp"
%include "FMSVishnuException.hpp"
#endif

// ***************************** TMS MODULE  *****************************

#ifdef SWIGJAVA

// Exception rule for system exception
%typemap (throws) SystemException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/vishnu/internal/InternalTMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Exception rule for user exception
%typemap (throws) UserException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/vishnu/internal/InternalTMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }
// Exception rule for user exception
%typemap (throws) TMSVishnuException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/vishnu/internal/InternalTMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }
// Exception rule for user exception
%typemap (throws) UMSVishnuException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/vishnu/internal/InternalTMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Add throw to method declaration
%javaexception ("InternalTMSException") { $action }

#endif


#ifdef COMPILE_TMS
%include "api_tms.hpp"
#endif

//  ***************************** IMS MODULE  *****************************


#ifdef SWIGPYTHON
// Remove output parameters from the command
%typemap(in, numinputs=0) int& freq(int temp) {
  $1 = &temp;
}


%typemap(argout) int& freq {
  PyObject *o = PyInt_FromLong(*$1);
  $result = SWIG_Python_AppendOutput($result, o);
 }
#endif

#ifdef SWIGJAVA
%include "typemaps.i"
%apply int *OUTPUT { int &freq };


// Exception rule for system exception
%typemap (throws) SystemException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/vishnu/internal/InternalIMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Exception rule for user exception
%typemap (throws) UserException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/vishnu/internal/InternalIMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Exception rule for information exception
%typemap (throws) IMSVishnuException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/vishnu/internal/InternalIMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }
// Exception rule for user exception
%typemap (throws) UMSVishnuException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/vishnu/internal/InternalIMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Add throw to method declaration
%javaexception ("InternalIMSException") { $action }

#endif

#ifdef COMPILE_IMS
%include "api_ims.hpp"
#endif

//  ***************************** FMS MODULE  *****************************

typedef unsigned int mode_t;

#ifdef SWIGPYTHON
%typemap(in, numinputs=0) std::string& contentOfFile (std::string temp) {
  $1 = &temp;
}

%typemap(argout) std::string& contentOfFile {
  PyObject *o = PyString_FromString($1->c_str());
  $result = SWIG_Python_AppendOutput($result, o);
}

#endif


#ifdef SWIGJAVA

%apply std::string &INOUT { std::string& contentOfFile };

// Exception rule for system exception
%typemap (throws) SystemException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/vishnu/internal/InternalFMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Exception rule for user exception
%typemap (throws) UserException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/vishnu/internal/InternalFMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }
// Exception rule for user exception
%typemap (throws) FMSVishnuException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/vishnu/internal/InternalFMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }
// Exception rule for user exception
%typemap (throws) UMSVishnuException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/vishnu/internal/InternalFMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Add throw to method declaration
%javaexception ("InternalFMSException") { $action }

#endif

#ifdef COMPILE_FMS
%include "api_fms.hpp"
#endif
