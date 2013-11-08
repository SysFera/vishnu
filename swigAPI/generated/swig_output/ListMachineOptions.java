/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sysfera.vishnu.api.vishnu.internal;

public class ListMachineOptions extends EObject {
  private long swigCPtr;

  public ListMachineOptions(long cPtr, boolean cMemoryOwn) {
    super(VISHNUJNI.ListMachineOptions_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  public static long getCPtr(ListMachineOptions obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        VISHNUJNI.delete_ListMachineOptions(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public ListMachineOptions() {
    this(VISHNUJNI.new_ListMachineOptions(), true);
  }

  public void _initialize() {
    VISHNUJNI.ListMachineOptions__initialize(swigCPtr, this);
  }

  public String getUserId() {
    return VISHNUJNI.ListMachineOptions_getUserId(swigCPtr, this);
  }

  public void setUserId(String _userId) {
    VISHNUJNI.ListMachineOptions_setUserId(swigCPtr, this, _userId);
  }

  public boolean isListAllMachine() {
    return VISHNUJNI.ListMachineOptions_isListAllMachine(swigCPtr, this);
  }

  public void setListAllMachine(boolean _listAllMachine) {
    VISHNUJNI.ListMachineOptions_setListAllMachine(swigCPtr, this, _listAllMachine);
  }

  public String getMachineId() {
    return VISHNUJNI.ListMachineOptions_getMachineId(swigCPtr, this);
  }

  public void setMachineId(String _machineId) {
    VISHNUJNI.ListMachineOptions_setMachineId(swigCPtr, this, _machineId);
  }

  public SWIGTYPE_p_ecorecpp__mapping__any eGet(int _featureID, boolean _resolve) {
    return new SWIGTYPE_p_ecorecpp__mapping__any(VISHNUJNI.ListMachineOptions_eGet(swigCPtr, this, _featureID, _resolve), true);
  }

  public void eSet(int _featureID, SWIGTYPE_p_ecorecpp__mapping__any _newValue) {
    VISHNUJNI.ListMachineOptions_eSet(swigCPtr, this, _featureID, SWIGTYPE_p_ecorecpp__mapping__any.getCPtr(_newValue));
  }

  public boolean eIsSet(int _featureID) {
    return VISHNUJNI.ListMachineOptions_eIsSet(swigCPtr, this, _featureID);
  }

  public void eUnset(int _featureID) {
    VISHNUJNI.ListMachineOptions_eUnset(swigCPtr, this, _featureID);
  }

  public SWIGTYPE_p_ecore__EClass _eClass() {
    long cPtr = VISHNUJNI.ListMachineOptions__eClass(swigCPtr, this);
    return (cPtr == 0) ? null : new SWIGTYPE_p_ecore__EClass(cPtr, false);
  }

}
