cmake_minimum_required(VERSION 3.11)

include(../cmake/raylib.cmake)
include(../cmake/raylib_cpp.cmake)
include(../cmake/nlohmann_json.cmake)

project(game VERSION 0.1.0 LANGUAGES CXX)

include(../cmake/doxygen.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Füge die Unterverzeichnisse hinzu, die gebaut werden sollen
add_subdirectory(core)
add_subdirectory(scenes)

# Erstelle die finale .exe-Datei NUR aus main.cpp
add_executable(${PROJECT_NAME} main.cpp)

# Füge die config.h.in Datei hinzu
configure_file(config.h.in config.h)
configure_file(config_enemies.h.in config_enemies.h)

target_include_directories(${PROJECT_NAME} PUBLIC
        "${PROJECT_BINARY_DIR}"
)

# Verlinke die .exe-Datei mit unseren beiden Bibliotheken
target_link_libraries(${PROJECT_NAME} scenes)

# Copy assets to assets path
add_custom_target(copy_assets
COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_LIST_DIR}/../cmake/copy-assets.cmake"
)
add_dependencies(${PROJECT_NAME} copy_assets)

# Wichtige System-DLLs für Windows finden und mitliefern
if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # Finde das Verzeichnis, in dem der Compiler und seine DLLs liegen
    get_filename_component(MINGW_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)

    # Liste der DLLs, die wir brauchen
    set(MINGW_REQUIRED_DLLS
            "libgcc_s_seh-1.dll"
            "libstdc++-6.dll"
            "libwinpthread-1.dll"
    )

    # Gehe durch die Liste und füge für jede gefundene DLL einen Installationsbefehl hinzu
    foreach(DLL_NAME ${MINGW_REQUIRED_DLLS})
        if(EXISTS "${MINGW_BIN_DIR}/${DLL_NAME}")
            install(FILES "${MINGW_BIN_DIR}/${DLL_NAME}" DESTINATION "${PROJECT_NAME}_Bundle")
        endif()
    endforeach()
endif()

if (APPLE)
target_link_libraries(${PROJECT_NAME} "-framework IOKit")
target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif ()

# CPack instructions
# call e.g. cpack -G ZIP -C Debug to create a binary distribution
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${PROJECT_NAME}_Bundle")
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/assets" DESTINATION "${PROJECT_NAME}_Bundle")