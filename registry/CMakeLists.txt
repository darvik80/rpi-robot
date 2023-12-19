cmake_minimum_required(VERSION 3.16)
project(registry)

file(GLOB SRCS_G
        src/*.cpp
        src/db/*.cpp
        src/http/*.cpp
        src/service/*.cpp
        src/service/registry/*.cpp
        src/service/device/*.cpp
        src/resources/*.cpp)

find_package(libpqxx)
find_package(prometheus-cpp)
find_package(mdns)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${libpqxx_INCLUDE_DIRS}
)

set(RESOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)
set(WWW_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/www/build)

message(STATUS "Build ReactJS App")
execute_process(
        COMMAND yarn install
        COMMAND yarn build
        WORKING_DIRECTORY ${WWW_DIRECTORY}
)

if (APPLE)
    message("Apple incubator build")

    find_library(IOKIT_LIBRARY IOKit)
    find_library(CORE_LIBRARY CoreFoundation)

    function(resource VAR SOURCE_PATH DESTINATION PATTERN)
        file(GLOB_RECURSE _LIST CONFIGURE_DEPENDS ${SOURCE_PATH}/${PATTERN})
        foreach (RESOURCE ${_LIST})
            get_filename_component(_PARENT ${RESOURCE} DIRECTORY)
            if (${_PARENT} STREQUAL ${SOURCE_PATH})
                set(_DESTINATION ${DESTINATION})
            else ()
                file(RELATIVE_PATH _DESTINATION ${SOURCE_PATH} ${_PARENT})
                set(_DESTINATION ${DESTINATION}/${_DESTINATION})
            endif ()
            set_property(SOURCE ${RESOURCE} PROPERTY MACOSX_PACKAGE_LOCATION ${_DESTINATION})
        endforeach (RESOURCE)
        set(${VAR} ${_LIST} PARENT_SCOPE)
    endfunction()

    resource(RESOURCES ${RESOURCE_DIRECTORY} Resources/ *.*)
    resource(HTML ${WWW_DIRECTORY}/build Resources/www *.*)
    resource(HTML_JS ${WWW_DIRECTORY}/build/static/js Resources/www/static/js *.js)
    resource(HTML_MEDIA ${WWW_DIRECTORY}/build/static/media Resources/www/static/media *.*)
    resource(HTML_IMG ${WWW_DIRECTORY}/build/img Resources/www/img *.png)

    set(RESOURCES_G
            ${RESOURCES}
            ${HTML}
            ${HTML_JS}
            ${HTML_MEDIA}
            ${HTML_CSS}
            ${HTML_IMG}
            )

    file(GLOB SRCS_OS src/resources/ResourceManager_apple.cpp)

    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRCS_G} ${SRCS_OS} ${RESOURCES_G})
else ()
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
    set(PROPERTIES_DIR ${EXECUTABLE_OUTPUT_PATH}/../etc)
    set(PLUGINS_DIR ${EXECUTABLE_OUTPUT_PATH}/../lib)
    set(VAR_DIR ${EXECUTABLE_OUTPUT_PATH}/../var)

    if (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES arm)
        set(TARGET_FORMAT "elf32-littlearm")
        set(TARGET_ARCHITECTURE "arm")
    elseif (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES aarch64)
        set(TARGET_FORMAT "elf64-littleaarch64")
        set(TARGET_ARCHITECTURE "aarch64")
    endif ()

    include(CMakeLists_linux.txt)

    add_executable(${PROJECT_NAME} ${SRCS_G} ${RES_SRC_FILE} ${RES_EMBED_FILES})
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
    #file(COPY ${WWW_DIRECTORY} DESTINATION "${PROPERTIES_DIR}/www")
    file(COPY resources/settings.json DESTINATION ${PROPERTIES_DIR}/${PROJECT_NAME})
endif ()

target_link_libraries(
        ${PROJECT_NAME}
        logging
        application
        iot-platform
        libpqxx::pqxx
        prometheus-cpp::prometheus-cpp
        mdns::mdns
        ${IOKIT_LIBRARY}
        ${CORE_LIBRARY}
)
