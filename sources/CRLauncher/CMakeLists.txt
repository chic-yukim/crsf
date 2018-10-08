# Author: Younguk Kim (bluekyu)

cmake_minimum_required(VERSION 3.10)
project(CRLauncher
    VERSION 3.0.0
    DESCRIPTION "Launcher for CRSF"
    LANGUAGES CXX
)

# === configure ====================================================================================
# find CRSF SDK
if(NOT CRSF_SDK_DIR)
    message(FATAL_ERROR "'CRSF_SDK_DIR' does not exist")
endif()

list(APPEND CMAKE_MODULE_PATH "${CRSF_SDK_DIR}/CMake")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)    # Project Grouping

include(windows_utils)

set(${PROJECT_NAME}_DEBUG_POSTFIX "-debug")
set(${PROJECT_NAME}_RELWITHDEBINFO_POSTFIX "-reldeb")

# === project specific packages ===========================
include(FindPackages)
if(VCPKG_TOOLCHAIN)
    _find_boost("1.68" "program_options")
else()
    _find_boost("1.65.1" "program_options")
endif()
# ==================================================================================================

# === target =======================================================================================
include("${CMAKE_CURRENT_SOURCE_DIR}/files.cmake")
add_executable(${PROJECT_NAME} ${crlauncher_sources})

# make easy debugging in MSVC
if(CRSF_BUILD_DEBUG_BIN_DIR)
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:${PROJECT_NAME}> ${CRSF_BUILD_DEBUG_BIN_DIR}
    )
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_auto_type)
if(MSVC)
    windows_add_longpath_manifest(${PROJECT_NAME})
    target_compile_options(${PROJECT_NAME} PRIVATE /MP /wd4251
        $<$<VERSION_GREATER:${MSVC_VERSION},1800>:/utf-8>

        # note: windows.cmake in vcpkg
        $<$<CONFIG:Release>:/Oi /Gy /Z7>
    )
    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS_RELWITHDEBINFO    " /INCREMENTAL:NO /OPT:REF /OPT:ICF ")
    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS_RELEASE           " /DEBUG /INCREMENTAL:NO /OPT:REF /OPT:ICF ")
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
endif()

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    # used for plugin postfix.
    $<$<CONFIG:Debug>:CRLAUNCHER_BUILD_CFG_POSTFIX="${${PROJECT_NAME}_DEBUG_POSTFIX}">
    $<$<CONFIG:RelWithDebInfo>:CRLAUNCHER_BUILD_CFG_POSTFIX="${${PROJECT_NAME}_RELWITHDEBINFO_POSTFIX}">
)

target_include_directories(${PROJECT_NAME}
    PRIVATE "${PROJECT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE $<$<NOT:$<BOOL:${Boost_USE_STATIC_LIBS}>>:Boost::dynamic_linking>
    Boost::thread Boost::program_options CRSeedLib
    fmt::fmt-header-only spdlog::spdlog
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    windows_add_delay_load(${PROJECT_NAME}
        "CRSeedLib.dll;render_pipeline.dll;"
        RELEASE
    )
    windows_add_delay_load(${PROJECT_NAME}
        "CRSeedLib-reldeb.dll;render_pipeline.dll;"
        RELWITHDEBINFO
    )
    windows_add_delay_load(${PROJECT_NAME}
        "CRSeedLib-debug.dll;render_pipeline-debug.dll;"
        DEBUG
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    FOLDER "crsf"
    DEBUG_POSTFIX ${${PROJECT_NAME}_DEBUG_POSTFIX}
    RELWITHDEBINFO_POSTFIX ${${PROJECT_NAME}_RELWITHDEBINFO_POSTFIX}
)

configure_debugging_information(${PROJECT_NAME})
# ==================================================================================================

# === install ======================================================================================
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Config
    RUNTIME DESTINATION "bin"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
)

install_debugging_information(${PROJECT_NAME} "bin")
# ==================================================================================================
