#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include "UMS_Data.hpp"

using namespace UMS_Data;
using namespace std;

int main(int argc, char* argv[])
{
    // CREATE DATA MODEL
    UMS_DataFactory_ptr ecoreFactory = UMS_DataFactory::_instance();
    UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();

    
    ListSessions_ptr listsession = ecoreFactory->createListSessions();
    
    Session_ptr sess1 = ecoreFactory->createSession();
     //session_ptr sess1 = ecoreFactory->createsession();
     
    
     sess1->setSessionId("ABCDEFGH");
     sess1->setSessionKey("Certif-1"); 
    // sess1->setDateCreation("Today");
     sess1->setStatus(1);
     
     Session_ptr sess2 = ecoreFactory->createSession();
     
     sess2->setSessionId("IGKLMNOPQRSTUW"); //Idsession("Session Paco 2");
     sess2->setSessionKey("Certif-2");
     sess2->setStatus(0);
     
     listsession->getSessions().push_back(sess1);
     listsession->getSessions().push_back(sess2);
     
     
     ListUsers_ptr listUsers = ecoreFactory->createListUsers();
    
    User_ptr user1 = ecoreFactory->createUser();
     //session_ptr user1 = ecoreFactory->createsession();
     
     user1->setFirstname("Eugène");
     user1->setLastname("PAMBA CAPO-CHICHI");
     user1->setUserId("eCapochi@SysFera.com");
     
     
     User_ptr user2 = ecoreFactory->createUser();
    
     
     listUsers->getUsers().push_back(user1);
     //listUsers->getUsers().push_back(user2);
    
     
     
     
    /*Job_ptr job1 = ecoreFactory->createJob();
    job1->setJobId("1");
    job1->setJobName("jobDeTest");

    Job_ptr job2 = ecoreFactory->createJob();
    job2->setJobId("2");
    job2->setJobName("jobBidon");

    listJobs->getJobs().push_back(job1);
    listJobs->getJobs().push_back(job2);
*/
    // SERIALIZE DATA MODEL
    const char* name = "Parser";
    ::ecorecpp::serializer::serializer _ser(name); // filename
    //cout<<
    
    string test = _ser.serialize(listsession);
    cout<<"SERIALIZE:"<<endl<< test<<endl<<endl;
    
   _ser.resetSerializer();
    string ListDesUsers = _ser.serialize(listUsers);
    
    cout<<"SERIALIZE:"<<endl<< ListDesUsers;
    
    //cout<< "Test" <<endl<<myString; 
    
   // cout<<"Bravo PACO:"<<_ser.serializer::getcontents(name);
    // READ DATA MODEL FROM FILE INTO BUFFER
   /* FILE *file;
    char *buffer;
    unsigned long fileLen;

    file = fopen(name, "rb");
    if (!file)
    {
      fprintf(stderr, "Unable to open file %s", name);
      return 1;
    }

    fseek(file, 0, SEEK_END);
    fileLen=ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer=(char *)malloc(fileLen+1);
    if (!buffer)
    {
      fprintf(stderr, "Memory error!");
                                  fclose(file);
      return 1;
    }

    //Read file contents into buffer
    fread(buffer, fileLen, 1, file);
    fclose(file);

    //Do what ever with buffer
    cout << "XML CONTENT:" << endl << buffer << endl;
    */
    // DELETE THE MODEL
    delete listsession;

    
    
    
    // CREATE THE DATA MODEL FROM THE BUFFER
    
    // Add the metamodel to the metamodel repository. Necessary for parsing
    // models conforming it.
    ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

    // Parse the model
    ecorecpp::parser::parser parser;
    ListSessions_ptr other_listssession = parser.load(test)->as< ListSessions >();

    // Display the model
    cout<< endl << "DESERIALIZE:" << endl;
    for (int i=0; i < other_listssession->getSessions().size(); ++i) {
      Session_ptr sess =  other_listssession->getSessions()[i];//other_listssession->getSessions[i];
      cout <<"idsess:"<< sess->getSessionId() <<endl; // << job->getJobName() << endl;
      cout <<"idsess:"<< sess->getDateCreation() <<endl;
      cout << "Status:"<<sess->getStatus() <<endl;
      cout << "Certif:"<<sess->getSessionKey()<<endl<<endl<<endl;
      	
    }
    
    
    ListUsers_ptr other_list = parser.load(ListDesUsers)->as< ListUsers >();

    
    cout<< endl << "DESERIALIZE:" << endl;
    
    //User_ptr user=  other_list->getUsers()[0];
    //cout <<"Mr:"<< user->getFirstname()<<" "<<user->getLastname()<<endl;
    for (int i=0; i <other_list->getUsers().size(); ++i) {
      User_ptr user=  other_list->getUsers()[i];//other_listssession->getSessions[i];
      cout <<"Mr:"<< user->getFirstname()<<" "<<user->getLastname()<<endl; // << job->getJobName() << endl;	
    }
    
    
    /*
    for (int i=0; i<other_listssession->getSessions.size(); ++i) {
      session_ptr sess = other_listssession->getSessions[i];
      cout << sess->getIdsession() <<endl;// << job->getJobName() << endl;
      cout << sess->getIduser() <<endl;
      cout << sess->getDatecreation() <<endl;
      cout << sess->getDateloading() <<endl;
      cout << sess->getStatus() <<endl;
    }
*/
    // Delete the parsed model
  //delete other_listssession;

    //free(buffer);
    return 0;

}
