#ifndef OPTION_VALUE_PROXY_H
#define OPTION_VALUE_PROXY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "SessionProxy.h"
#include "OptionValue.hpp"

class OptionValueProxy
{
 
public:
  OptionValueProxy(const UMS_Data::OptionValue& optionValue, const SessionProxy& session);
  int configureOption();
  int configureDefaultOption();
  UMS_Data::OptionValue getData();
  SessionProxy getSessionProxy();
  ~OptionValueProxy();

private:

  int setOptionValue(bool defaultValue=true);

  UMS_Data::OptionValue moptionValue;
  SessionProxy msessionProxy;
};
#endif
