include_directories(${CMAKE_CURRENT_SOURCE_DIR})

IF (WIN32 AND MSVC)
  ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF (WIN32 AND MSVC)

if (NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
endif()

#--------------------------------------------------------------------------------
### Projects
#--------------------------------------------------------------------------------

add_subdirectory(Groom)
add_subdirectory(Optimize)
add_subdirectory(Application)

