#ifndef INSTRUMENT_TOOL_KIT_INPUTADDAUDIOFUNCTOR_H
#define INSTRUMENT_TOOL_KIT_INPUTADDAUDIOFUNCTOR_H

#include <processing/generator/AudioFunctor.h>


using namespace itk;

class InputAddAudioFunctor : public AudioFunctor {
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

    InputAddAudioFunctor(IndexType deviceId, unsigned int sampleRate);
};


#endif //INSTRUMENT_TOOL_KIT_INPUTADDAUDIOFUNCTOR_H
