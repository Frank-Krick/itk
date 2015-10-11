#include <device_graph/DeviceGraph.h>

namespace itk {

std::ostream & operator << (std::ostream & stream, const ConnectionType & type) {
    switch (type) {
        case ConnectionType::AUDIO:
            stream << "Audio";
            break;
        case ConnectionType::CONTROL:
            stream << "Control";
            break;
    }
    return stream;
}

}