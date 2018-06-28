set(header_include
    "${PROJECT_SOURCE_DIR}/include/openvr_camera_object.h"
    "${PROJECT_SOURCE_DIR}/include/openvr_module.h"
)

# grouping
source_group("include" FILES ${header_include})

set(module_headers
    ${header_include}
)


set(source_src
    "${PROJECT_SOURCE_DIR}/src/openvr_camera_object.cpp"
    "${PROJECT_SOURCE_DIR}/src/openvr_module.cpp"
)

# grouping
source_group("src" FILES ${source_src})

set(module_sources
    ${source_src}
)
