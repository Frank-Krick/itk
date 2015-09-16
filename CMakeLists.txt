cmake_minimum_required(VERSION 3.3)

project(instrument_tool_kit)
set(itk_VERSION_MAJOR 0)
set(itk_VERSION_MINOR 1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

include_directories(
        /Users/frank/Development/c++/libraries/boost_1_59_0
        src/include)

set(SOURCE_FILES
        src/include/device_graph/DeviceGraph.h
        src/include/device_graph/DeviceGraphInstance.h
        src/include/device/Device.h
        src/source/device_graph/DeviceGraphImplementation.cpp
        src/source/device_graph/DeviceGraphImplementation.h
        src/source/device_graph/DeviceGraphFactory.cpp
        src/include/device_graph/DeviceGraphFactory.h
        src/include/parameter_table/ParameterTable.h
        src/source/parameter_table/ParameterTableImplementation.cpp
        src/source/parameter_table/ParameterTableImplementation.h
        src/include/parameter_description/ParameterDescription.h
        src/source/device/DeviceImplementation.cpp
        src/source/device/DeviceImplementation.h
        src/source/parameter_description/ParameterDescription.cpp
        src/include/parameter_description/ParameterDescription.h
        src/source/device/DeviceFactory.cpp
        src/include/device/DeviceFactory.h
        src/source/exception/DeviceTypeMismatch.cpp
        src/include/exception/DeviceTypeMismatch.h
        src/source/device/Device.cpp
        src/source/device_graph/DeviceGraph.cpp
        src/include/Typedef.h
        src/source/exception/DeviceParameterMissing.cpp
        src/include/exception/DeviceParameterMissing.h
        src/source/exception/DeviceMissing.cpp
        src/include/exception/DeviceMissing.h
        src/include/processing/generator/AudioFunctor.h
        src/include/processing/Parameter.h
        src/source/processing/generator/SineGenerator.cpp
        src/source/processing/generator/SineGenerator.h
        src/source/Utility.cpp src/include/Utility.h
        src/source/processing/generator/AudioFunctor.cpp
        src/include/processing/instance/Instance.h
        src/include/processing/functor_list/FunctorList.h
        src/include/processing/generator/Functor.h
        src/source/processing/instance/InstanceImplementation.cpp
        src/source/processing/instance/InstanceImplementation.h
        src/include/processing/generator/ControlFunctor.cpp
        src/include/processing/generator/ControlFunctor.h)

set(TEST_FILES
        test/device_graph/DeviceGraphTests.cpp
        test/device/DeviceTests.cpp
        test/processing/generator/SineGeneratorTests.cpp
        test/processing/functor_list/FunctorListTests.cpp
        test/parameter_table/ParameterTableTests.cpp
        test/test_main.cpp
        test/test_data/device/TestDevices.cpp
        test/test_data/device/TestDevices.h
        test/test_data/device/ParameterCopyAudioFunctor.cpp
        test/test_data/device/ParameterCopyAudioFunctor.h
        test/test_data/device/TestParameter.cpp
        test/test_data/device/TestParameter.h
        test/instance/InstanceTests.cpp
        test/test_data/instance/TestInstances.cpp
        test/test_data/instance/TestInstances.h
        test/test_data/device/TestFunctors.cpp
        test/test_data/device/TestFunctors.h
        test/test_data/device/TestFunctorLists.cpp
        test/test_data/device/TestFunctorLists.h
        test/instance/InstanceImplementationTests.cpp)

add_executable(instrument_tool_kit src/source/main.cpp ${SOURCE_FILES})
add_executable(itk_tests ${TEST_FILES} ${SOURCE_FILES})