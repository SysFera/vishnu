/**
 * \file HM.hpp
 * \brief This file defines the class that creates the history maker
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 27/04/11
 */

#ifndef __HM__HH__
#define __HM__HH__
#include "data/MetricServer.hpp"


/**
 * \class HM
 * \brief History maker class, this class deals with the creation of the cache of the machines states
 */
class HM{
public:
  /**
   * \brief Constructor
   */
  HM();
  /**
   * \brief Destructor
   */
  ~HM();
  /**
   * \brief To run the history maker
   */
  void
  run();
protected:
private:
  /**
   * \brief The metric server object
   */
  MetricServer mms;
};

#endif
