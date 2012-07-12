/**
 * \file Env.hpp
 * \brief This file contains the VISHNU Env class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date December 2011
 */

#ifndef _ENV_H
#define _ENV_H

#include <sstream>
#include "utilVishnu.hpp"

/**
 * \class Env
 * \brief Env class implementation
 */
class Env
{

public:
	/**
	 * \param batchType The batch scheduler type
	 * \brief Constructor
	 */
	Env(const BatchType& batchType);

	/**
	 * \brief Function to replace all occurences in a string
	 * \param scriptContent The string to modify
	 * \param oldValue The value to replace
	 * \param newValue The new value
	 */
	void replaceAllOccurences(std::string& scriptContent,
			const std::string& oldValue,
			const std::string& newValue);

	/**
	 * \brief Function to replace some environment variables in a string
	 * \param scriptContent The string content to modify
	 */
	void replaceEnvVariables(std::string& scriptContent);

	/**
	 * \brief function to set parameters appearing in a script
	 * \param scriptContent The string to modify
	 * \param \param params a list of parameters in the form of PARAM1=value1  PARAM2=value2 ...
	 */
	void setParams(std::string& scriptContent, const std::string& params) ;

	/**
	 * \brief Function to set environment variables accordinf to parameters
	 * \param params a list of parameters in the form of PARAM1=value1  PARAM2=value2 ...
	 */
	static void setParamsEnvVars(const std::string& params) ;

private:

	/**
	 * \brief The type of the batch scheduler
	 */
	BatchType mbatchType;

};

#endif
