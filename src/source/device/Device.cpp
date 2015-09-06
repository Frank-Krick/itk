//
// Created by Frank Krick on 9/5/15.
//

#include <device/Device.h>

namespace itk {

    std::ostream & operator << (std::ostream & stream, const DeviceType & type) {
        switch (type) {
            case DeviceType::AUDIO:
                stream << "Audio";
                break;
            case DeviceType::CONTROL:
                stream << "Control";
                break;
        }
        return stream;
    }

}