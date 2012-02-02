/**
  * \file LDAPProxy.hpp
  * \brief This file contains the VISHNU LDAPProxy class.
  * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
  * \date March 2011
  */

#ifndef _LDAP_PROXY_H
#define _LDAP_PROXY_H

#include <iostream>
extern "C" {
# include <ldap.h>
# include <lber.h>
}

static const int desired_version = LDAP_VERSION3;
using namespace std;

/**
 * \class LDAPProxy
 * \brief LDAPProxy class implementation
 */
class LDAPProxy {

  public:

  /**
  * \param uri The LDAP uri by of the form host:port
  * \param distName the distinguished name of the entry used to bind
  * \param authMechanism the authentication method used to bind
  * \param credential the credential of the distName name used for authentication
  * \param serverCtrls A list of LDAP server controls
  * \param clientCtrls A list of LDAP client controls
  * \brief Constructor
  */
  explicit LDAPProxy(const string& uri,
                     const string& distName,
                     const string& authMechanism,
                     const string& credential,
                     LDAPControl* serverCtrls=NULL,
                     LDAPControl* clientCtrls=NULL
                    );

  /**
  * \brief Function to initialize a connection on a LDAP server
  */
  int
  connectLDAP();

  /**
  * \param base specifies the DN of the entry at which to start the search
  * \param filter specifies a string representation of the filter to apply in the search
  * \param scope specifies the scope of the search. It can be LDAP_SCOPE_BASE
  * (to search the object itself), or LDAP_SCOPE_ONELEVEL (to search the object's immediate children),
  * or LDAP_SCOPE_SUBTREE (to search the object and all its descendents)
  * \param attrs specifies a null-terminated array of character string attribute types
  * to return from entries that match filter. If NULL is specified, all attributes will be returned.
  * \param attrsonly Specifies attribute information. Attrsonly should be set to 1.
  * to request attribute types only. Set to 0 to request both attributes types and attribute values.
  * \param serverCtrls A list of LDAP server controls
  * \param clientCtrls A list of LDAP client controls
  * \param timeout The local search timeout value
  * and the operation time limit that is sent to the server within the search request.
  * \param sizelimit specifies the maximum number of entries to return.
  * \return resutls
  * \brief Function to search data
  */
  int
  searchLDAP( string          base,
              string          filter,
              LDAPMessage     **res,
              int             scope=LDAP_SCOPE_SUBTREE,
              char            **attrs=NULL,
              int             attrsonly=0,
              LDAPControl     **serverctrls=NULL,
              LDAPControl     **clientctrls=NULL,
              struct timeval  *timeout=NULL,
              int             sizelimit=LDAP_NO_LIMIT);
  /**
  * \brief Function to display the search results
  */
  int
  print(LDAPMessage **res);

  /**
  * \brief Function to know if results have found
  * \return true if results have found else false
  */
  bool
  hasResults(LDAPMessage **res);

  /**
  * \brief Function to get the user's password
  */
  string
  getUserPassword(LDAPMessage **res);

  /**
    * \brief Destructor
    */
  ~LDAPProxy();

  private:
  int
  bind();
  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
  * \brief An LDAP structure
  */
  LDAP* mld;

  /**
  * \brief The LDAP uri by of the form host:port
  */
   string muri;
  /**
  * \brief the distinguished name of the entry used to bind
  */
   string mdistName;
  /**
  * \brief the authentication method used
  */
   string mauthMechanism;
   /**
  * \brief the credential to use for authentication
  */
   string mcredential;

   LDAPControl* mserverCtrls;
   LDAPControl* mclientCtrls;
};
#endif //_LDAP_PROXY_H
