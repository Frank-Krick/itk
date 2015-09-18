//
// Created by Frank Krick on 9/15/15.
//

#ifndef INSTRUMENT_TOOL_KIT_TESTFUNCTORS_H
#define INSTRUMENT_TOOL_KIT_TESTFUNCTORS_H

#include <processing/generator/AudioFunctor.h>

using namespace itk;

class TestFunctors {
public:
    static AudioFunctor::Ptr createParameterCopyFunctor(IndexType deviceId, IndexType numParameters,
                                                        IndexType parameterId, unsigned int sampleRate);

    static AudioFunctor::Ptr inputCopyFunctor(IndexType deviceId, unsigned int sampleRate);
};


#endif //INSTRUMENT_TOOL_KIT_TESTFUNCTORS_H
