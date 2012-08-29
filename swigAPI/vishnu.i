/* ----------------------------------------------------
 * LICENCE
 *
 * vishnu.i
 *
 * Header SWIG file for the VISHNU modules for all the data types
 */

%module VISHNU
// fix compilation with GCC 4.6
%{
#include <cstddef>
%}

%feature("kwargs","1");
%feature("compactdefaultargs");
%feature("autodoc","1");

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
#ifdef SWIGPYTHON
%include "pyname_compat.i"
#endif SWIGPYTHON

// Keep in separated files and before includes, all module need their own eobject in java
%include "ecore/EObject.hpp"
%include "ecorecpp/mapping/type_traits.hpp"
%include "ecorecpp/mapping/out_ptr.hpp"
%include "ecore_forward.hpp"

// All EMF includes (same as in UMS_Data.hpp)
%include "UMS_Data_forward.hpp"
%include "UMS_Data/ConnectOptions.hpp"
%include "UMS_Data/Session.hpp"
%include "UMS_Data/ListUsersOptions.hpp"
%include "UMS_Data/ListSessionOptions.hpp"
%include "UMS_Data/User.hpp"
%include "UMS_Data/Command.hpp"
%include "UMS_Data/ListCmdOptions.hpp"
%include "UMS_Data/LocalAccount.hpp"
%include "UMS_Data/ListLocalAccOptions.hpp"
%include "UMS_Data/Machine.hpp"
%include "UMS_Data/ListMachineOptions.hpp"
%include "UMS_Data/Configuration.hpp"
%include "UMS_Data/OptionValue.hpp"
%include "UMS_Data/ListOptOptions.hpp"
%include "UMS_Data/AuthAccount.hpp"
%include "UMS_Data/AuthSystem.hpp"
%include "UMS_Data/ListAuthSysOptions.hpp"
%include "UMS_Data/ListAuthAccOptions.hpp"


// All EMF includes (same as in TMS_Data.hpp)
%include "TMS_Data_forward.hpp"
%include "TMS_Data/Job.hpp"
%include "TMS_Data/JobResult.hpp"
%include "TMS_Data/ListJobsOptions.hpp"
%include "TMS_Data/ProgressOptions.hpp"
%include "TMS_Data/Progression.hpp"
%include "TMS_Data/Queue.hpp"
%include "TMS_Data/SubmitOptions.hpp"
%include "TMS_Data/LoadCriterion.hpp"
%include "TMS_Data/Work.hpp"
%include "TMS_Data/AddWorkOptions.hpp"

// All EMF includes (same as in IMS_Data.hpp)
%include "IMS_Data_forward.hpp"
%include "IMS_Data/ExportOp.hpp"
%include "IMS_Data/CurMetricOp.hpp"
%include "IMS_Data/Metric.hpp"
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
%include "FMS_Data/FileTransfer.hpp"
%include "FMS_Data/HeadOfFileOptions.hpp"
%include "FMS_Data/LsDirOptions.hpp"
%include "FMS_Data/LsTransferOptions.hpp"
%include "FMS_Data/StopTransferOptions.hpp"
%include "FMS_Data/TailOfFileOptions.hpp"
%include "FMS_Data/RmFileOptions.hpp"
%include "FMS_Data/CreateDirOptions.hpp"
%include "FMS_Data/DirEntry.hpp"

#ifdef SWIGJAVA
%include "ecorecpp/mapping/EList.hpp"
%include "UMS_Data/ListSessions.hpp"
%include "UMS_Data/ListUsers.hpp"
%include "UMS_Data/ListUsersOptions.hpp"
%include "UMS_Data/ListUsers.hpp"
%include "UMS_Data/ListCommands.hpp"
%include "UMS_Data/ListLocalAccounts.hpp"
%include "UMS_Data/ListMachines.hpp"
%include "UMS_Data/ListOptionsValues.hpp"
%include "TMS_Data/ListJobResults.hpp"
%include "TMS_Data/ListJobs.hpp"
%include "TMS_Data/ListProgression.hpp"
%include "TMS_Data/ListQueues.hpp"
%include "IMS_Data/ListMetric.hpp"
%include "IMS_Data/ListProcesses.hpp"
%include "IMS_Data/ListSysInfo.hpp"
%include "IMS_Data/ListThreshold.hpp"
%include "FMS_Data/FileTransferList.hpp"
%include "FMS_Data/DirEntryList.hpp"
%include "UMS_Data/ListAuthSystems.hpp"
%include "UMS_Data/ListAuthAccounts.hpp"

%template(EListPtr) ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList< ::ecore::EObject > >; 
%template(ESessionList) ::ecorecpp::mapping::EList<::UMS_Data::Session>;
%template(EUserList) ::ecorecpp::mapping::EList<::UMS_Data::User>;
%template(EMachineList) ::ecorecpp::mapping::EList<::UMS_Data::Machine>;
%template(ELocalAccList) ::ecorecpp::mapping::EList<::UMS_Data::LocalAccount>;
%template(ECommandList) ::ecorecpp::mapping::EList<::UMS_Data::Command>;
%template(EOptionList) ::ecorecpp::mapping::EList<::UMS_Data::OptionValue>;
%template(EAuthSystemList) ::ecorecpp::mapping::EList<::UMS_Data::AuthSystem>;
%template(EAuthAccountList) ::ecorecpp::mapping::EList<::UMS_Data::AuthAccount>;
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
%template(EDirEntryList) ::ecorecpp::mapping::EList<::FMS_Data::DirEntry>;
#endif


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



//  ***************************** UMS MODULE  *****************************

#ifdef SWIGPYTHON

// Python ne gere absolument pas les overloads
%rename (connect_m) connect(UMS_Data::ListUsers& users,
                            UMS_Data::Session& session,
                            const UMS_Data::ConnectOptions& connectOpt);


// Macro to map emf lists and python list in both ways (python->C++ and C++->python)
// type : type of the contained object (User, Session, ...)
// cont : C++ namespace (UMS_Data, IMS_Data, ...)
// getter : The C++ getter function name to get the iterative list from the global class (getSessions, getUsers, ...)
// list : The list class name (ListUsers, ListSessions, ...)
// name : name of the param in out, ex listUsers
// strtype : type in string, ex "UMS_Data::User"
// facto : factory name, ex UMS_DataFactory
// create : function to create the type, ex createUser
%define EMFTYPE(type,cont,getter,list,name,strtype, facto, create)

 // Python list -> c++ list
 // create a local variable temp to work

%typemap(in) cont::list& users(cont::list temp){
  // Check it is a list
  if (PySequence_Check($input)) {
    int size = PySequence_Size($input);
    
    // Instanciating a list object
    cont::facto * ecoreFactory = cont::facto::_instance();
    
    // For each item of the list
    for (int i = 0; i < size ; ++i) {
      // Extract the python object
      PyObject *o = PySequence_GetItem($input,i);
      // Instanciating an object to copy each item of the C++ list (because emf destroys its objects at the end)
      cont::type* test = ecoreFactory->create();
      cont::type* copy = ecoreFactory->create();

      SWIG_ConvertPtr(o, (void **)&test, $descriptor(cont::type*),1);
      *copy = *test;
      temp.getter().push_back(copy);
      // Handle reference on object
      Py_DECREF(o);
    }
  } else {
    PyErr_SetString(PyExc_TypeError, "You must give a python list in input");
  }
  // Assign reference to the new created list
  $1 = &temp;
 }

// c++ list -> python list
%typemap(argout) cont::list& name{
  // Creating the out tupple (res code + list)
  PyObject * tup = PyTuple_New(2);
  // Setting the result code of the function in the tupple
  PyTuple_SetItem(tup, 0, $result);
  // Creating the list to receive the object
  PyObject *mylist = PyList_New($1->getter().size());
  // Instanciating a list object
  cont::facto * ecoreFactory = cont::facto::_instance();
  // For each item of the list
  for (int i = 0; i < $1->getter().size() ; ++i) {
    // Getting the corresponding swig type    
    swig_type_info* swig_type = SWIG_TypeQuery(string(string(strtype)+string(" *")).c_str());

    // Instanciating an object to copy each item of the C++ list (because emf destroys its objects at the end)
    cont::type* object = ecoreFactory->create();
    // Copying the object
    *object = *((*(&$1))->getter().get(i));
    // Convert it to a pyobject
    PyObject *o = SWIG_Python_NewPointerObj((void *)object, swig_type, 1);
 
    // Add it to the list
    PyList_SetItem(mylist,i,o);
  }
  // Add the list to the output tuples
  PyTuple_SetItem(tup, 1, mylist);
  // The result is the tupple
  $result = tup;
 }
// Remove the list object from the input param
%typemap(in, numinputs=0) cont::list& name (cont::list temp) {
  $1 = &temp;
 }

%enddef // End of macro handling the lists

// Mapping lists for ums module

EMFTYPE(User,UMS_Data,getUsers,ListUsers, listUsers, "UMS_Data::User", UMS_DataFactory, createUser)
EMFTYPE(Session,UMS_Data,getSessions,ListSessions, listSession, "UMS_Data::Session", UMS_DataFactory, createSession)
EMFTYPE(Machine,UMS_Data,getMachines,ListMachines, listMachine, "UMS_Data::Machine", UMS_DataFactory, createMachine)
EMFTYPE(LocalAccount,UMS_Data,getAccounts,ListLocalAccounts, listLocalAcc, "UMS_Data::LocalAccount", UMS_DataFactory, createLocalAccount)
EMFTYPE(Command,UMS_Data,getCommands,ListCommands, listCommands, "UMS_Data::Command", UMS_DataFactory, createCommand)
EMFTYPE(OptionValue,UMS_Data,getOptionValues,ListOptionsValues, listOptValues, "UMS_Data::OptionValue", UMS_DataFactory, createOptionValue)
EMFTYPE(AuthSystem,UMS_Data,getAuthSystems,ListAuthSystems, listAuthSys, "UMS_Data::AuthSystem", UMS_DataFactory, createAuthSystem)
EMFTYPE(AuthAccount,UMS_Data,getAuthAccounts,ListAuthAccounts, listAuthAccounts, "UMS_Data::AuthAccount", UMS_DataFactory, createAuthAccount)

// Mapping lists for tms module
EMFTYPE(Job,TMS_Data,getJobs,ListJobs, listOfJobs, "TMS_Data::Job", TMS_DataFactory, createJob)
EMFTYPE(Progression,TMS_Data,getProgress,ListProgression, listOfProgress, "TMS_Data::Progression", TMS_DataFactory, createProgression)
EMFTYPE(Queue,TMS_Data,getQueues,ListQueues, listofQueues, "TMS_Data::Queue", TMS_DataFactory, createQueue)
EMFTYPE(JobResult,TMS_Data,getResults,ListJobResults, listOfResults, "TMS_Data::JobResult", TMS_DataFactory, createJobResult)


// Mapping lists for ims module

EMFTYPE(Metric,IMS_Data,getMetric,ListMetric, val, "IMS_Data::Metric", IMS_DataFactory, createMetric)
EMFTYPE(Metric,IMS_Data,getMetric,ListMetric, list, "IMS_Data::Metric", IMS_DataFactory, createMetric)
EMFTYPE(Process,IMS_Data,getProcess,ListProcesses, list, "IMS_Data::Process", IMS_DataFactory, createProcess)
EMFTYPE(SystemInfo,IMS_Data,getSysInfo,ListSysInfo, list, "IMS_Data::SystemInfo", IMS_DataFactory, createSystemInfo)
EMFTYPE(Threshold,IMS_Data,getThreshold,ListThreshold, list, "IMS_Data::Threshold", IMS_DataFactory, createThreshold)

// Mapping lists for fms module

EMFTYPE(DirEntry,FMS_Data,getDirEntries,DirEntryList, dirContent, "FMS_Data::DirEntry", FMS_DataFactory, createDirEntry)
EMFTYPE(FileTransfer,FMS_Data,getFileTransfers,FileTransferList, fileTransferList, "FMS_Data::FileTransfer", FMS_DataFactory, createFileTransfer)

 //

// Removing public key from input and setting as output
%typemap(in, numinputs=0) std::string& sshPublicKey (std::string temp) {
  $1 = &temp;
}

%typemap(argout) std::string& sshPublicKey {
  PyObject *o = PyString_FromString($1->c_str());
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
