add_executable (ProcessFile)

target_link_libraries (ProcessFile hance-engine)
target_link_libraries (ProcessFile RiffWave)

target_sources (ProcessFile
  PRIVATE ProcessFile.cpp
)

set_target_properties (ProcessFile PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin"
)

if (MSVC)
  set (CMAKE_GENERATOR_PLATFORM x64)
  add_custom_command (TARGET ProcessFile POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${HANCE_DLL}" "$<TARGET_FILE_DIR:ProcessFile>")
endif()
