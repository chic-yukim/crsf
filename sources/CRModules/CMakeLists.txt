cmake_minimum_required(VERSION 3.12)
project(CRModules
    LANGUAGES CXX
)

# === configure ====================================================================================
# find CRSF SDK
if(NOT CRSF_SDK_DIR)
    message(FATAL_ERROR "'CRSF_SDK_DIR' does not exist")
endif()
list(APPEND CMAKE_MODULE_PATH "${CRSF_SDK_DIR}/cmake")
include(FindPackages)

# common variable
set(CRMODULE_CONFIG_IN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/crmodule-config.cmake.in")
# ==================================================================================================

# === subdirectory =================================================================================
# modules
set(CRSF_MODULES
    "crprofiler"
    "openvr"
    "webcam"
)

foreach(module_id ${CRSF_MODULES})
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/${module_id}")
endforeach()
# ==================================================================================================
