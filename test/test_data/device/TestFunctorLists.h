//
// Created by Frank Krick on 9/15/15.
//

#ifndef INSTRUMENT_TOOL_KIT_TESTFUNCTORLISTS_H
#define INSTRUMENT_TOOL_KIT_TESTFUNCTORLISTS_H

#include <processing/functor_list/FunctorList.h>

using namespace itk;

class TestFunctorLists {
public:
    static FunctorList<AudioFunctor::Ptr>::Ptr audioFunctorList(std::initializer_list<AudioFunctor::Ptr> functors);
};


#endif //INSTRUMENT_TOOL_KIT_TESTFUNCTORLISTS_H
