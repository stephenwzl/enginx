cmake_minimum_required(VERSION 3.6)
project(enginx)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    enginx/core/vendor/http_parser/http_parser.c
    enginx/core/vendor/http_parser/http_parser.h
    enginx/core/vendor/rapidjson/error/en.h
    enginx/core/vendor/rapidjson/error/error.h
    enginx/core/vendor/rapidjson/internal/biginteger.h
    enginx/core/vendor/rapidjson/internal/diyfp.h
    enginx/core/vendor/rapidjson/internal/dtoa.h
    enginx/core/vendor/rapidjson/internal/ieee754.h
    enginx/core/vendor/rapidjson/internal/itoa.h
    enginx/core/vendor/rapidjson/internal/meta.h
    enginx/core/vendor/rapidjson/internal/pow10.h
    enginx/core/vendor/rapidjson/internal/regex.h
    enginx/core/vendor/rapidjson/internal/stack.h
    enginx/core/vendor/rapidjson/internal/strfunc.h
    enginx/core/vendor/rapidjson/internal/strtod.h
    enginx/core/vendor/rapidjson/internal/swap.h
    enginx/core/vendor/rapidjson/msinttypes/inttypes.h
    enginx/core/vendor/rapidjson/msinttypes/stdint.h
    enginx/core/vendor/rapidjson/allocators.h
    enginx/core/vendor/rapidjson/document.h
    enginx/core/vendor/rapidjson/encodedstream.h
    enginx/core/vendor/rapidjson/encodings.h
    enginx/core/vendor/rapidjson/filereadstream.h
    enginx/core/vendor/rapidjson/filewritestream.h
    enginx/core/vendor/rapidjson/fwd.h
    enginx/core/vendor/rapidjson/istreamwrapper.h
    enginx/core/vendor/rapidjson/memorybuffer.h
    enginx/core/vendor/rapidjson/memorystream.h
    enginx/core/vendor/rapidjson/ostreamwrapper.h
    enginx/core/vendor/rapidjson/pointer.h
    enginx/core/vendor/rapidjson/prettywriter.h
    enginx/core/vendor/rapidjson/rapidjson.h
    enginx/core/vendor/rapidjson/reader.h
    enginx/core/vendor/rapidjson/schema.h
    enginx/core/vendor/rapidjson/stream.h
    enginx/core/vendor/rapidjson/stringbuffer.h
    enginx/core/vendor/rapidjson/writer.h
    enginx/core/enginx.h
    enginx/core/enginxDefinitions.cpp
    enginx/core/enginxDefinitions.h
    enginx/core/enginxInstance.cpp
    enginx/core/enginxInstance.h
    enginx/core/enginxLocationProcessor.cpp
    enginx/core/enginxLocationProcessor.h
    enginx/core/enginxServerProcessor.cpp
    enginx/core/enginxServerProcessor.h
    enginx/core/enginxWorker.cpp
    enginx/core/enginxWorker.h
    enginx/main.cpp)

add_executable(enginx ${SOURCE_FILES})

#add_library(enginxLib
#            STATIC
#            ${SOURCE_FILES})