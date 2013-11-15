/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sysfera.vishnu.api.vishnu.internal;

public class TailOfFileOptions extends EObject {
  private long swigCPtr;

  public TailOfFileOptions(long cPtr, boolean cMemoryOwn) {
    super(VISHNUJNI.TailOfFileOptions_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  public static long getCPtr(TailOfFileOptions obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        VISHNUJNI.delete_TailOfFileOptions(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public TailOfFileOptions() {
    this(VISHNUJNI.new_TailOfFileOptions(), true);
  }

  public void _initialize() {
    VISHNUJNI.TailOfFileOptions__initialize(swigCPtr, this);
  }

  public int getNline() {
    return VISHNUJNI.TailOfFileOptions_getNline(swigCPtr, this);
  }

  public void setNline(int _nline) {
    VISHNUJNI.TailOfFileOptions_setNline(swigCPtr, this, _nline);
  }

  public SWIGTYPE_p_ecorecpp__mapping__any eGet(int _featureID, boolean _resolve) {
    return new SWIGTYPE_p_ecorecpp__mapping__any(VISHNUJNI.TailOfFileOptions_eGet(swigCPtr, this, _featureID, _resolve), true);
  }

  public void eSet(int _featureID, SWIGTYPE_p_ecorecpp__mapping__any _newValue) {
    VISHNUJNI.TailOfFileOptions_eSet(swigCPtr, this, _featureID, SWIGTYPE_p_ecorecpp__mapping__any.getCPtr(_newValue));
  }

  public boolean eIsSet(int _featureID) {
    return VISHNUJNI.TailOfFileOptions_eIsSet(swigCPtr, this, _featureID);
  }

  public void eUnset(int _featureID) {
    VISHNUJNI.TailOfFileOptions_eUnset(swigCPtr, this, _featureID);
  }

  public SWIGTYPE_p_ecore__EClass _eClass() {
    long cPtr = VISHNUJNI.TailOfFileOptions__eClass(swigCPtr, this);
    return (cPtr == 0) ? null : new SWIGTYPE_p_ecore__EClass(cPtr, false);
  }

}
