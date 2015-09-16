//
// Created by Frank Krick on 9/4/15.
//

#ifndef INSTRUMENT_TOOL_KIT_DEVICEIMPLEMENTATION_H
#define INSTRUMENT_TOOL_KIT_DEVICEIMPLEMENTATION_H

#include <device/Device.h>
#include <unordered_set>
#include <processing/generator/Functor.h>

namespace itk {

    class DeviceImplementation : Device {
    public:
        virtual std::tuple<Iter, Iter> availableParameters();
        virtual DeviceType deviceType() override;
        void deviceType(DeviceType type);
        void setFunctor(Functor::Ptr functor);

    private:
        DeviceType type = DeviceType::AUDIO;
        Functor::Ptr functor;
        ParameterList parameters;
    };

}

#endif //INSTRUMENT_TOOL_KIT_DEVICEIMPLEMENTATION_H
