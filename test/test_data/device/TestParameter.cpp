//
// Created by Frank Krick on 9/12/15.
//

#include "TestParameter.h"

#include <sstream>

using std::stringstream;
ParameterDescription TestParameter::testParameter(IndexType parameterId) {
    ParameterDescription parameterDescription;
    stringstream stream;
    stream << "Test  parameter with id " << parameterId;
    parameterDescription.description = stream.str();
    stream.clear();
    stream << "Para " << parameterId;
    parameterDescription.name = stream.str();
    parameterDescription.id = parameterId;
    parameterDescription.max = 500;
    parameterDescription.min = 1;
    return parameterDescription;
}