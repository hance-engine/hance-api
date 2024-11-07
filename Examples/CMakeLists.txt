cmake_minimum_required (VERSION 3.22)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

include ("../HanceVersion.txt")
project (HanceExamples VERSION ${HANCE_VERSION})

set (CMAKE_CXX_STANDARD 11)

add_library (hance-engine SHARED IMPORTED)

if (MSVC)
  set (HANCE_DLL "${CMAKE_SOURCE_DIR}/../Bin/Windows_x64/HanceEngine.dll")
  set_target_properties (hance-engine PROPERTIES
    IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/../Bin/Windows_x64/HanceEngine.lib"
    IMPORTED_LOCATION "${HANCE_DLL}"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/../Include")
else()
  if (APPLE)
    set (HANCE_DLL "${CMAKE_SOURCE_DIR}/../Bin/macOS/libHanceEngine.dylib")
  else()
    if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64")
      set (HANCE_DLL "${CMAKE_SOURCE_DIR}/../Bin/Linux_arm64/libHanceEngine.so")
    else()
      set (HANCE_DLL "${CMAKE_SOURCE_DIR}/../Bin/Linux_x64/libHanceEngine.so")
    endif()
  endif()
  set_target_properties (hance-engine PROPERTIES
    IMPORTED_LOCATION "${HANCE_DLL}"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/../Include")
endif()

add_subdirectory (RiffWave)
add_subdirectory (ProcessFile)
