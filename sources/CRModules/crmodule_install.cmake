# Author: Younguk Kim
# Date  : 2017-07-13

# === install ======================================================================================
install(TARGETS ${PROJECT_NAME} DESTINATION ${CRMODULE_INSTALL_DIR})
install(TARGETS ${CRMODULE_ID} EXPORT ${TARGET_EXPORT_NAME})
export(EXPORT ${TARGET_EXPORT_NAME}
    NAMESPACE ${TARGET_NAMESPACE}
    FILE "${PROJECT_BINARY_DIR}/${TARGET_EXPORT_NAME}.cmake"
)

install(FILES "${PROJECT_SOURCE_DIR}/config.json" DESTINATION ${CRMODULE_INSTALL_DIR})
install(DIRECTORY "${PROJECT_SOURCE_DIR}/resources" DESTINATION ${CRMODULE_INSTALL_DIR}
    OPTIONAL
)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include" DESTINATION ${CRMODULE_INSTALL_DIR}
    CONFIGURATIONS Release
    OPTIONAL
)

install(FILES ${PACKAGE_CONFIG_FILE} ${PACKAGE_VERSION_CONFIG_FILE}
    DESTINATION ${PACKAGE_CMAKE_INSTALL_DIR}
    CONFIGURATIONS Release
)
install(FILES ${${PROJECT_NAME}_MACRO_CMAKE_FILE}
    DESTINATION ${PACKAGE_CMAKE_INSTALL_DIR}
    CONFIGURATIONS Release
    OPTIONAL
)
install(EXPORT ${TARGET_EXPORT_NAME} NAMESPACE ${TARGET_NAMESPACE} DESTINATION ${PACKAGE_CMAKE_INSTALL_DIR})

install_debugging_information(${PROJECT_NAME} "${CRMODULE_INSTALL_DIR}")
# ==================================================================================================
