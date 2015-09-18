//
// Created by Frank Krick on 9/4/15.
//

#ifndef INSTRUMENT_TOOL_KIT_DEVICEIMPLEMENTATION_H
#define INSTRUMENT_TOOL_KIT_DEVICEIMPLEMENTATION_H

#include <device/Device.h>
#include <unordered_set>
#include <processing/generator/Functor.h>

namespace itk {

    class DeviceImplementation : public Device {
    public:
        virtual std::tuple<Iter, Iter> availableParameters();
        virtual DeviceType deviceType() override;
        void deviceType(DeviceType type);
        void functor(Functor::Ptr functor);
        virtual Functor::Ptr functor(IndexType deviceId) override;

    private:
        DeviceType type = DeviceType::AUDIO;
        Functor::Ptr _functor;
        ParameterList parameters;
    };

}

#endif //INSTRUMENT_TOOL_KIT_DEVICEIMPLEMENTATION_H
