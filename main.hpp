#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>

#define MAX_SCRAMBLED_SIZE 24
#define MAJOR_VER 0
#define MINOR_VER 2
#define CHANGE_VER 1

struct Dict{
  int letters[26];
  bool pass;
  std::string word;
  Dict();
};

struct Functions {
  static std::vector<std::string> param_list;
  bool help;
  bool substring;

  Functions() : help(), substring() {}

  void print_help();
  bool valid_args(const std::vector<std::string>& args);
  void set_functions(const std::vector<std::string>& args);
};

int get_index(const char &c);
Dict process_word(const std::string &here);
bool check_if_pass(const Dict &source, Dict &current);
void load_settings(std::ifstream &settings, std::string &dictionary_file);
void preload_dictionary(const std::string& dictionary_file, bool message = true);
void scramble(const std::string &dictionary_file, bool substring = false);
void change_dictionary(std::string &dictionary_file);
