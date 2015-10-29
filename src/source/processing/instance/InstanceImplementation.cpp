#include "InstanceImplementation.h"

#include <boost/iterator/counting_iterator.hpp>
#include <algorithm>


namespace itk {

void InstanceImplementation::operator () (OutputChannels beginOut, OutputChannels endOut) {
    using std::cbegin;
    using std::cend;
    using std::copy;
    using std::fill;

    if (isOutputFunctorValid()) {
        initializeParameterMap();
        calculateControlParameter();
        calculateAudioOutput();
        auto left = cbegin(audioFunctorTable[_audioOutput]->left);
        auto right = cbegin(audioFunctorTable[_audioOutput]->right);
        auto left_end = cend(audioFunctorTable[_audioOutput]->left);
        auto right_end = cend(audioFunctorTable[_audioOutput]->right);
        copy(left, left_end, beginOut[0]);
        copy(right, right_end, beginOut[1]);
    } else {
        fill(beginOut[0], endOut[0], 0.0);
        fill(beginOut[1], endOut[1], 0.0);
    }
}

void InstanceImplementation::calculateAudioOutput() {
    using std::begin;
    using std::cbegin;
    using std::end;
    using std::cend;

    auto nodes = findAudioDeviceLeafs();
    while (nodes.size() > 0) {
        for (auto node : nodes) {
            auto wrapper = audioFunctorTable[node];
            auto beginOut = OutputChannels();
            beginOut[0] = begin(wrapper->left);
            beginOut[1] = begin(wrapper->right);
            auto endOut = OutputChannels();
            endOut[0] = end(wrapper->left);
            endOut[1] = end(wrapper->right);

            auto input = findAudioDevicePredecessors(node);
            auto endIn = InputChannels(std::max(input.size() * 2, 2UL));
            auto beginIn = InputChannels(std::max(input.size() * 2, 2UL));
            if (input.size() == 0) {
                beginIn[0] = begin(wrapper->left);
                beginIn[1] = begin(wrapper->left);
                endIn[0] = end(wrapper->left);
                endIn[1] = end(wrapper->left);
            } else {
                IndexType offset = 0;
                for (auto inputId : input) {
                    auto inNode = audioFunctorTable[inputId];
                    beginIn[offset] = begin(inNode->left);
                    beginIn[offset + 1] = begin(inNode->right);
                    endIn[offset] = end(inNode->left);
                    endIn[offset + 1] = end(inNode->right);
                    offset += 2;
                }
            }
            auto functor = wrapper->functor;
            auto inputParameterMap = createInputParameterMapForFunctorList<AudioFunctor>(*wrapper);
            (*functor)(beginIn, endIn, beginOut, endOut, *inputParameterMap);
        }
        nodes = findAudioDeviceChildren(begin(nodes), end(nodes));
    }
}

std::vector<IndexType> InstanceImplementation::findAudioDevicePredecessors(IndexType vertexId) {
    std::vector<IndexType> result;
    for (auto adjacencyList : audioDeviceEdges) {
        for (auto edge : adjacencyList) {
            if (edge.second == vertexId) {
                result.push_back(edge.first);
            }
        }
    }
    return result;
}

void InstanceImplementation::initializeParameterMap() {
    using std::fill;
    using std::begin;
    using std::end;
    for (auto parameterList : parameterMap) {
        auto deviceId = parameterList.first;
        auto parameters = parameterList.second;
        for (auto parameter : parameters) {
            auto parameterId = parameter.first;
            auto value = parameterTable.parameterValue(deviceId, parameterId);
            fill(begin(parameter.second->buffer), end(parameter.second->buffer), value);
        }
    }
}

void InstanceImplementation::calculateControlParameter() {
    using std::begin;
    using std::cbegin;
    using std::end;
    using std::cend;
    using std::copy;
    auto nodes = findControlDeviceLeafs();
    while (nodes.size() > 0) {
        for (auto node : nodes) {
            auto wrapper = controlFunctorTable[node];
            auto functor = wrapper->functor;
            auto inputParameterMap = createInputParameterMapForFunctorList<ControlFunctor>(*wrapper);
            (*functor)(begin(wrapper->control), end(wrapper->control), *inputParameterMap);
            distributeParametersAlongEdges(wrapper->index);
        }
        nodes = findControlDeviceChildren(begin(nodes), end(nodes));
    }
}

std::unordered_set<IndexType> InstanceImplementation::findAudioDeviceChildren(
        std::unordered_set<IndexType>::iterator begin,
        std::unordered_set<IndexType>::iterator end) {

    std::vector<IndexType> result;
    for (auto it = begin; it != end; ++it) {
        auto children = findChildren(audioDeviceEdges[*it]);
        for (auto child : children) {
            result.push_back(child);
        }
    }
    return std::unordered_set<IndexType>(std::cbegin(result), std::cend(result));
}

std::vector<IndexType> InstanceImplementation::findChildren(AudioEdgeList edgeList) {
    std::vector<IndexType> result;
    for (auto edge : edgeList) {
        result.push_back(edge.second);
    }
    return result;
}

std::vector<IndexType> InstanceImplementation::findChildren(ControlEdgeList edgeList) {
    std::vector<IndexType> vertex;
    for (auto edge : edgeList) {
        vertex.push_back(edge.second.deviceId);
    }
    return vertex;
}

std::vector<IndexType> InstanceImplementation::findControlDeviceChildren(std::vector<IndexType>::iterator begin,
                                                                         std::vector<IndexType>::iterator end) {

    std::vector<IndexType> result;
    for (auto it = begin; it != end; ++it) {
        auto children = findChildren(controlDeviceEdges[*it]);
        for (auto child : children) {
            result.push_back(child);
        }
    }
    return result;
}

void InstanceImplementation::distributeParametersAlongEdges(IndexType vertexId) {
    using std::begin;
    using std::cbegin;
    using std::end;
    using std::cend;
    using std::copy;
    auto outEdges = controlDeviceEdges[vertexId];
    for (auto edge : outEdges) {
        auto sourceDeviceId = edge.first;
        auto targetDeviceId = edge.second.deviceId;
        auto targetParameterId = edge.second.parameterId;
        auto sourceBegin = cbegin(controlFunctorTable[sourceDeviceId]->control);
        auto sourceEnd = cend(controlFunctorTable[sourceDeviceId]->control);
        auto targetBegin = begin(parameterMap[targetDeviceId][targetParameterId]->buffer);
        copy(sourceBegin, sourceEnd, targetBegin);
    }
}

std::unordered_set<IndexType> InstanceImplementation::findAudioDeviceLeafs() {
    using std::unordered_set;
    using std::begin;
    using std::end;
    using boost::counting_iterator;
    auto leafs = unordered_set<IndexType>(
            counting_iterator<IndexType>(0),
            counting_iterator<IndexType>(audioFunctorTable.size()));

    for (auto adjacencyList : audioDeviceEdges) {
        for (auto edge : adjacencyList) {
            leafs.erase(edge.second);
        }
    }
    return leafs;
}

std::vector<IndexType> InstanceImplementation::findControlDeviceLeafs() {
    using std::unordered_set;
    using std::begin;
    using std::end;
    using boost::counting_iterator;
    auto leafs = unordered_set<IndexType>(
            counting_iterator<IndexType>(0),
            counting_iterator<IndexType>(controlFunctorTable.size()));
/*
    for (auto adjacencyList : controlDeviceEdges) {
        for (auto edge : adjacencyList) {
            leafs.erase(edge.second.deviceId);
        }
    }
 */
    return std::vector<IndexType>(cbegin(leafs), cend(leafs));
}

void InstanceImplementation::allocateMemory() {
    allocateParameterMemory();
    allocateDeviceMemory();
}

void InstanceImplementation::allocateParameterMemory() {
    for (auto parameterList : parameterMap) {
        for (auto parameter : parameterList.second) {
            parameter.second->buffer = DataBuffer(_bufferSize);
        }
    }
}

void InstanceImplementation::allocateDeviceMemory() {
    for (auto audioFunctor : audioFunctorTable) {
        audioFunctor->left = DataBuffer(_bufferSize);
        audioFunctor->right = DataBuffer(_bufferSize);
    }

    for (auto controlFunctor : controlFunctorTable) {
        controlFunctor->control = DataBuffer(_bufferSize);
    }
}

void InstanceImplementation::bufferSize(unsigned int bufferSize) {
    _bufferSize = bufferSize;
}

IndexType InstanceImplementation::addAudioFunctorList(AudioFunctorList::Ptr functorList) {
    using namespace std;

    auto functorWrapper = make_shared<FunctorWrapper<AudioFunctorList>>();
    functorWrapper->functor = functorList;
    functorWrapper->index = audioFunctorTable.size();
    audioFunctorTable.push_back(functorWrapper);

    auto functorListParameters = functorList->allParameters();
    for (auto functorParameters : functorListParameters) {
        auto deviceId = functorParameters.first;
        auto parameterList = functorParameters.second;
        for (auto parameter : parameterList) {
            auto parameterId = parameter.id();
            auto wrapper = new ParameterWrapper();
            parameterMap[deviceId][parameterId] = ParameterWrapper::Ptr(wrapper);
        }
    }

    audioDeviceEdges.push_back(AudioEdgeList());
    return functorWrapper->index;
}

IndexType InstanceImplementation::addControlFunctorList(ControlFunctorList::Ptr functorList) {
    using namespace std;

    auto functorWrapper = make_shared<FunctorWrapper<ControlFunctorList>>();
    functorWrapper->functor = functorList;
    functorWrapper->index = controlFunctorTable.size();
    controlFunctorTable.push_back(functorWrapper);

    auto functorListParameters = functorList->allParameters();
    for (auto functorParameters : functorListParameters) {
        auto deviceId = functorParameters.first;
        auto parameterList = functorParameters.second;
        for (auto parameter : parameterList) {
            auto parameterId = parameter.id();
            auto wrapper = new ParameterWrapper();
            parameterMap[deviceId][parameterId] = ParameterWrapper::Ptr(wrapper);
        }
    }

    controlDeviceEdges.push_back(ControlEdgeList());
    return functorWrapper->index;
}

void InstanceImplementation::connect(IndexType sourceId, IndexType targetId) {
    auto edge = AudioEdge(sourceId, targetId);
    audioDeviceEdges[sourceId].push_back(edge);
}

void InstanceImplementation::connect(IndexType sourceId, IndexType targetId, IndexType parameterId) {
    auto edgeEnd = ControlEdgeEnd();
    edgeEnd.deviceId = sourceId;
    edgeEnd.parameterId = parameterId;
    auto edge = ControlEdge(sourceId, edgeEnd);
    controlDeviceEdges[sourceId].push_back(edge);
}

void InstanceImplementation::connectOutput(IndexType sourceId) {
    _audioOutput = sourceId;
}

}
