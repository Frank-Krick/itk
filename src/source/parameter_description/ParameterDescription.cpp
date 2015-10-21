//
// Created by Frank Krick on 9/5/15.
//

#include <parameter_description/ParameterDescription.h>

namespace itk {

    bool operator<(const ParameterDescription &lhs, const ParameterDescription &rhs) {
        return lhs.id() < rhs.id();
    }

    bool operator==(const ParameterDescription &lhs, const ParameterDescription &rhs) {
        return lhs.id() == rhs.id();
    }

}
