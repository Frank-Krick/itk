//
// Created by Frank Krick on 9/15/15.
//

#include "TestFunctorLists.h"

FunctorList<AudioFunctor::Ptr>::Ptr TestFunctorLists::audioFunctorList(
        std::initializer_list<AudioFunctor::Ptr> functors) {

    auto functorList = new FunctorList<AudioFunctor::Ptr>();
    for (auto functor : functors) {
        functorList->push_back(functor);
    }
    return FunctorList<AudioFunctor::Ptr>::Ptr(functorList);
}
