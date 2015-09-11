//
// Created by Frank Krick on 9/7/15.
//
#ifndef INSTRUMENT_TOOL_KIT_SINEGENERATOR_H
#define INSTRUMENT_TOOL_KIT_SINEGENERATOR_H

#include <processing/generator/AudioFunctor.h>

namespace itk {
    /*
     * Generates sine waves using the standard sin() function.
     */
    class SineGenerator : public AudioFunctor {
    public:
        virtual void operator () (
                InputChannels beginIn, InputChannels endIn,
                OutputChannels beginOut, OutputChannels endOut,
                ParameterMap & parameter);

        virtual ParameterList parameterList() override;

        virtual bool operator==(const AudioFunctor &generator) override;

        virtual void reset() override;
        virtual Ptr clone() override;
        virtual Ptr create() override;

        SineGenerator(unsigned int sampleRate);

    private:
        double phaseOffset = 0.0;
    };

}

#endif //INSTRUMENT_TOOL_KIT_SINEGENERATOR_H
