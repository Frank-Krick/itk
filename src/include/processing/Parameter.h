//
// Created by Frank Krick on 9/7/15.
//

#ifndef INSTRUMENT_TOOL_KIT_PARAMETER_H
#define INSTRUMENT_TOOL_KIT_PARAMETER_H

#include <Typedef.h>

#include <unordered_map>

namespace itk {

    struct Parameter {
        DataBuffer::iterator begin;
        DataBuffer::iterator end;
    };

    typedef std::unordered_map<IndexType, Parameter> ParameterMap;
}

#endif //INSTRUMENT_TOOL_KIT_PARAMETER_H
