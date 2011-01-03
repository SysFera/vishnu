#include <iostream>
#include <cstdlib>
#include <exception>

#include "UMS.hh"
#include "TMS.hh"
#include "FMS.hh"
#include "IMS.hh"

#include <typeinfo>

#define NBMOD 4

int main(int argc, char** argv){

  int r;
  UMS u;
  TMS t;
  FMS f;
  IMS i;
  srand (time (NULL));

  while (1){
    r = rand ();
    try{

#ifdef DEBUG
      int tmp, tmp2, tmp3;
      tmp = r%NBMOD;
      tmp2 = r%7;
      tmp3 = r%5;

      fprintf (stderr, "Valeur de r-nbmod = %d, r-7=%d, r-5=%d \n", tmp, tmp2, tmp3);
#endif //DEBUG
      if ((r%NBMOD) == 0)
	u.call (rand()%7);
      else if ((r%NBMOD) == 1)
	t.call (rand()%5);
//      else if ((r%NBMOD) == 2)
//	f.call (rand()%5);
      else if ((r%NBMOD) == 3)
	i.call (rand()%5);
    }
    catch (SystemException* e){
      std::cout << " Erreur dans le module : " << e->getTypeS() << std::endl;
      std::cout << " Message generique <-> : " << e->getMsg() << std::endl;
      std::cout << "Details supplementaires : " << e->getMsgComp() << std::endl;
    }
    sleep (2);
  }
  return 0;
}
