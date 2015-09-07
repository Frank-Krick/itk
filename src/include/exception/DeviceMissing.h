//
// Created by Frank Krick on 9/6/15.
//

#ifndef INSTRUMENT_TOOL_KIT_DEVICEMISSING_H
#define INSTRUMENT_TOOL_KIT_DEVICEMISSING_H

#include <Typedef.h>

#include <exception>

namespace itk {

    class DeviceMissing : std::exception {
    public:
        DeviceMissing(IndexType deviceId);

        IndexType getDeviceId();

        virtual const char *what() const noexcept override;

    private:
        IndexType deviceId;
    };

}

#endif //INSTRUMENT_TOOL_KIT_DEVICEMISSING_H
