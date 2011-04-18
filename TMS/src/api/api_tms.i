/* ----------------------------------------------------
 * LICENCE
 *
 * api_tms.i
 *
 * SWIG file for the VISHNU TMS API
 */

%module VISHNU_TMS

%{
#define SWIG_FILE_WITH_INIT
#include "TMS_Data.hpp"
#include "api_tms.hpp"
%}

 // Include for exception handling
%include "exception.i"

// this includes the typemaps for STL strings
%include "std_string.i"
%include "std_except.i"


// this includes the required type declarations for EMF4CPP
// WARNING: some may be missing!
%include "ecore/EObject.hpp"
%include "ecorecpp/mapping/type_traits.hpp"
%include "ecorecpp/mapping/out_ptr.hpp"
%include "ecorecpp/mapping/EList.hpp"
%include "ecore_forward.hpp"

// All EMF includes (same as in UMS_Data.hpp)
%include "TMS_Data_forward.hpp"
%include "TMS_Data/Job.hpp"
%include "TMS_Data/JobResult.hpp"
%include "TMS_Data/ListJobResult.hpp"
%include "TMS_Data/ListJobs.hpp"
%include "TMS_Data/ListJobsOptions.hpp"
%include "TMS_Data/ListProgression.hpp"
%include "TMS_Data/ListQueues.hpp"
%include "TMS_Data/ProgressOptions.hpp"
%include "TMS_Data/Queue.hpp"
%include "TMS_Data/SubmitOptions.hpp"

// TODO make copy of object when using the push_back() method of EList
// because this causes a segfault due to double free

// Instantiate the template for all lists
// the templates used within the list template must be instantiated first
%template(EListPtr) ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList< ::ecore::EObject > >;
%template(EJobList) ::ecorecpp::mapping::EList<::UMS_Data::Job>;
%template(EProgressionList) ::ecorecpp::mapping::EList<::UMS_Data::Progression>;
%template(EQueueList) ::ecorecpp::mapping::EList<::UMS_Data::Queue>;
%template(EJobResultList) ::ecorecpp::mapping::EList<::UMS_Data::JobResult>;

#ifdef SWIGPYTHON
// Exception rule for System exception
%typemap (throws) SystemException{
    SWIG_Python_Raise(SWIG_NewPointerObj((new SystemException(static_cast< const SystemException& >(_e))),SWIGTYPE_p_SystemException,SWIG_POINTER_OWN), "SystemException", SWIGTYPE_p_SystemException); SWIG_fail;
// Exception rule for User exception
%typemap (throws) UMSVishnuException{
    SWIG_Python_Raise(SWIG_NewPointerObj((new UserException(static_cast< const UserException& >(_e))),SWIGTYPE_p_UserException,SWIG_POINTER_OWN), "UserException", SWIGTYPE_p_UserException); SWIG_fail;
 }
// Exception rule for UMS exception
%typemap (throws) UMSVishnuException{
    SWIG_Python_Raise(SWIG_NewPointerObj((new UMSVishnuException(static_cast< const UMSVishnuException& >(_e))),SWIGTYPE_p_UserException,SWIG_POINTER_OWN), "UserException", SWIGTYPE_p_UserException); SWIG_fail;
 }
// Exception rule for TMS exception
%typemap (throws) TMSVishnuException{
    SWIG_Python_Raise(SWIG_NewPointerObj((new TMSVishnuException(static_cast< const TMSVishnuException& >(_e))),SWIGTYPE_p_UserException,SWIG_POINTER_OWN), "UserException", SWIGTYPE_p_UserException); SWIG_fail;
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
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/tms/internal/InternalTMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Exception rule for user exception
%typemap (throws) UserException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/tms/internal/InternalTMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }
// Exception rule for user exception
%typemap (throws) TMSVishnuException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/tms/internal/InternalTMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }
// Exception rule for user exception
%typemap (throws) UMSVishnuException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/tms/internal/InternalTMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Add throw to method declaration
%javaexception ("InternalTMSException") { $action }

#endif

// Remove the parameters of vishnuInitialize
%typemap(in, numinputs=0) int argc {
  $1 = 0;
}
%typemap(in, numinputs=0) char** argv {
  $1 = NULL;
}

%include "api_tms.hpp"
#ifdef SWIGPYTHON
%include "VishnuException.hpp"
%include "UserException.hpp"
%include "SystemException.hpp"
%include "TMSVishnuException.hpp"
%include "UMSVishnuException.hpp"
#endif
