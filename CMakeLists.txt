cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0091 NEW) # Required for the static CRT (/MT)

# =================================================
# Mod metadata: edit this section first
# =================================================
set(MY_MOD_NAME "MyMod" CACHE STRING "Mod name; also the DLL file name (no spaces)")
set(MY_VERSION "0.1.0" CACHE STRING "Mod version (major.minor.patch [.build])")
set(MY_DESCRIPTION "A Soku mod built from the mod template." CACHE STRING "Full description")
set(MY_DESCRIPTION_FILE "A Soku mod" CACHE STRING "Short description used in version resources")
set(MY_COMPANY "YourName" CACHE STRING "Author or organization name for version resources")
option(MY_LUA_EXPORT "Export as Lua module, callable via require(\"<mod name>\")" OFF)
set(MY_GAME_FOLDER "$ENV{TH123}" CACHE STRING "Game root directory for install and debugging; defaults to the TH123 environment variable")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

project(${MY_MOD_NAME}
  DESCRIPTION ${MY_DESCRIPTION}
  VERSION ${MY_VERSION}
  LANGUAGES C CXX
)

if(NOT WIN32)
  message(FATAL_ERROR "This project can only be built as x86 (Win32).")
endif()
if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
if(NOT MY_GAME_FOLDER)
  message(WARNING "MY_GAME_FOLDER / TH123 is empty: automatic install and debug launch will be unavailable.")
endif()

# Generate version.rc from MY_VERSION
string(REGEX MATCHALL "[0-9]+" MY_VERSION_PARTS "${MY_VERSION}")
string(TIMESTAMP MY_BUILD_DATE "%y%m%d")
string(TIMESTAMP MY_BUILD_DATE_SHORT "%m%d")
list(LENGTH MY_VERSION_PARTS _len)
list(APPEND MY_VERSION_PARTS 0 0 0)
list(GET MY_VERSION_PARTS 0 MY_VERSION_MAJOR)
list(GET MY_VERSION_PARTS 1 MY_VERSION_MINOR)
list(GET MY_VERSION_PARTS 2 MY_VERSION_PATCH)
if(_len GREATER 3)
    list(GET MY_VERSION_PARTS 3 MY_VERSION_BUILD)
else()
    set(MY_VERSION_BUILD "${MY_BUILD_DATE_SHORT}")
endif()
configure_file("${CMAKE_SOURCE_DIR}/src/version.rc.in" "${CMAKE_BINARY_DIR}/version.rc" @ONLY)


# =================================================
# Import Third-party dependencies
# =================================================
add_subdirectory("${CMAKE_SOURCE_DIR}/thirdparty" EXCLUDE_FROM_ALL)

file(GLOB_RECURSE GLOB_SOURCE_FILE CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.c"
  "${CMAKE_SOURCE_DIR}/src/*.rc.in"
)
file(GLOB_RECURSE GLOB_HEADER_FILE CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/*.hpp" "${CMAKE_SOURCE_DIR}/src/*.h"
)
set(DIST_HEADER_FILE "${CMAKE_SOURCE_DIR}/distribution/export.hpp")
source_group(TREE "${CMAKE_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${GLOB_SOURCE_FILE})
source_group(TREE "${CMAKE_SOURCE_DIR}/src" PREFIX "Header Files" FILES ${GLOB_HEADER_FILE})
source_group("Header Files\\distribution" FILES ${DIST_HEADER_FILE})
# =================================================
# Main Target includes and linking
# =================================================
add_library(${PROJECT_NAME} MODULE
  ${GLOB_SOURCE_FILE} ${GLOB_HEADER_FILE} ${DIST_HEADER_FILE}
  "${CMAKE_BINARY_DIR}/version.rc"
)
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/distribution
)
target_link_libraries(${PROJECT_NAME} PRIVATE
  SokuLib
  Shlwapi
  d3dx9
)
if(MY_LUA_EXPORT)
  target_link_libraries(${PROJECT_NAME} PRIVATE LuaBridge)
endif()

# =================================================
# Compile and Project Settings
# =================================================
target_compile_definitions(${PROJECT_NAME} PRIVATE
  _CRT_SECURE_NO_WARNINGS
  # Minimum supported Windows version
  WINVER=0x0501 _WIN32_WINNT=0x0501
  # Mod metadata consumed by the C++ sources
  MY_MOD_NAME=${PROJECT_NAME}
  MY_MOD_VERSION=${MY_VERSION}
  # Export API symbols from this DLL
  MY_MOD_EXPORTS
)
if(MY_LUA_EXPORT)
  target_compile_definitions(${PROJECT_NAME} PRIVATE MY_LUA_EXPORT)
endif()

# UTF-8 source and execution charsets, required for non-ASCII comments
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /utf-8 /EHsc /nologo $<$<CONFIG:Release>:/O2> $<$<CONFIG:Debug>:/ZI>)
  target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS /Brepro $<$<CONFIG:Debug>:/INCREMENTAL>)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-c++11-narrowing -Wno-microsoft-cast)
  endif()
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra $<$<CONFIG:Release>:-O2>)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

# =================================================
# Install
# =================================================
set(CMAKE_INSTALL_PREFIX "${MY_GAME_FOLDER}" CACHE PATH "Install prefix, can be targeted to game folder." FORCE)
cmake_path(APPEND CMAKE_INSTALL_PREFIX "modules" "${PROJECT_NAME}" OUTPUT_VARIABLE CMAKE_INSTALL_DIR)
message("Will install to ${CMAKE_INSTALL_DIR}")
install(FILES $<TARGET_FILE:${PROJECT_NAME}> DESTINATION "${CMAKE_INSTALL_DIR}")
# PDBs for Debug and RelWithDebInfo builds
if(MSVC)
  install(
    FILES $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:$<TARGET_PDB_FILE:${PROJECT_NAME}>>
    DESTINATION "${CMAKE_INSTALL_DIR}"
  )
endif()

# =================================================
# Debugging
# =================================================
if(MSVC)
  set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
  if(MY_GAME_FOLDER)
    set_target_properties(${PROJECT_NAME} PROPERTIES
      VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
      VS_DEBUGGER_COMMAND "${CMAKE_INSTALL_PREFIX}/th123.exe"
      VS_DEBUGGER_COMMAND_ARGUMENTS ""
    )
  endif()
endif()
add_custom_command(TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND tasklist | findstr th123.exe &&
            (taskkill /IM th123.exe /F) ||
            (exit /b 0)
    COMMENT "Stop any running game instance."
)
add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND}
        --install "${CMAKE_BINARY_DIR}"
        --config $<CONFIG>
    VERBATIM
    COMMENT "Install after build."
)
