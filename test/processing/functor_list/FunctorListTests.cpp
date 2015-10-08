#include <boost/test/unit_test.hpp>

#include <Typedef.h>
#include <processing/functor_list/FunctorList.h>

using namespace itk;

using std::begin;
using std::end;
using std::equal;

class TestFunctor {
public:
    typedef std::shared_ptr<TestFunctor> Ptr;

    IndexType deviceId() { return _deviceId; };
    void operator () (
            InputChannels beginIn, InputChannels endIn,
            OutputChannels beginOut, OutputChannels endOut,
            ParameterMap & parameter) {

        for (auto it = beginOut[0]; it != endOut[0]; ++it) {
            *it = *it + 1.0;
        }

        for (auto it = beginOut[1]; it != endOut[1]; ++it) {
            *it = *it + 1.0;
        }
    }

    TestFunctor(IndexType deviceId) : _deviceId(deviceId) {};

private:
    IndexType _deviceId;
};

BOOST_AUTO_TEST_SUITE( FunctorListTests )

    BOOST_AUTO_TEST_CASE( functor_list_should_evaluate_functors ) {
        auto functorList = FunctorList<TestFunctor::Ptr>();
        functorList.push_back(TestFunctor::Ptr(new TestFunctor(0)));
        functorList.push_back(TestFunctor::Ptr(new TestFunctor(1)));
        functorList.push_back(TestFunctor::Ptr(new TestFunctor(2)));
        functorList.push_back(TestFunctor::Ptr(new TestFunctor(3)));
        functorList.push_back(TestFunctor::Ptr(new TestFunctor(4)));
        auto left = DataBuffer(5000);
        auto right = DataBuffer(5000);
        OutputChannels dataBegin { {begin(left), begin(right)} };
        OutputChannels dataEnd { {end(left), end(right)} };
        InputChannels inDataBegin { {begin(left), begin(right)} };
        InputChannels inDataEnd { {end(left), end(right)} };
        AudioFunctor::ParameterDeviceMap parameterMap;
        functorList(inDataBegin, inDataEnd, dataBegin, dataEnd, parameterMap);
        auto compareValue = std::vector<double>(5000);
        for (auto it = begin(compareValue); it != end(compareValue); ++it) {
            *it = 5.0;
        }
        BOOST_CHECK(equal(dataBegin[0], dataEnd[0], begin(compareValue), end(compareValue)));
        BOOST_CHECK(equal(dataBegin[1], dataEnd[1], begin(compareValue), end(compareValue)));
    }

BOOST_AUTO_TEST_SUITE_END()