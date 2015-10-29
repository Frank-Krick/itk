#ifndef INSTRUMENT_TOOL_KIT_CONSTANTADDAUDIOFUNCTOR_H
#define INSTRUMENT_TOOL_KIT_CONSTANTADDAUDIOFUNCTOR_H

#include <processing/generator/AudioFunctor.h>


using namespace itk;

class ConstantAddAudioFunctor : public AudioFunctor {
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

    ConstantAddAudioFunctor(IndexType deviceId, unsigned int sampleRate, DataType constant);

private:
    DataType _constant;
};


#endif //INSTRUMENT_TOOL_KIT_CONSTANTADDAUDIOFUNCTOR_H
