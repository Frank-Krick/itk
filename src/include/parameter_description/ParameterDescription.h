//
// Created by Frank Krick on 9/4/15.
//

#ifndef INSTRUMENT_TOOL_KIT_PARAMETERDESCRIPTION_H
#define INSTRUMENT_TOOL_KIT_PARAMETERDESCRIPTION_H

#include <Typedef.h>

#include <memory>
#include <string>

namespace itk {

    struct ParameterDescription {
    public:
        IndexType id = 0;
        std::string name;
        std::string description;
        DataType max = 0.0;
        DataType min = 0.0;

    private:
    };

    bool operator< (const ParameterDescription &lhs, const ParameterDescription &rhs);
    bool operator== (const ParameterDescription &lhs, const ParameterDescription &rhs);
}

#endif //INSTRUMENT_TOOL_KIT_PARAMETERDESCRIPTION_H
