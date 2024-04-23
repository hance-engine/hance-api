add_executable (SeparateStems)

target_link_libraries (SeparateStems hance-engine)
target_link_libraries (SeparateStems RiffWave)

target_sources (SeparateStems
  PRIVATE SeparateStems.cpp
)

set_target_properties (SeparateStems PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin"
)

if (MSVC)
  set (CMAKE_GENERATOR_PLATFORM x64)
  add_custom_command (TARGET SeparateStems POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${HANCE_DLL}" "$<TARGET_FILE_DIR:SeparateStems>")
endif()