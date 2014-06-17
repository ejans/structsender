CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(microblx_structsender)

set(CMAKE_CXX_FLAGS "-Wall -Werror -fvisibility=hidden")
set(CMAKE_CXX_COMPILER clang++ )
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake") ## temporary resources, for UBX

# Global
set(INSTALL_LIB_TYPES_DIR lib/ubx/types CACHE PATH "Installation directory for types libraries")
set(INSTALL_LIB_BLOCKS_DIR lib/ubx/blocks CACHE PATH "Installation directory for blocks libraries")
set(INSTALL_INCLUDE_DIR include/ubx CACHE PATH "Installation directory for header files (types)")
set(INSTALL_CMAKE_DIR  share/ubx/cmake CACHE PATH  "Installation directory for CMake files") # default ${DEF_INSTALL_CMAKE_DIR}

# Make relative paths absolute
foreach(p LIB_TYPES LIB_BLOCKS BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

##
# Add uninstall target.
##
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
    
add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
)

set (UBX_ROOT $ENV{UBX_ROOT} )
if (NOT UBX_ROOT)
 message( FATAL_ERROR "UBX_ROOT is not defined. CMake install willl abort." )
endif()

find_package(Microblx REQUIRED)

set(GEN_HEXARR ${UBX_ROOT}/tools/file2carr.lua)

file(GLOB types
  ${CMAKE_CURRENT_SOURCE_DIR}/types/*.h
)


set(outfiles "")
foreach( _type ${types} )
  string(REPLACE ".h" ".h.hexarr" _outfile ${_type})
  add_custom_command(
  OUTPUT ${_outfile}
  COMMAND ${GEN_HEXARR}
  ARGS ${_type}  > ${_outfile}
  )
  list(APPEND outfiles ${_outfile})
endforeach(_type)
add_custom_target( expanded_defs ALL DEPENDS ${outfiles} )

include_directories(
  ${UBX_INCLUDE_DIR}
)
# Compile library sender
add_library(sender SHARED src/sender.c )
set_target_properties(sender PROPERTIES PREFIX "")
target_link_libraries(sender ${UBX_LIBRARIES})

# Install sender
install(TARGETS sender DESTINATION ${INSTALL_LIB_BLOCKS_DIR} EXPORT sender-block)
set_property(TARGET sender PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
install(EXPORT sender-block DESTINATION ${INSTALL_CMAKE_DIR})

# Compile library receiver
add_library(receiver SHARED src/receiver.c )
set_target_properties(receiver PROPERTIES PREFIX "")
target_link_libraries(receiver ${UBX_LIBRARIES})

# Install receiver
install(TARGETS receiver DESTINATION ${INSTALL_LIB_BLOCKS_DIR} EXPORT receiver-block)
set_property(TARGET receiver PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
install(EXPORT receiver-block DESTINATION ${INSTALL_CMAKE_DIR})

#install types
add_library(ssendertypes SHARED src/decltypes.c)
set_target_properties(ssendertypes PROPERTIES PREFIX "") 
set_property(TARGET ssendertypes PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(ssendertypes ${UBX_LIBRARIES})

### Install libraries
set_target_properties(ssendertypes PROPERTIES PUBLIC_HEADER "${outfiles};${types}" )

install(TARGETS ssendertypes 
  EXPORT ssendertypes-targets
  LIBRARY DESTINATION "${INSTALL_LIB_TYPES_DIR}" COMPONENT kdl_types
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/types" COMPONENT dev
)
