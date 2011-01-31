
#include <iostream>
#include "POSTGREDatabase.hh"
#include "DbFactory.hh"
#include <exception>
#include "SystemException.hh"


int main () {
    //DbFactory factory = new DbFactory::DbFactory();// = new DbFactory();
  //factory.create(POSTGREDB, NULL, NULL, NULL, "Paco",0);
  //create(int type, std::string host, std::string user, std::string pwd, std::string db, unsigned int port)
  std::cout<<"It's my test"<<std::endl;
  DbFactory factory;
  
  Database *p = factory.create(POSTGREDB, "", "", "", "Paco");
 
 // p->connect();
  int ret = 0;// = 
  
  std::cout<< "************ MIS A JOUR ********** "<<std::endl;
  
  //ret = p->process("DELETE FROM tableuser;");
  
  
  //std::cout<< "int retour "<<ret<<std::endl;
  
 /* 
  try {
  p->process("select * from tabluser");
  } catch (SystemException* e) {
  std::cout << "Message generique <-> : " << e->getMsg();
  std::cout << "Details supplementaires : " << e->getMsgComp();  
  }*/
  
  try {
  p->connect();  
  p->process("DELETE FROM tableuser;");  
  std::cout<< " Je suis là 1"<<std::endl;
  p->process("insert into tableuser (nom,password) values ('Paco1','GENTILDD'); insert into tableuser (nom,password) values ('Paco2','MECHANT');");
  std::cout<< " Je suis là 2"<<std::endl;
  p->connect();  
  } catch (SystemException& e) {
  std::cout << "Message generique <-> 1: " << e.getMsg()<<std::endl;
  std::cout << "Details supplementaires 2: " << e.getMsgComp() <<std::endl;  
  }
  
  DatabaseResult *data;
  
  try {
  data = p->getResult("select * from tableuser");
  std::cout<< "************ ATTRIBUT NAMES ********** "<<std::endl;
  data->printAttributesNames();
  std::cout<< "************ ALL ********** "<<std::endl;
  data->print();
  
  std::cout<< "NB RESULT:"<<data->getNbTuples()<<std::endl;
  std::cout<< "NB CHAMPS:"<<data->getNbFields()<<std::endl;
  
  
  std::cout<< "get(-1)"<<std::endl;
  
  std::vector<std::string> tmp = data->get(-1);
  std::vector<std::string>::iterator ii;
  
  //std::vector<std::vector<std::string> >* 
  //p = 
  
  
  
    for(ii=tmp.begin();ii!=tmp.end();ii++) {
	    std::cout << *ii <<"  ";
    }
    
    tmp.clear();
    std::cout<< "get(100)"<<std::endl;
  
    tmp = data->get(100);
    
    for(ii=tmp.begin();ii!=tmp.end();ii++) {
	    std::cout << *ii <<"  ";
    }
    
    std::cout<< "get(1)"<<std::endl;
  
    tmp.clear();
    tmp = data->get(0);
    
    for(ii=tmp.begin();ii!=tmp.end();ii++) {
	    std::cout << *ii <<"  ";
    }
   
   /*
  std::cout<<std::endl<< "TEST" << std::endl<<std::endl;
  
  std::vector<std::vector<std::string> > po = data->getResults();  
  
  for (int i = 0; i < static_cast<int> (po.size()); ++i) {
    tmp.clear();
    tmp = po[i];
    
    for(ii=tmp.begin();ii!=tmp.end();ii++) {
	    std::cout << *ii <<"  ";
    }
    
  }
  
  std::cout<<std::endl<<"FIN TEST" << std::endl;
  */
  
  
  std::string transac = "insert into tableuser (nom,password) values ('Paco1','GENTILDD'); insert into tableuser (nom,password) values ('Paco2','MECHANT');";
  //transac.append("insert into tableuser (nom,password) values ('William','titi');");
  
  std::cout<< "************ TRANSACTION ********** "<< std::endl;
  
  
  p->startTransaction(transac);
 // p->commit(); 
  
  data = p->getResult("select * from tableuser");
  std::cout<< "************ ATTRIBUT NAMES ********** "<<std::endl;
  data->printAttributesNames();
  std::cout<< "************ ALL ********** "<<std::endl;
  data->print();
  
  
  //ret = p->process("select * from tableuser");
  std::cout<< "int retour "<<ret<<std::endl;
  
  std::cout<< "************ ROLLBACK ********** "<<std::endl;
  p->rollback();
  
  data = p->getResult("select * from tableuser");
  std::cout<< "************ ATTRIBUT NAMES ********** "<<std::endl;
  data->printAttributesNames();
  std::cout<< "************ ALL ********** "<<std::endl;
  data->print();
  
  //ret = p->process("select * from tableuser");
  std::cout<< "int retour "<<ret<<std::endl;
  
  std::cout<< "************ COMMIT ********** "<<std::endl;
  
  p->startTransaction(transac);
  p->commit();
  
  
  //ret = p->process("select * from tableuser");
  std::cout<< "int retour "<<ret<<std::endl;
  
  std::cout<< "************ ROLLBACK ********** "<<std::endl;
  p->rollback();
  
  //ret = p->process("select * from tableuser");
  std::cout<< "int retour "<<ret<<std::endl;
  
 // ret = p->process("DELETE FROM session;");
  //ret = p->process("select * from tableuser");
  
 // std::vector<std::string> v = p->getResult();
 
  /*for (int i=0; i<v.size(); i++) {
    std::cout << v[i].c_str() << std::endl;
  }*/

 /*std::cout<<"It's my test2"<<std::endl;
 
 p->process("select * from tableuser");
  
  v = p->getResult();
 
  for (int i=0; i<v.size(); i++) {
    std::cout << v[i].c_str() << std::endl;
}*/
 
 } catch (SystemException& e) {
  std::cout << "Message generique <-> : " << e.getMsg();
  std::cout << "Details supplementaires : " << e.getMsgComp();  
  }
 
 /*std::cout<< "************ RECONNECT ********** "<<std::endl;
  
  
  try { 
  p->reconnect();
  } catch (SystemException* e) {
  std::cout << "Message generique <-> : " << e->getMsg();
  std::cout << "Details supplementaires : " << e->getMsgComp();  
  }
 */
 
}