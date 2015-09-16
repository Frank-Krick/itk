//
// Created by Frank Krick on 9/13/15.
//

#ifndef INSTRUMENT_TOOL_KIT_CONTROLFUNCTOR_H
#define INSTRUMENT_TOOL_KIT_CONTROLFUNCTOR_H

#include <parameter_description/ParameterDescription.h>
#include <processing/Parameter.h>
#include <unordered_map>
#include "Functor.h"

namespace itk {

    class ControlFunctor : public Functor {
    public:
        typedef std::unordered_map<IndexType, ParameterMap> ParameterDeviceMap;
        typedef std::shared_ptr<ControlFunctor> Ptr;

        virtual void operator () (
                DataBuffer::iterator outputBegin, DataBuffer::iterator outputEnd,
                ParameterDeviceMap & parameter) = 0;

        virtual std::list<ParameterDescription> parameterList();

        ControlFunctor(IndexType deviceId, unsigned int sampleRate)
                : Functor(deviceId),
                  sampleRate(sampleRate) {};

    protected:
        unsigned int sampleRate;
    };

}

#endif //INSTRUMENT_TOOL_KIT_CONTROLFUNCTOR_H
