//
// Created by Frank Krick on 9/12/15.
//

#ifndef INSTRUMENT_TOOL_KIT_PARAMETERCOPYAUDIOFUNCTOR_H
#define INSTRUMENT_TOOL_KIT_PARAMETERCOPYAUDIOFUNCTOR_H

#include <processing/generator/AudioFunctor.h>

using namespace itk;

class ParameterCopyAudioFunctor : AudioFunctor {
public:
    virtual void operator () (
            InputChannels beginIn, InputChannels endIn,
            OutputChannels beginOut, OutputChannels endOut,
            ParameterMap &parameter) override;

    virtual bool operator==(const AudioFunctor &functor) override;

    virtual ParameterList parameterList() override;

    virtual Ptr clone() override;
    virtual void reset() override;

    ParameterCopyAudioFunctor(IndexType deviceId, IndexType numParameters,
                              IndexType sourceParameterId, unsigned int sampleRate);

private:
    IndexType sourceParameterId = 0;
    IndexType numParameters = 0;
};


#endif //INSTRUMENT_TOOL_KIT_PARAMETERCOPYAUDIOFUNCTOR_H
