//
// Created by Frank Krick on 9/16/15.
//

#ifndef INSTRUMENT_TOOL_KIT_INPUTCOPYAUDIOFUNCTOR_H
#define INSTRUMENT_TOOL_KIT_INPUTCOPYAUDIOFUNCTOR_H

#include <processing/generator/AudioFunctor.h>

using namespace itk;

class InputCopyAudioFunctor : public AudioFunctor {
public:
    virtual void operator () (
            InputChannels beginIn, InputChannels endIn,
            OutputChannels beginOut, OutputChannels endOut,
            ParameterMap &parameter) override;

    virtual bool operator==(const AudioFunctor &functor) override;

    virtual ParameterList parameterList() override;

    virtual Ptr clone() override;

    virtual Functor::Ptr clone(IndexType deviceId) override;

    virtual void reset() override;

    InputCopyAudioFunctor(IndexType deviceId, unsigned int sampleRate);
};


#endif //INSTRUMENT_TOOL_KIT_INPUTCOPYAUDIOFUNCTOR_H
