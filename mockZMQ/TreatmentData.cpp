#include "TreatmentData.hpp"

TreatmentData::TreatmentData(){
}

TreatmentData::~TreatmentData(){
}



void
TreatmentData::setAnnuary(Annuary* ann){
  mann = ann;
}


void
TreatmentData::setServer(SeD* ser){
  mser = ser;
}


Annuary*
TreatmentData::getAnnuary(){
  return mann;
}

SeD*
TreatmentData::getServer(){
  return mser;
}

