//
// Created by Frank Krick on 9/10/15.
//

#include <processing/generator/Generator.h>

namespace itk {

    bool Generator::operator != (const Generator &generator) {
        return ! this->operator==(generator);
    }

}
