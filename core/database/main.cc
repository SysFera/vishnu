
#include <iostream>
#include "POSTGREDatabase.hh"
#include "DbFactory.hh"

int main () {
    //DbFactory factory = new DbFactory::DbFactory();// = new DbFactory();
  //factory.create(POSTGREDB, NULL, NULL, NULL, "Paco",0);
  //create(int type, std::string host, std::string user, std::string pwd, std::string db, unsigned int port)
  std::cout<<"It's my test"<<std::endl;
  DbFactory factory;
  
  Database *p = factory.create(POSTGREDB, "", "", "", "Paco");
  
  int ret = 0;// = 
  
  std::cout<< "************ MIS A JOUR ********** "<<std::endl;
  
  ret = p->process("DELETE FROM tableuser;");
  std::cout<< "int retour "<<ret<<std::endl;
  
   p->process("select * from tableuser");
  
  
  std::string transac = "insert into tableuser (nom,password) values ('Paco1','GENTILDD'); insert into tableuser (nom,password) values ('Paco2','MECHANT');";
  //transac.append("insert into tableuser (nom,password) values ('William','titi');");
  
  std::cout<< "************ TRANSACTION ********** "<<std::endl;
  
  
  p->startTransaction(transac);
 // p->commit();
  
  
  ret = p->process("select * from tableuser");
  std::cout<< "int retour "<<ret<<std::endl;
  
  std::cout<< "************ ROLLBACK ********** "<<std::endl;
  p->rollback();
  
  ret = p->process("select * from tableuser");
  std::cout<< "int retour "<<ret<<std::endl;
  
  std::cout<< "************ COMMIT ********** "<<std::endl;
  
  p->startTransaction(transac);
  p->commit();
  
  
  ret = p->process("select * from tableuser");
  std::cout<< "int retour "<<ret<<std::endl;
  
  std::cout<< "************ ROLLBACK ********** "<<std::endl;
  p->rollback();
  
  ret = p->process("select * from tableuser");
  std::cout<< "int retour "<<ret<<std::endl;
  
  ret = p->process("DELETE FROM session;");
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
 
}