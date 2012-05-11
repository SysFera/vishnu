#ifndef __TREATMENTDATA__H__
#define __TREATMENTDATA__H__

// Container class to contain the data for the treatment
// only to abstract the treatment parameters within the class,
// just attributes, getters and setters, nothing more than a struct

#include "Annuary.hpp"

template <class T>
class TreatmentData {

public:
  TreatmentData() {}


  ~TreatmentData(){}

  void
  setAnnuary(Annuary *ann){
    mann = ann;
  }

  void
  setServer(T* ser){
    mser = ser;
  }

  Annuary*
  getAnnuary(){
    return mann;
  }


  T*
  getServer(){
    return mser;
  }


private:
  Annuary* mann;
  T* mser;


};

#endif // __TREATMENTDATA__H__
