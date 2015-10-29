#ifndef INSTRUMENT_TOOL_KIT_RAMPCONTROLFUNCTOR_H
#define INSTRUMENT_TOOL_KIT_RAMPCONTROLFUNCTOR_H

#include <processing/generator/ControlFunctor.h>


using namespace itk;

class RampControlFunctor : public ControlFunctor {

public:
    virtual void operator()(DataBuffer::iterator outputBegin, DataBuffer::iterator outputEnd,
                            ParameterDeviceMap &parameter);

    virtual ParameterList parameterList();

    virtual Functor::Ptr clone(IndexType deviceId);

    RampControlFunctor(IndexType deviceId, unsigned int sampleRate, IndexType rampSampleLength)
            : ControlFunctor(deviceId, _sampleRate), _rampSampleLength(rampSampleLength) {}

private:
    IndexType _rampSampleLength;
};


#endif //INSTRUMENT_TOOL_KIT_RAMPCONTROLFUNCTOR_H
