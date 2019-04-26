set(header_include
    "${PROJECT_SOURCE_DIR}/include/webcam_interface.h"
)

# grouping
source_group("include" FILES ${header_include})

set(module_headers
    ${header_include}
)


set(source_src
    "${PROJECT_SOURCE_DIR}/src/webcam_interface.cpp"
    "${PROJECT_SOURCE_DIR}/src/webcam_module.cpp"
    "${PROJECT_SOURCE_DIR}/src/webcam_module.h"
)

# grouping
source_group("src" FILES ${source_src})

set(module_sources
    ${source_src}
)
