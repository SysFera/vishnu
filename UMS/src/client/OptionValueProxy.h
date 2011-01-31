#ifndef OPTION_VALUE_PROXY_H
#define OPTION_VALUE_PROXY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "SessionProxy.h"
#include "UMS_Data/OptionValue.h"

class OptionValueProxy
{
private:
	UMS_Data::OptionValue optionValue;
	SessionProxy sessionProxy;
	SessionProxy sessionProxy;
 
public:
	void OptionValueProxy(UMS_Data::OptionValue optionValue, SessionProxy session);
	int configureOption();
	int configureDefaultOption();
	void ~OptionValueProxy();
	UMS_Data::OptionValue getData();
	SessionProxy getSessionProxy();
};
#endif
