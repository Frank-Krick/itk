//
// Created by Frank Krick on 9/10/15.
//

#ifndef INSTRUMENT_TOOL_KIT_INSTANCE_H
#define INSTRUMENT_TOOL_KIT_INSTANCE_H

#include <Typedef.h>
#include <device/Device.h>
#include <processing/generator/AudioFunctor.h>
#include <processing/functor_list/FunctorList.h>

#include <list>
#include <array>

namespace itk {

    class Instance {
    public:
        typedef std::shared_ptr<Instance> Ptr;

        virtual void operator () (OutputChannels beginOut, OutputChannels endOut) = 0;
    };

}

#endif //INSTRUMENT_TOOL_KIT_INSTANCE_H
