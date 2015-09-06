//
// Created by Frank Krick on 9/4/15.
//

#ifndef INSTRUMENT_TOOL_KIT_DEVICEIMPLEMENTATION_H
#define INSTRUMENT_TOOL_KIT_DEVICEIMPLEMENTATION_H

#include <device/Device.h>
#include <unordered_set>

namespace itk {

    class DeviceImplementation : Device {
    public:

        virtual std::tuple<Iter, Iter> availableParameters();
        virtual void addParameter(ParameterDescription &parameter);

        virtual DeviceType deviceType() override;
        void deviceType(DeviceType type);

    private:

        Parameters parameters;
        DeviceType type = DeviceType::AUDIO;

    };

}

#endif //INSTRUMENT_TOOL_KIT_DEVICEIMPLEMENTATION_H
