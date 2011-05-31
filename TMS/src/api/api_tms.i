/* ----------------------------------------------------
 * LICENCE
 *
 * api_tms.i
 *
 * SWIG file for the VISHNU TMS API
 */

%module VISHNU_TMS

%{
#define SWIG_FILE_TMS_WITH_INIT
#include "TMS_Data.hpp"
#include "api_tms.hpp"
%}

// Keep in separated files and before includes, all module need their own eobject in java
%include "ecore/EObject.hpp"
%include "ecorecpp/mapping/type_traits.hpp"
%include "ecorecpp/mapping/out_ptr.hpp"
%include "ecorecpp/mapping/EList.hpp"
%include "ecore_forward.hpp"

%import "api_ums_include.i"


// All EMF includes (same as in UMS_Data.hpp)
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



// Instantiate the template for all lists
// the templates used within the list template must be instantiated first
 //%template(EListPtr) ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList< ::ecore::EObject > >;
%template(EListPtr) ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList< ::ecore::EObject > >;
%template(EJobList) ::ecorecpp::mapping::EList<::TMS_Data::Job>;
%template(EProgressionList) ::ecorecpp::mapping::EList<::TMS_Data::Progression>;
%template(EQueueList) ::ecorecpp::mapping::EList<::TMS_Data::Queue>;
%template(EJobResultList) ::ecorecpp::mapping::EList<::TMS_Data::JobResult>;

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
%include "TMSVishnuException.hpp"
#endif
