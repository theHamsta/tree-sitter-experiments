
// Filename - test-json-parser.c

#include "tree_sitter/runtime.h"
#include <assert.h>
#include <iostream>
#include <queue>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

// Declare the `tree_sitter_json` function, which is
// implemented by the `tree-sitter-json` library.
extern "C" TSLanguage *tree_sitter_json();
extern "C" TSLanguage *tree_sitter_cpp();

int main() {
  // Create a parser.
  TSParser *parser = ts_parser_new();

  // Set the parser's language (JSON in this case).
  ts_parser_set_language(parser, tree_sitter_cpp());

  // Build a syntax tree based on source code stored in a string.
  const char *source_code =
      R"(      
#include <exception>
#include <iostream>

#include <opencv2/core.hpp>


using namespace std;


int main( int argc, const char** argv )
{
    cout << "Hallo Welt!" << endl;
	cv::Mat a;
	a.create(3, 3, CV_32FC1);
	cout << a.at<float>(1,1) << endl;
}
)";
  TSTree *tree =
      ts_parser_parse_string(parser, NULL, source_code, strlen(source_code));

  // Get the root node of the syntax tree.
  TSNode root_node = ts_tree_root_node(tree);

  // Get some child nodes.
  TSNode array_node = ts_node_named_child(root_node, 0);
  TSNode number_node = ts_node_named_child(array_node, 0);

  //// Check that the nodes have the expected types.
  // assert(strcmp(ts_node_type(root_node), "value") == 0);
  // assert(strcmp(ts_node_type(array_node), "array") == 0);
  // assert(strcmp(ts_node_type(number_node), "number") == 0);

  //// Check that the nodes have the expected child counts.
  // assert(ts_node_child_count(root_node) == 1);
  // assert(ts_node_child_count(array_node) == 5);
  // assert(ts_node_named_child_count(array_node) == 2);
  // assert(ts_node_child_count(number_node) == 0);

  // Print the syntax tree as an S-expression.
  char *string = ts_node_string(root_node);
  // printf("Syntax tree: %s\n", string);

  FILE *dot_file = fopen("out.dot", "w");
  ts_tree_print_dot_graph(tree, dot_file);
  fclose(dot_file);

  std::queue<TSNode> node_queue;

  node_queue.push(root_node);

  std::string cpp_source_code(source_code);

  while (!node_queue.empty()) {
    TSNode current_node = node_queue.front();
    node_queue.pop();

    // TSNode ts_node_named_child(TSNode, uint32_t);
    // uint32_t ts_node_named_child_count(TSNode);
    auto num_children = ts_node_child_count(current_node);

     for (int i = 0; i < num_children; i++) {
     TSNode child = ts_node_child(current_node, i);
     node_queue.push(child);
    }

    auto start = ts_node_start_byte(current_node);
    auto end = ts_node_end_byte(current_node);
    auto length = end - start;

    if (num_children == 0) {
      // std::cout << ts_node_string(current_node) << ": ";
      std::cout << cpp_source_code.substr(start, length) << std::endl;
    }
  }
  //std::cout << ts_node_string(root_node) << ": ";

  // Free all of the heap-allocated memory.
  free(string);
  ts_tree_delete(tree);
  ts_parser_delete(parser);
  return 0;
}
