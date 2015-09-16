//
// Created by Frank Krick on 9/12/15.
//
#ifndef INSTRUMENT_TOOL_KIT_FUNCTOR_H
#define INSTRUMENT_TOOL_KIT_FUNCTOR_H

namespace itk {

    class Functor {
    public:
        typedef std::shared_ptr<Functor> Ptr;
        virtual ParameterList parameterList() = 0;

        virtual IndexType deviceId() { return this->_deviceId; }

        Functor(IndexType deviceId) : _deviceId(deviceId) {}

    protected:
        IndexType _deviceId;
    };

}

#endif //INSTRUMENT_TOOL_KIT_FUNCTOR_H
