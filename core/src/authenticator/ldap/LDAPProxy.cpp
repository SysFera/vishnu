#include "LDAPProxy.hpp"
#include "SystemException.hpp"
#include "UMSVishnuException.hpp"

/**
* \param uri The LDAP uri by of the form host:port
* \param userName the distinguished name of the entry used to bind
* \param authMechanism the authentication method used to bind
* \param password the password of the user
* \param serverCtrls A list of LDAP server controls
* \param clientCtrls A list of LDAP client controls
* \brief Constructor
*/
LDAPProxy::LDAPProxy(const string& uri,
                     const string& userName,
                     const string& authMechanism,
                     const string& password,
                     LDAPControl* serverCtrls,
                     LDAPControl* clientCtrls) :
  muri(uri), muserName(userName),
  mauthMechanism(authMechanism), mpwd(password)
{

   mld = NULL;
   mserverCtrls =  serverCtrls;
   mclientCtrls = clientCtrls;
}

/**
* \brief Function to initialize a connection on a LDAP server
*/
int
LDAPProxy::connectLDAP(const string& ldapbase) {
  int ret;

  /* Initialize the LDAP session */
  if ((ldap_initialize(&mld, const_cast<char*>(muri.c_str()))) != LDAP_SUCCESS) {
      throw SystemException(ERRCODE_AUTHENTERR, "LDAP session initialization failed");
  }

  ldap_set_option(mld, LDAP_OPT_PROTOCOL_VERSION, &desired_version);

  ret = bind();
  if (ret != LDAP_SUCCESS ) {
    if (ret != LDAP_INVALID_CREDENTIALS ) {
      throw SystemException(ERRCODE_AUTHENTERR, ldap_err2string(ret));
    }
    throw UserException(ERRCODE_UNKNOWN_USER, "The user is unknown");
  }
  return 0;
}

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
//int
//LDAPProxy::searchLDAP(  string          base,
//                        string          filter,
//                        LDAPMessage     **res,
//                        int             scope,
//                        char            **attrs,
//                        int             attrsonly,
//                        LDAPControl     **serverctrls,
//                        LDAPControl     **clientctrls,
//                        struct timeval  *timeout,
//                        int             sizelimit) {
//
//  int result = ldap_search_ext_s( mld,
//                                  base.c_str(),
//                                  scope,
//                                  filter.c_str(),
//                                  attrs,
//                                  attrsonly,
//                                  serverctrls,
//                                  clientctrls,
//                                  timeout,
//                                  sizelimit,
//                                  res);
//  if (result != LDAP_SUCCESS) {
//    ldap_msgfree(*res);
//    throw SystemException(ERRCODE_AUTHENTERR, ldap_err2string(result));
//  }
//
//  return 0;
//}

/**
* \brief Function to display the search results
*/
int
LDAPProxy::print(LDAPMessage **res) {

  LDAPMessage *entry;
  BerElement  *ber;
  char *attribute;
  char *dn;
  struct berval** resBerval;
  struct berval res_data;
  int entryCount;

  //ldap_compare_ext_s

  for (entry =   ldap_first_entry( mld, *res);
            entry   !=  NULL;
            entry   =   ldap_next_entry( mld, entry ) ) {

        if (( dn = ldap_get_dn(mld, entry )) != NULL) {
            cout << "dn: " << dn << "\n";
            ldap_memfree( dn );
        }
        for (   attribute = ldap_first_attribute( mld, entry, &ber );
                attribute != NULL;
                attribute = ldap_next_attribute( mld, entry, ber ) ) {
            /* Get values and print.  Assumes all values are strings. */
            resBerval = ldap_get_values_len(mld, entry, attribute);

            if (resBerval != NULL ) {
              res_data = *resBerval[0];
              cout << attribute << "   : " << res_data.bv_val << "\n";
            }
            ldap_memfree( attribute );
            ldap_value_free_len(resBerval);
        }//End For
        ber_free(ber, 0);
    }//End For
    entryCount = ldap_count_entries(mld, *res);
    cout << "\n    Search completed successfully.\n    Entries  returned:" <<  entryCount << "\n";
  return 0;
}

/**
* \brief Function to know if results have found
* \return true if results have found else false
*/
bool
LDAPProxy::hasResults(LDAPMessage **res) {
  return (ldap_count_entries(mld, *res) != 0);
}

/**
* \brief Function to get the user's password
*/

string
LDAPProxy::getUserPassword(LDAPMessage **res) {

  LDAPMessage *entry;
  BerElement  *ber;
  char *attribute;
  char *dn;
  struct berval** resBerval;
  string pwd;

  for (entry =   ldap_first_entry( mld, *res);
            entry   !=  NULL;
            entry   =   ldap_next_entry( mld, entry ) ) {

        for (   attribute = ldap_first_attribute( mld, entry, &ber );
                attribute != NULL;
                attribute = ldap_next_attribute( mld, entry, ber ) ) {

          if (string("userPassword").compare(attribute) == 0) {
            resBerval = ldap_get_values_len(mld, entry, attribute);
            if (resBerval != NULL ) {
              pwd = (*resBerval[0]).bv_val;
            }
            ldap_value_free_len(resBerval);
          }
          ldap_memfree( attribute );
        }//End For
        ber_free(ber, 0);
    }//End For
  return pwd;
}



int
LDAPProxy::bind() {
  struct berval cred;
  cred.bv_val = const_cast<char*>(mpwd.c_str());
  cred.bv_len = strlen(const_cast<char*>(mpwd.c_str()));

  /* Bind to the server */
  return ldap_sasl_bind_s (
  mld,
  const_cast<char*>(muserName.c_str()),
  LDAP_SASL_SIMPLE,
  &cred,
  &mserverCtrls,
  &mclientCtrls,
  NULL);
}

LDAPProxy::~LDAPProxy() {
  if (mld != NULL) {
    ldap_unbind_ext_s ( mld, NULL, NULL );
  }
}
