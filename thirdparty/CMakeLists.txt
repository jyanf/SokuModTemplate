# SokuLib
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/SokuLib" EXCLUDE_FROM_ALL)
# SokuLib requires d3dx9 but does not link it itself
target_link_directories(SokuLib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/SokuLib/lib
)
target_link_libraries(SokuLib PUBLIC d3dx9)
target_include_directories(SokuLib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/SokuLib/src
  ${CMAKE_CURRENT_SOURCE_DIR}/SokuLib/directx
)

if(MY_LUA_EXPORT)
  # lua 5.4, built as a static library
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lua" EXCLUDE_FROM_ALL)

  # LuaBridge3, header-only distribution
  add_library(LuaBridge INTERFACE)
  target_link_libraries(LuaBridge INTERFACE lua::lua)
  target_include_directories(LuaBridge INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/LuaBridge/Distribution"
  )
endif()
