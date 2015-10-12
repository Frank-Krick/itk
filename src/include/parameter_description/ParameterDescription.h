#ifndef INSTRUMENT_TOOL_KIT_PARAMETERDESCRIPTION_H
#define INSTRUMENT_TOOL_KIT_PARAMETERDESCRIPTION_H

#include <Typedef.h>

#include <memory>
#include <string>

namespace itk {

class ParameterDescription {
public:
    ParameterDescription(
            IndexType id,
            const std::string &name,
            const std::string &description,
            DataType max,
            DataType min)
            : _id(id), _name(name), _description(description), _max(max), _min(min) {}

    IndexType id() const { return _id; }
    std::string name() const { return _name; }
    std::string description() const { return _description; }
    DataType max() const { return _max; }
    DataType min() const { return _min; }

private:
    IndexType _id = 0;
    std::string _name;
    std::string _description;
    DataType _max = 0.0;
    DataType _min = 0.0;
};

bool operator< (const ParameterDescription &lhs, const ParameterDescription &rhs);
bool operator== (const ParameterDescription &lhs, const ParameterDescription &rhs);

}

#endif //INSTRUMENT_TOOL_KIT_PARAMETERDESCRIPTION_H
