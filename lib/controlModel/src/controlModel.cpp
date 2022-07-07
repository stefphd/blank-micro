//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: controlModel.cpp
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
#include "rtwtypes.h"

// Includes for objects with custom storage classes
#include "controlModel.h"

// Exported data definition

// Definition for custom storage class: Struct
params_type params = {
  // gain
  2.0F
};

// Model step function
void ControlClass::update()
{
  // Outport: '<Root>/output1' incorporates:
  //   Gain: '<Root>/Gain'
  //   Inport: '<Root>/input1'

  controlModel_Y.output1 = params.gain * controlModel_U.input1;

  // Outport: '<Root>/output2' incorporates:
  //   Inport: '<Root>/input2'

  controlModel_Y.output2 = controlModel_U.input2;
}

// Model initialize function
void ControlClass::begin()
{
  // (no initialization code required)
}

// Model terminate function
void ControlClass::stop()
{
  // (no terminate code required)
}

// Constructor
ControlClass::ControlClass():
  controlModel_U(),
  controlModel_Y()
{
  // Currently there is no constructor body generated.
}

// Destructor
ControlClass::~ControlClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
