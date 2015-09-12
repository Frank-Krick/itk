//
// Created by Frank Krick on 9/11/15.
//

#ifndef INSTRUMENT_TOOL_KIT_FUNCTORLIST_H
#define INSTRUMENT_TOOL_KIT_FUNCTORLIST_H

#include <Typedef.h>
#include <processing/Parameter.h>

#include <array>

namespace itk {

    template<class T>
    class FunctorList {
    public:
        void operator () (
                InputChannels beginIn, InputChannels endIn,
                OutputChannels beginOut, OutputChannels endOut,
                ParameterMap & parameter) {

            for (auto functor : functors) {
                functor(beginIn, endIn, beginOut, endOut, parameter);
            }
        }

        void push_back(T functor) { functors.push_back(functor); }

        FunctorList() {}

    private:
        std::list<T> functors;
    };

}

#endif //INSTRUMENT_TOOL_KIT_FUNCTORLIST_H
