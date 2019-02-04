
// Filename - test-json-parser.c

#include "tree_sitter/runtime.h"
#include <assert.h>
#include <iostream>
#include <stack>
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


int main(  int argc, const char** argv )
{
    // Comment
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

  // Print the syntax tree as an S-expression.
  char *s_expressions = ts_node_string(root_node);
  std::cout << "Syntax tree: " << std::endl;
  std::cout << s_expressions << std::endl;
  free(s_expressions);

  FILE *dot_file = fopen("out.dot", "w");
  ts_tree_print_dot_graph(tree, dot_file);
  fclose(dot_file);

  std::stack<TSNode> node_stack;

  node_stack.push(root_node);

  std::string cpp_source_code(source_code);

  std::cout << std::endl;
  std::cout << "Concrete syntax tree:" << std::endl;
  while (!node_stack.empty()) {
    TSNode current_node = node_stack.top();
    node_stack.pop();

    // TSNode ts_node_named_child(TSNode, uint32_t);
    // uint32_t ts_node_named_child_count(TSNode);
    auto num_children = ts_node_child_count(current_node);

    for (int i = num_children - 1; i >= 0; i--) {
      TSNode child = ts_node_child(current_node, i);
      node_stack.push(child);
    }

    auto start = ts_node_start_byte(current_node);
    auto end = ts_node_end_byte(current_node);
    auto length = end - start;

    if (num_children == 0) {
      char *node_string = ts_node_string(current_node);
      std::cout << cpp_source_code.substr(start, length) << "\t\t\t\t";
      std::cout << node_string << std::endl;
      free(node_string);
    }
  }

  std::cout << std::endl;
  std::cout << "Abstract syntax tree:" << std::endl;
  node_stack.push(root_node);
  while (!node_stack.empty()) {
    TSNode current_node = node_stack.top();
    node_stack.pop();

    // TSNode ts_node_named_child(TSNode, uint32_t);
    // uint32_t ts_node_named_child_count(TSNode);
    auto num_children = ts_node_named_child_count(current_node);

    for (int i = num_children - 1; i >= 0; i--) {
      TSNode child = ts_node_named_child(current_node, i);
      node_stack.push(child);
    }

    auto start = ts_node_start_byte(current_node);
    auto end = ts_node_end_byte(current_node);
    auto length = end - start;

    if (num_children == 0) {
      char *node_string = ts_node_string(current_node);
      std::cout << cpp_source_code.substr(start, length) << "\t\t\t\t";
      std::cout << node_string << std::endl;
      free(node_string);
    }
  }

  std::cout << std::endl;
  std::cout << "Code reprinted:" << std::endl;
  node_stack.push(root_node);
  while (!node_stack.empty()) {
    TSNode current_node = node_stack.top();
    node_stack.pop();

    // TSNode ts_node_named_child(TSNode, uint32_t);
    // uint32_t ts_node_named_child_count(TSNode);
    auto num_children = ts_node_child_count(current_node);

    for (int i = num_children - 1; i >= 0; i--) {
      TSNode child = ts_node_child(current_node, i);
      node_stack.push(child);
    }

    auto start = ts_node_start_byte(current_node);
    auto end = ts_node_end_byte(current_node);
    auto length = end - start;

    if (num_children == 0) {
      char *node_string = ts_node_string(current_node);
      std::string current_code = cpp_source_code.substr(start, length);
      std::cout << current_code << " ";
      if (current_code == ";" ||
          strcmp(node_string, "(system_lib_string)") == 0 ||
          current_code == "{" || strcmp(node_string, "(comment)") == 0) {
        std::cout << std::endl;
      }
      free(node_string);
    }
  }
  std::cout << std::endl;

  ts_tree_delete(tree);
  ts_parser_delete(parser);
  return 0;
}
