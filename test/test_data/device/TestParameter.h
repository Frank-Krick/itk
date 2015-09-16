//
// Created by Frank Krick on 9/12/15.
//

#ifndef INSTRUMENT_TOOL_KIT_TESTPARAMETER_H
#define INSTRUMENT_TOOL_KIT_TESTPARAMETER_H

#include <parameter_description/ParameterDescription.h>

using namespace itk;

class TestParameter {
public:
    static ParameterDescription testParameter(IndexType parameterId);

};


#endif //INSTRUMENT_TOOL_KIT_TESTPARAMETER_H
