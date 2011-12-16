/* ----------------------------------------------------
 * LICENCE
 *
 * api_fms.i
 *
 * SWIG file for the VISHNU FMS API
 */

%module VISHNU_FMS

%{
#define SWIG_FILE_FMS_WITH_INIT
#include "FMS_Data.hpp"
#include "api_fms.hpp"
%}

%import "api_ums.i"

// Include for exception handling
%include "exception.i"

// this includes the typemaps for STL strings
%include "std_string.i"
%include "std_except.i"
%include "std_vector.i"

// this includes the required type declarations for EMF4CPP
// WARNING: some may be missing!
%include "ecore/EObject.hpp"
%include "ecorecpp/mapping/type_traits.hpp"
%include "ecorecpp/mapping/out_ptr.hpp"
%include "ecorecpp/mapping/EList.hpp"
%include "ecore_forward.hpp"

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
%include "FMS_Data/TailOfFileOptions.hpp"


// Instantiate the template for all lists
// the templates used within the list template must be instantiated first
//%template(EListPtr) ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList< ::ecore::EObject > >;
%template(EFileTransferList) ::ecorecpp::mapping::EList<::FMS_Data::FileTransfer>;
%template(EFileStatList) ::ecorecpp::mapping::EList<::FMS_Data::FileStat>;
%template(EStringList) std::vector< ::ecore::EString >;

#ifdef SWIGPYTHON

// Add the output parameters to the result
%typemap(argout) std::string& info {
  PyObject *o = PyString_FromString($1);
  $result = SWIG_Python_AppendOutput($result, o);
}

#endif


#ifdef SWIGJAVA
%include "various.i"
// Use a specific typemap for strings passed by reference
%include "string.i"

// Exception rule for system exception
%typemap (throws) SystemException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/fms/internal/InternalFMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Exception rule for user exception
%typemap (throws) UserException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/fms/internal/InternalFMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }
// Exception rule for user exception
%typemap (throws) FMSVishnuException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/fms/internal/InternalFMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }
// Exception rule for user exception
%typemap (throws) UMSVishnuException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/fms/internal/InternalFMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Add throw to method declaration
%javaexception ("InternalFMSException") { $action }

#endif

// Remove the parameters of vishnuInitialize
%typemap(in, numinputs=0) int argc {
  $1 = 0;
}
%typemap(in, numinputs=0) char** argv {
  $1 = NULL;
}

%include "api_fms.hpp"
#ifdef SWIGPYTHON
%include "FMSVishnuException.hpp"
#endif
