cmake_minimum_required (VERSION 3.15)
project (HanceExamples VERSION 2.0.1)

set (CMAKE_CXX_STANDARD 11)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)
add_library (hance-engine SHARED IMPORTED)

if (MSVC)
  set (HANCE_DLL "${CMAKE_SOURCE_DIR}/../Bin/Windows_x64/HanceEngine.dll")
  set_target_properties (hance-engine PROPERTIES
    IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/../Bin/Windows_x64/HanceEngine.lib"
    IMPORTED_LOCATION "${HANCE_DLL}"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/../Include")
else()
  set (HANCE_TARGET_DIR "/usr/local/lib/")
  if (APPLE)
    set (HANCE_DLL "${CMAKE_SOURCE_DIR}/../Bin/macOS/libHanceEngine.dylib")
  else()
    set (HANCE_DLL "${CMAKE_SOURCE_DIR}/../Bin/Linux/libHanceEngine.so")
  endif()
  set_target_properties (hance-engine PROPERTIES
    IMPORTED_LOCATION "${HANCE_DLL}"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/../Include")
endif()

add_subdirectory (RiffWave)
add_subdirectory (ProcessFile)
add_subdirectory (SeparateStems)