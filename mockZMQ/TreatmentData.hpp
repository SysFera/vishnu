#ifndef __TREATMENTDATA__H__
#define __TREATMENTDATA__H__

// Container class to contain the data for the treatment
// only to abstract the treatment parameters within the class,
// just attributes, getters and setters, nothing more than a struct

#include "Annuary.hpp"
#include "SeD.hpp"

class TreatmentData {

public:
  TreatmentData();

  ~TreatmentData();

  void
  setAnnuary(Annuary *ann);

  void
  setServer(SeD* ser);

  Annuary*
  getAnnuary();

  SeD*
  getServer();

private:
  Annuary* mann;
  SeD* mser;


};

#endif // __TREATMENTDATA__H__
