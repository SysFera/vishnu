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

// TODO make copy of object when using the push_back() method of EList
// because this causes a segfault due to double free

// Instantiate the template for all lists
// the templates used within the list template must be instantiated first
%template(EListPtr) ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList< ::ecore::EObject > >;

#ifdef SWIGPYTHON


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
#endif
