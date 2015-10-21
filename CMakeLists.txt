cmake_minimum_required(VERSION 3.1.3)

project(instrument_tool_kit)
set(itk_VERSION_MAJOR 0)
set(itk_VERSION_MINOR 1)

set(CLANG_CXX_FLAGS "-std=c++1y -Weverything -Wno-c++98-compat -Wno-padded -Wno-shadow -Wno-unused-macros -Wno-missing-prototypes -Wno-unused-parameter -Wno-c++98-compat-pedantic -Wno-missing-prototypes")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_CXX_FLAGS}")

set(PYTHON_LIBRARY
        /usr/local/Cellar/python/2.7.9/Frameworks/Python.framework/Versions/2.7/lib/libpython2.7.dylib)

find_package(Boost 1.58.0)
if(Boost_FOUND)
    SET(Boost_USE_STATIC_LIBS OFF)
    SET(Boost_USE_MULTITHREADED ON)
    SET(Boost_USE_STATIC_RUNTIME OFF)
    FIND_PACKAGE(Boost 1.45.0 COMPONENTS python)
elseif(NOT Boost_FOUND)
    MESSAGE(FATAL_ERROR "Coulnd't find boost.")
endif()

include_directories(
        ${Boost_INCLUDE_DIRS}
        /usr/local/Cellar/python/2.7.9/Frameworks/Python.framework/Headers
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
        src/include/processing/generator/ControlFunctor.h
        src/source/device/DeviceRegistry.cpp
        src/include/device/DeviceRegistry.h
        src/source/processing/generator/Functor.cpp
        src/source/parameter_table/ParameterTable.cpp
        src/source/processing/instance/Instance.cpp)

set(PYTHON_WRAPPER
        src/wrapper/itk.cpp
        src/wrapper/device/PyDeviceRegistry.cpp
        src/wrapper/device/PyDeviceRegistry.h
        src/wrapper/device_graph/PyDeviceGraph.cpp
        src/wrapper/device_graph/PyDeviceGraph.h
        src/wrapper/device/PyDevice.cpp
        src/wrapper/device/PyDevice.h)

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
        test/instance/InstanceImplementationTests.cpp
        test/test_data/device/InputCopyAudioFunctor.cpp
        test/test_data/device/InputCopyAudioFunctor.h)

add_executable(instrument_tool_kit src/source/main.cpp ${SOURCE_FILES})

add_executable(itk_tests ${TEST_FILES} ${SOURCE_FILES})

set(MY_BOOST_PYTHON /Users/frank/Development/boost/python/boost.python/build/Debug/libboost_python.a)

set(LIBRARY_OUTPUT_PATH
        /Users/frank/Development/itk_full/itk/build/Debug/itk.so)


add_library(itk_python SHARED ${PYTHON_WRAPPER} ${SOURCE_FILES})
target_link_libraries(itk_python
        ${MY_BOOST_PYTHON}
        ${PYTHON_LIBRARY})
set_target_properties(itk_python PROPERTIES OUTPUT_NAME itk)
set_target_properties(itk_python PROPERTIES PREFIX "")
set_target_properties(itk_python PROPERTIES SUFFIX .so)
