//
// Created by Frank Krick on 9/13/15.
//

#ifndef INSTRUMENT_TOOL_KIT_TESTINSTANCES_H
#define INSTRUMENT_TOOL_KIT_TESTINSTANCES_H

#include "../../src/source/processing/instance/InstanceImplementation.h"

using namespace itk;

class TestInstances {
public:
    static std::pair<ParameterTable::Ptr, Instance::Ptr> createSimpleTestInstance(unsigned int bufferSize);
    static std::pair<ParameterTable::Ptr, Instance::Ptr> twoFunctorListInSeriesInstance(unsigned int bufferSize);
};


#endif //INSTRUMENT_TOOL_KIT_TESTINSTANCES_H
