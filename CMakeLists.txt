cmake_minimum_required (VERSION 3.5)

# projectname is the same as the main-executable
project(tree-experiments)

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


