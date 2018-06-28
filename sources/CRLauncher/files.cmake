# list include/



# list src/
set(source_root
    "${CMAKE_CURRENT_SOURCE_DIR}/src/application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/program_options.cpp"
)

set(crlauncher_sources
    ${source_root}
)

# grouping
source_group("src" FILES ${source_root})
