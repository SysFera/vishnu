/* ----------------------------------------------------
 * LICENCE
 *
 * api_ims.i
 *
 * SWIG file for the VISHNU IMS API
 */

%module VISHNU_IMS

%{
#define SWIG_FILE_WITH_INIT
#include "IMS_Data.hpp"
#include "api_ims.hpp"
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

// TODO make copy of object when using the push_back() method of EList
// because this causes a segfault due to double free

// Instantiate the template for all lists
// the templates used within the list template must be instantiated first
%template(EListPtr) ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList< ::ecore::EObject > >;
%template(EMetricList) ::ecorecpp::mapping::EList<::IMS_Data::Metric>;
%template(EProcessesList) ::ecorecpp::mapping::EList<::IMS_Data::Process>;
%template(ESysInfoList) ::ecorecpp::mapping::EList<::IMS_Data::SystemInfo>;
%template(EThresholdList) ::ecorecpp::mapping::EList<::IMS_Data::Threshold>;

#ifdef SWIGPYTHON

// Remove output parameters from the command

// Add the output parameters to the result

#endif

#ifdef SWIGJAVA
%include "various.i"
// Use a specific typemap for strings passed by reference
%include "string.i"

// Exception rule for system exception
%typemap (throws) SystemException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/ims/internal/InternalIMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Exception rule for user exception
%typemap (throws) UserException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/ims/internal/InternalIMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Exception rule for information exception
%typemap (throws) IMSVishnuException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/ims/internal/InternalIMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }
// Exception rule for user exception
%typemap (throws) UMSVishnuException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/ims/internal/InternalIMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Add throw to method declaration
%javaexception ("InternalIMSException") { $action }

#endif

// Remove the parameters of vishnuInitialize
%typemap(in, numinputs=0) int argc {
  $1 = 0;
}
%typemap(in, numinputs=0) char** argv {
  $1 = NULL;
}

%include "api_ims.hpp"
#ifdef SWIGPYTHON
%include "VishnuException.hpp"
%include "UserException.hpp"
%include "SystemException.hpp"
%include "UMSVishnuException.hpp"
%include "IMSVishnuException.hpp"
#endif
