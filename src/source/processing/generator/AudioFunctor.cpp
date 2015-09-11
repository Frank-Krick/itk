//
// Created by Frank Krick on 9/10/15.
//

#include <processing/generator/AudioFunctor.h>

namespace itk {

    bool AudioFunctor::operator != (const AudioFunctor &generator) {
        return ! this->operator==(generator);
    }

}
