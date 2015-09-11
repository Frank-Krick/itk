//
// Created by Frank Krick on 9/10/15.
//

#ifndef INSTRUMENT_TOOL_KIT_UTILITY_H
#define INSTRUMENT_TOOL_KIT_UTILITY_H

#include <memory>

using std::shared_ptr;

namespace itk {

    template<typename T>
    auto clone(shared_ptr<T> obj) {
        return obj->clone();
    }

    template<typename T>
    auto clone(T obj) {
        return obj.clone();
    }

}


#endif //INSTRUMENT_TOOL_KIT_UTILITY_H
