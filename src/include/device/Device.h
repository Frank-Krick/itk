//
// Created by Frank Krick on 9/2/15.
//

#ifndef INSTRUMENT_TOOL_KIT_DEVICE_H
#define INSTRUMENT_TOOL_KIT_DEVICE_H

#include <Typedef.h>
#include <parameter_description/ParameterDescription.h>
#include <processing/generator/Functor.h>

#include <ostream>
#include <memory>
#include <set>

namespace itk {

enum class DeviceType { AUDIO, CONTROL };

std::ostream & operator << (std::ostream & stream, const DeviceType & type);

struct Device {
public:
    typedef std::shared_ptr<Device> Ptr;
    typedef ParameterList::iterator Iter;

    virtual std::tuple<Iter, Iter> availableParameters() = 0;
    virtual Functor::Ptr functor(IndexType deviceId) = 0;

    std::string name() { return _name; }
    std::string description() { return _description; };
    DeviceType deviceType() { return _deviceType; };

    virtual ~Device() {};

protected:
    std::string _name = "";
    std::string _description = "";
    DeviceType _deviceType = DeviceType::AUDIO;
};

}

#endif //INSTRUMENT_TOOL_KIT_DEVICE_H
