# Author: Younguk Kim (bluekyu)

cmake_minimum_required(VERSION 3.12)

get_filename_component(CRMODULE_ID ${CMAKE_CURRENT_LIST_DIR} NAME)
project(crmodule_${CRMODULE_ID}
    VERSION 0.1.0
    LANGUAGES CXX
)

# === configure ====================================================================================
set_property(GLOBAL PROPERTY USE_FOLDERS ON)    # Project Grouping

# find CRSF SDK
if(NOT CRSF_SDK_DIR)
    string(REPLACE "\\" "/" CRSF_SDK_DIR "$ENV{CRSF_SDK_DIR}")
    message("CRSF SDK: ${CRSF_SDK_DIR}")
endif()
list(APPEND CMAKE_MODULE_PATH "${CRSF_SDK_DIR}/cmake")

set(CRMODULE_INSTALL_DIR "${CRSeedLib_MODULE_DIR}/${CRMODULE_ID}")

# === project specific packages ===
include(FindPackages)

find_package(imgui CONFIG REQUIRED)

set(${PROJECT_NAME}_MACRO_CMAKE_FILE "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}-macro.cmake")
include(${${PROJECT_NAME}_MACRO_CMAKE_FILE})
# ==================================================================================================

# === target =======================================================================================
include("${PROJECT_SOURCE_DIR}/files.cmake")
add_library(${PROJECT_NAME} MODULE ${module_sources} ${module_headers})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_auto_type)
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /MP /wd4251)
    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS_RELWITHDEBINFO    " /INCREMENTAL:NO /OPT:REF /OPT:ICF ")
    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS_RELEASE           " /DEBUG /INCREMENTAL:NO /OPT:REF /OPT:ICF ")
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
endif()

target_compile_definitions(${PROJECT_NAME}
    PRIVATE CRMODULE_ID_STRING="${CRMODULE_ID}"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE CRSeedLib
    fmt::fmt-header-only spdlog::spdlog

    imgui::imgui
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    FOLDER "crsf/CRModules"
    DEBUG_POSTFIX ${CRSeedLib_DEBUG_POSTFIX}
    RELWITHDEBINFO_POSTFIX ${CRSeedLib_RELWITHDEBINFO_POSTFIX}
    VERSION ${PROJECT_VERSION}
)

# configure local debugging for MSVC
if((MSVC) AND (WIN32))
    set(${PROJECT_NAME}_JUNCTION_DIRS "resources")
    configure_build_directory("module" "${${PROJECT_NAME}_JUNCTION_DIRS}")
endif()

include("${CRSF_SDK_DIR}/cmake/debugging-information.cmake")
configure_debugging_information(${PROJECT_NAME})
# ==================================================================================================

# === target =======================================================================================
# create module interface target
add_library(${CRMODULE_ID} INTERFACE)

target_include_directories(${CRMODULE_ID}
    INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CRMODULE_INSTALL_DIR}/include>
)

target_link_libraries(${CRMODULE_ID}
    INTERFACE CRSeedLib rpplugins::openvr
)

# configure package
set(PACKAGE_NAME "crmodule_${CRMODULE_ID}")
set(TARGET_NAMESPACE "crmodule::")
set(PACKAGE_VERSION_CONFIG_FILE "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}-config-version.cmake")
set(PACKAGE_CONFIG_FILE "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}-config.cmake")
set(TARGET_EXPORT_NAME "${PACKAGE_NAME}-targets")
set(PACKAGE_CMAKE_INSTALL_DIR "${CRMODULE_INSTALL_DIR}/cmake")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(${PACKAGE_VERSION_CONFIG_FILE}
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
configure_package_config_file(${CRMODULE_CONFIG_IN_FILE}
    ${PACKAGE_CONFIG_FILE}
    INSTALL_DESTINATION ${PACKAGE_CMAKE_INSTALL_DIR}
)

add_library(${TARGET_NAMESPACE}${CRMODULE_ID} ALIAS ${CRMODULE_ID})
# ==================================================================================================

# === install ======================================================================================
include("../crmodule_install.cmake")
# ==================================================================================================
