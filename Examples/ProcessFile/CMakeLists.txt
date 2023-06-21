cmake_minimum_required(VERSION 3.0)

project(ProcessFile)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_library(hance-engine SHARED IMPORTED)

if(MSVC)
  set(HANCE_DLL "${CMAKE_SOURCE_DIR}/../../Bin/Windows_x64/HanceEngine.dll")
  set(HANCE_TARGET_DIR "$<TARGET_FILE_DIR:ProcessFile>")
  set_target_properties(hance-engine PROPERTIES
    IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/../../Bin/Windows_x64/HanceEngine.lib"
    IMPORTED_LOCATION "${HANCE_DLL}"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/../../Include")
elseif(APPLE)
  set(HANCE_TARGET_DIR "/usr/local/lib/")
  set(HANCE_DLL "${CMAKE_SOURCE_DIR}/../../Bin/macOS/HanceEngine.dylib")
  set_target_properties(hance-engine PROPERTIES
    IMPORTED_LOCATION "${HANCE_DLL}"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/../../Include")
elseif(UNIX)
  set(HANCE_TARGET_DIR "/usr/local/lib/")
  set(HANCE_DLL "${CMAKE_SOURCE_DIR}/../../Bin/Linux/libHanceEngine.so")
  set_target_properties(hance-engine PROPERTIES
    IMPORTED_LOCATION "${HANCE_DLL}"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/../../Include")
endif()

add_executable(ProcessFile ProcessFile.cpp RiffWave.h RiffWave.cpp)
target_link_libraries(ProcessFile hance-engine)

if(MSVC)
  set(CMAKE_GENERATOR_PLATFORM x64)
  add_custom_command(TARGET ProcessFile POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${HANCE_DLL}"
      ${HANCE_TARGET_DIR})
endif()