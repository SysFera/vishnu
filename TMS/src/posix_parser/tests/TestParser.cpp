#include <iostream>
#include <vector>
#include <boost/make_shared.hpp>
#include "POSIXParser.hpp"

class TestPosixParser {
public:
  TestPosixParser(){
  }

  ~TestPosixParser() {}

/////////////////// Get next val
  void
  testGetNextValeurPrefix(){
    std::string source("#@ vishnu_user = toto\n");
    stringstream stream(source);
    string val = "";
    bool res;
    res = GetNextValeur(stream, val);
    BOOST_REQUIRE(res && val.compare("#@")==0);
  }

  void
  testGetNextValeurKey(){
    std::string source("vishnu_user = toto\n");
    stringstream stream(source);
    string val = "";
    bool res;
    res = GetNextValeur(stream, val);
    BOOST_REQUIRE(res && val.compare("vishnu_user")==0);
  }

  void
  testGetNextValeurEqual(){
    std::string source("= toto\n");
    stringstream stream(source);
    string val = "";
    bool res;
    res = GetNextValeur(stream, val);
    BOOST_REQUIRE(res && val.compare("=")==0);
  }

  void
  testGetNextValeurVal(){
    std::string source("toto\n");
    stringstream stream(source);
    string val = "";
    bool res;
    res = GetNextValeur(stream, val);
    BOOST_REQUIRE(res && val.compare("toto")==0);
  }

  void
  testGetNextValeurRet(){
    std::string source("\n");
    stringstream stream(source);
    string val = "";
    bool res;
    res = GetNextValeur(stream, val);
    BOOST_REQUIRE(!res);
  }

///////////////////////// Get next equal

  void
  testGetNextEqual(){
    std::string source("= toto\n");
    stringstream stream(source);
    bool res;
    res = GetNextEqualSign(stream);
    BOOST_REQUIRE(res);
  }

  void
  testGetNextEqualSpace(){
    std::string source(" = toto\n");
    stringstream stream(source);
    bool res;
    res = GetNextEqualSign(stream);
    BOOST_REQUIRE(res);
  }

  void
  testGetNextEqualMid(){
    std::string source("#@ vishnu_user = toto\n");
    stringstream stream(source);
    bool res;
    res = GetNextEqualSign(stream);
    BOOST_REQUIRE(res);
  }

  void
  testGetNextEqualBad(){
    std::string source("#@ vishnu_user toto\n");
    stringstream stream(source);
    bool res;
    res = GetNextEqualSign(stream);
    BOOST_REQUIRE(!res);
  }

/////////////////////: GetNextToken
  void
  testGetNextToken(){
    std::string source("#@ vishnu_user = toto\n");
    stringstream stream(source);
    string val = "";
    bool res;
    res = GetNextToken(stream, val);
    BOOST_REQUIRE(res && val.compare("vishnu_user")==0);
  }

  void
  testGetNextTokenNoEq(){
    std::string source(" toto\n");
    stringstream stream(source);
    string val = "";
    bool res;
    res = GetNextToken(stream, val);
    BOOST_REQUIRE(res && val.compare("toto")==0);
  }

  void
  testGetNextTokenNoDeb(){
    std::string source(" vishnu_user = toto\n");
    stringstream stream(source);
    string val = "";
    bool res;
    res = GetNextToken(stream, val);
    BOOST_REQUIRE(res && val.compare("vishnu_user")==0);
  }

  void
  testGetNextTokenEmpty(){
    std::string source("");
    stringstream stream(source);
    string val = "";
    bool res;
    res = GetNextToken(stream, val);
    BOOST_REQUIRE(!res);
  }

/////////////////////: GetNextLine
  void
  testGetNextLineLast(){
    std::string source("#@ vishnu_user = toto \n");
    Definition d;
    stringstream stream(source);
    bool res;
    bool valide;
    res = GetNextLine(stream, d, valide);
    BOOST_REQUIRE(res && d.key.compare("vishnu_user")==0 && d.value.compare("toto")==0 && valide);
  }

  void
  testGetNextLineLastNoSpace(){
    std::string source("#@ vishnu_user = toto\n");
    Definition d;
    stringstream stream(source);
    bool res;
    bool valide;
    res = GetNextLine(stream, d, valide);
    std::cerr << "res : " << res << std::endl;
    std::cerr << "key : " << d.key << std::endl;
    std::cerr << "value : " << d.value << std::endl;
    std::cerr << "valide : " << valide << std::endl;
    BOOST_REQUIRE(res && d.key.compare("vishnu_user")==0 && d.value.compare("toto")==0 && valide);
  }

  void
  testGetNextLine(){
    std::string source("#@ vishnu_user = toto \n titi");
    Definition d;
    stringstream stream(source);
    bool res;
    bool valide;
    res = GetNextLine(stream, d, valide);
    BOOST_REQUIRE(res && d.key.compare("vishnu_user")==0 && d.value.compare("toto")==0 && valide);
  }

  void
  testGetNextLineNoKey(){
    std::string source("#@ = toto \n titi");
    Definition d;
    stringstream stream(source);
    bool res;
    bool valide;
    res = GetNextLine(stream, d, valide);
    BOOST_REQUIRE(!res && !valide);
  }

  void
  testGetNextLineNoValue(){
    std::string source("#@ vishnu_user =  \n");
    Definition d;
    stringstream stream(source);
    bool res;
    bool valide;
    res = GetNextLine(stream, d, valide);
    BOOST_REQUIRE(!res && !valide);
  }

  void
  testGetNextLineNoValueSpace(){
    std::string source("#@ vishnu_user =  \n ");
    Definition d;
    stringstream stream(source);
    bool res;
    bool valide;
    res = GetNextLine(stream, d, valide);
    BOOST_REQUIRE(!res && !valide);
  }

/////////////////////////// GetNextDefinition
  void
  testGetNextDefinition(){
    std::string source("#@ vishnu_user = toto \n ");
    Definition d;
    stringstream stream(source);
    bool res;
    bool valide;
    res = GetNextDefinition(stream, d);
    BOOST_REQUIRE(res);
  }

  void
  testGetNextDefinitionNoKey(){
    std::string source("#@  = toto \n ");
    Definition d;
    stringstream stream(source);
    bool res;
    bool valide;
    res = GetNextDefinition(stream, d);
    BOOST_REQUIRE(!res);
  }

  void
  testGetNextDefinitionNoVal(){
    std::string source("#@ vishnu_user = \n ");
    Definition d;
    stringstream stream(source);
    bool res;
    bool valide;
    res = GetNextDefinition(stream, d);
    BOOST_REQUIRE(!res);
  }

  void
  testGetNextDefinitionNoEq(){
    std::string source("#@ vishnu_user toto \n ");
    Definition d;
    stringstream stream(source);
    bool res;
    bool valide;
    res = GetNextDefinition(stream, d);
    BOOST_REQUIRE(!res);
  }

///////////////////////////////////// parse command

private :
};
