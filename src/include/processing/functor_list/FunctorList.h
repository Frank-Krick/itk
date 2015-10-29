#ifndef INSTRUMENT_TOOL_KIT_FUNCTORLIST_H
#define INSTRUMENT_TOOL_KIT_FUNCTORLIST_H

#include <Typedef.h>
#include <processing/Parameter.h>

#include <array>
#include <processing/generator/ControlFunctor.h>
#include <processing/generator/AudioFunctor.h>

namespace itk {

template<class T>
class FunctorList {
public:
    typedef std::shared_ptr<FunctorList<T>> Ptr;
    typedef std::pair<IndexType, ParameterList> ParameterPair;
    typedef std::list<ParameterPair> AllParameters;

    void operator () (
            InputChannels beginIn, InputChannels endIn,
            OutputChannels beginOut, OutputChannels endOut,
            AudioFunctor::ParameterDeviceMap & parameter) {

        for (auto functor : functors) {
            (*functor)(beginIn, endIn, beginOut, endOut, parameter[functor->deviceId()]);
        }
    }

    void operator () (DataBuffer::iterator begin, DataBuffer::iterator end,
                      ControlFunctor::ParameterDeviceMap & parameter) {

        for (auto functor : functors) {
                (*functor)(begin, end, parameter);
        }
    }

    AllParameters allParameters() {
        AllParameters parameters;
        for (auto functor : functors) {
            auto functorParameters = functor->parameterList();
            parameters.push_back(ParameterPair(functor->deviceId(), functorParameters));
        }
        return parameters;
    }

    void push_back(T functor) { functors.push_back(functor); }

    FunctorList() {}

private:
    std::list<T> functors;
};

}

#endif //INSTRUMENT_TOOL_KIT_FUNCTORLIST_H
