cmake_minimum_required(VERSION 2.8.12)
project(CPlusPlus)

add_definitions("-std=c++11")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup()

add_executable(check_ip check_ip.cpp src/StorageFile.cpp src/StorageFile.h src/IPAddressStorageFile.h src/IPAddressStorageFile.cpp src/NetworkService.cpp src/NetworkService.h src/IPAddressNetworkService.cpp src/IPAddressNetworkService.h src/Exceptions.h)

target_link_libraries(check_ip ${CONAN_LIBS})
