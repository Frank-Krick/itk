//
// Created by Frank Krick on 9/7/15.
//

#ifndef INSTRUMENT_TOOL_KIT_GENERATOR_H
#define INSTRUMENT_TOOL_KIT_GENERATOR_H

#include <Typedef.h>
#include <processing/Parameter.h>
#include <parameter_description/ParameterDescription.h>

#include "Functor.h"

#include <array>
#include <unordered_map>
#include <list>

namespace itk {
    /*
     * The functor AudioFunctor provides the base class for generating functions
     * like sine or square generators.
     */
    class AudioFunctor : public Functor {
    public:
        typedef std::unordered_map<IndexType, ParameterMap> ParameterDeviceMap;
        typedef std::shared_ptr<AudioFunctor> Ptr;
        /*
         * Called to generate the data. The output is written into the
         * range defined by begin and end. ParameterMap contains iterators
         * that enable access to all requested parameters.
         * The iterator for each parameter is guaranteed to not reach end
         * until the range defined by begin and end ends.
         */
        virtual void operator () (
                InputChannels beginIn, InputChannels endIn,
                OutputChannels beginOut, OutputChannels endOut,
                ParameterMap & parameter) = 0;
        /*
         * Returns true if the sequence generated by the generator
         * would be the same given the same input (Two iterators with
         * the same distance and the same parameters).
         */
        virtual bool operator == (const AudioFunctor &) = 0;
        virtual bool operator != (const AudioFunctor &);
        /*
         * Returns a copy of self.
         */
        virtual Ptr clone() = 0;
        /*
         * Returns a copy of self with the given device id.
         */
        virtual Functor::Ptr clone(IndexType deviceId) override = 0;
        /*
         * Sets the initial values for all member variables.
         */
        virtual void reset() = 0;

        virtual ParameterList parameterList() override = 0;

        AudioFunctor(IndexType deviceId, unsigned int sampleRate) :
                Functor(deviceId),
                sampleRate(sampleRate) {};

        virtual ~AudioFunctor() {};

    protected:
        unsigned int sampleRate;

    };

}

#endif //INSTRUMENT_TOOL_KIT_GENERATOR_H
