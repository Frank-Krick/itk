//
// Created by Frank Krick on 9/12/15.
//
#ifndef INSTRUMENT_TOOL_KIT_FUNCTOR_H
#define INSTRUMENT_TOOL_KIT_FUNCTOR_H

#include <Typedef.h>

#include <memory>

namespace itk {

class Functor {
public:
    typedef std::shared_ptr<Functor> Ptr;
    virtual ParameterList parameterList() = 0;
    virtual Ptr clone(IndexType deviceId) = 0;
    virtual IndexType deviceId() const;

    Functor(IndexType deviceId) : _deviceId(deviceId) {}
    Functor(const Functor &functor) : _deviceId(functor.deviceId()) {}
    virtual ~Functor() {}

protected:
    IndexType _deviceId;
};

}

#endif //INSTRUMENT_TOOL_KIT_FUNCTOR_H
