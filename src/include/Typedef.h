//
// Created by Frank Krick on 9/5/15.
//

#ifndef INSTRUMENT_TOOL_KIT_TYPEDEF_H
#define INSTRUMENT_TOOL_KIT_TYPEDEF_H

#include <vector>
#include <list>

namespace itk {

    class ParameterDescription;

    typedef unsigned int IndexType;
    typedef double DataType;
    typedef std::vector<DataType> DataBuffer;
    typedef std::list<ParameterDescription> ParameterList;
    typedef DataBuffer::iterator Iterator;
    typedef DataBuffer::const_iterator ConstIterator;
    typedef std::array<Iterator, 2> OutputChannels;
    typedef std::array<ConstIterator, 2> InputChannels;

}

#endif //INSTRUMENT_TOOL_KIT_TYPEDEF_H
