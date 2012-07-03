/**
 * \file SOCISession.cpp
 * \brief This file implements the SOCI session.
 * \author Jean-Baptiste Ghoul (jean-baptiste.ghoul@sysfera.com)
 * \date 03/07/12
 */

#include "SOCISession.hpp"


/*
 * \brief default constructor
 */
SOCISession::SOCISession()
	: msession(NULL)
{}
/*
 * \brief copy constructor
 */
SOCISession::SOCISession(const SOCISession & s)
	: msession(s.msession),pool_position(s.pool_position)
{}
/*
 * \brief constructor with existing soci session
 */
SOCISession::SOCISession(soci::session* asession, size_t pos)
	: msession(asession),pool_position(pos)
{}

SOCISession::~SOCISession()
{
}
