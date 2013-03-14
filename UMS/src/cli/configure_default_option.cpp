#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr

#include "Options.hpp"                  // for Options
#include "api_ums.hpp"                  // for configureDefaultOption
#include "configureOptionUtils.hpp"     // for commonConfigure

using namespace std;
using namespace vishnu;

int
main (int ac, char* av[]) {
  boost::shared_ptr<Options> opt(new Options(av[0])) ;

  return  commonConfigure(opt,  ac,  av,&configureDefaultOption);
}  // end of main
