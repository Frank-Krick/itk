//
// Created by Frank Krick on 9/2/15.
//

#ifndef INSTRUMENT_TOOL_KIT_DEVICEGRAPHINSTANCE_H
#define INSTRUMENT_TOOL_KIT_DEVICEGRAPHINSTANCE_H

#include <memory>

namespace itk {

    class DeviceGraphInstance {
    public:
        typedef std::shared_ptr<DeviceGraphInstance> Ptr;

        ~DeviceGraphInstance() { };
    };

}


#endif //INSTRUMENT_TOOL_KIT_DEVICEGRAPHINSTANCE_H
