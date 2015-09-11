//
// Created by Frank Krick on 9/7/15.
//
#ifndef INSTRUMENT_TOOL_KIT_SINEGENERATOR_H
#define INSTRUMENT_TOOL_KIT_SINEGENERATOR_H

#include <processing/generator/Generator.h>

namespace itk {
    /*
     * Generates sine waves using the standard sin() function.
     */
    class SineGenerator : public Generator {
    public:
        virtual void operator () (Iterator begin, Iterator end, ParameterMap &parameter) override;

        virtual ParameterList parameterList() override;

        virtual bool operator==(const Generator &generator) override;

        virtual void reset() override;
        virtual Ptr clone() override;
        virtual Ptr create() override;

        SineGenerator(unsigned int sampleRate);

    private:
        double phaseOffset = 0.0;
    };

}

#endif //INSTRUMENT_TOOL_KIT_SINEGENERATOR_H
