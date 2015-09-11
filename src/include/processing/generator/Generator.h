//
// Created by Frank Krick on 9/7/15.
//

#ifndef INSTRUMENT_TOOL_KIT_GENERATOR_H
#define INSTRUMENT_TOOL_KIT_GENERATOR_H

#include <Typedef.h>
#include <processing/Parameter.h>
#include <parameter_description/ParameterDescription.h>

#include <array>
#include <unordered_map>
#include <list>

namespace itk {
    /*
     * The functor Generator provides the base class for generating functions
     * like sine or square generators.
     */
    class Generator {
    public:
        typedef std::shared_ptr<Generator> Ptr;
        typedef std::list<ParameterDescription> ParameterList;
        typedef DataBuffer::iterator Iterator;
        /*
         * Called to generate the data. The output is written into the
         * range defined byy begin and end. ParameterMap contains iterators
         * that enable access to all requested parameters.
         * The iterator for each parameter is guaranteed to not reach end
         * until the range defined by begin and end ends.
         */
        virtual void operator () (Iterator begin, Iterator end, ParameterMap & parameter) = 0;
        /*
         * Returns true if the sequence generated by the generator
         * would be the same given the same input (Two iterators with
         * the same distance and the same parameters).
         */
        virtual bool operator == (const Generator &) = 0;
        virtual bool operator != (const Generator &);
        /*
         * Returns the supported parameter.
         */
        virtual ParameterList parameterList() = 0;
        /*
         * Returns a copy of self.
         */
        virtual Ptr clone() = 0;
        /*
         * Creates a new Generator with initial values.
         */
        virtual Ptr create() = 0;
        /*
         * Sets the initial values for all member variables.
         */
        virtual void reset() = 0;

        Generator(unsigned int sampleRate) { this->sampleRate = sampleRate; };
        virtual ~Generator() {};

    protected:
        unsigned int sampleRate = 0;
    };

}

#endif //INSTRUMENT_TOOL_KIT_GENERATOR_H
