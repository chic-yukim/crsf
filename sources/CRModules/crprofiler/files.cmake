set(header_include
    "${PROJECT_SOURCE_DIR}/include/crmodules/${CRMODULE_ID}/module.hpp"
)

# grouping
source_group("include\\${CRMODULE_ID}" FILES ${header_include})

set(module_headers
    ${header_include}
)


set(source_src
    "${PROJECT_SOURCE_DIR}/src/gui_dsm.cpp"
    "${PROJECT_SOURCE_DIR}/src/gui_nm.cpp"
    "${PROJECT_SOURCE_DIR}/src/module.cpp"
)

# grouping
source_group("src" FILES ${source_src})

set(module_sources
    ${source_src}
)
