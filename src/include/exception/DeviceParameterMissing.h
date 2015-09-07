//
// Created by Frank Krick on 9/6/15.
//

#ifndef INSTRUMENT_TOOL_KIT_DEVICEPARAMETERMISSING_H
#define INSTRUMENT_TOOL_KIT_DEVICEPARAMETERMISSING_H

#include <Typedef.h>

#include <exception>

namespace itk {

    class DeviceParameterMissing : std::exception {
    public:
        DeviceParameterMissing(IndexType deviceId, IndexType parameterId);

        IndexType getParameterId();
        IndexType getDeviceId();

        virtual const char *what() const noexcept override;

    private:
        IndexType parameterId;
        IndexType deviceId;
    };

}

#endif //INSTRUMENT_TOOL_KIT_DEVICEPARAMETERMISSING_H
