//
// Created by Frank Krick on 9/13/15.
//

#include <list>
#include "ControlFunctor.h"

namespace itk {

    std::list<ParameterDescription> itk::ControlFunctor::parameterList() {
        using std::list;
        return list<ParameterDescription>();
    }

}
