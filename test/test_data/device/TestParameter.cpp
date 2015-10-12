//
// Created by Frank Krick on 9/12/15.
//

#include "TestParameter.h"

#include <sstream>

using std::stringstream;
ParameterDescription TestParameter::testParameter(IndexType parameterId) {
    stringstream stream;
    stream << "Test  parameter with id " << parameterId;
    auto description = stream.str();
    stream.clear();
    stream << "Para " << parameterId;
    auto name = stream.str();
    ParameterDescription parameterDescription(
            parameterId,
            name,
            description,
            500,
            1
    );
    return parameterDescription;
}