//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: controlModel.h
//
// Code generated for Simulink model 'controlModel'.
//
// Model version                  : 3.13
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Thu Jul  7 10:36:57 2022
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
//    3. ROM efficiency
//    4. MISRA C:2012 guidelines
// Validation result: Not run
//
#ifndef RTW_HEADER_controlModel_h_
#define RTW_HEADER_controlModel_h_
#include "rtwtypes.h"
#include "controlModel_types.h"

// Type definition for custom storage class: Struct
struct params_type {
  real32_T gain;                       // Referenced by: '<Root>/Gain'
};

// Exported data declaration

// Declaration for custom storage class: Struct
extern params_type params;

// Class declaration for model controlModel
class ControlClass
{
  // public data and function members
 public:
  // External inputs (root inport signals with default storage)
  struct ExtU_controlModel_T {
    real32_T input1;                   // '<Root>/input1'
    real32_T input2;                   // '<Root>/input2'
  };

  // External outputs (root outports fed by signals with default storage)
  struct ExtY_controlModel_T {
    real32_T output1;                  // '<Root>/output1'
    real32_T output2;                  // '<Root>/output2'
  };

  // External inputs
  ExtU_controlModel_T controlModel_U;

  // External outputs
  ExtY_controlModel_T controlModel_Y;

  // Root inports set method
  void setExternalInputs(const ExtU_controlModel_T *pExtU_controlModel_T)
  {
    controlModel_U = *pExtU_controlModel_T;
  }

  // Root outports get method
  const ExtY_controlModel_T &getExternalOutputs() const
  {
    return controlModel_Y;
  }

  // model initialize function
  static void begin();

  // model step function
  void update();

  // model terminate function
  static void stop();

  // Constructor
  ControlClass();

  // Destructor
  ~ControlClass();

  \
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'controlModel'

#endif                                 // RTW_HEADER_controlModel_h_

//
// File trailer for generated code.
//
// [EOF]
//
