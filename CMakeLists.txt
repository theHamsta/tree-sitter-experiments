cmake_minimum_required (VERSION 3.5)

# projectname is the same as the main-executable
project(tree-experiments)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

set(CXX_STANDARD 17)

aux_source_directory(source SOURCES)

add_executable(tree-experiments
    ${SOURCES}
    main.cpp
    tree-sitter/src/runtime/runtime.c
    tree-sitter-json/src/parser.c 
    tree-sitter-cpp/src/parser.c 
    tree-sitter-cpp/src/scanner.cc 
    )

target_include_directories(
    tree-experiments
PRIVATE
    tree-sitter/src
    tree-sitter/include
    tree-sitter/externals/utf8proc
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
	)

target_link_libraries(tree-experiments)


