#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <cstdint>

#define MAX_SCRAMBLED_SIZE 24
#define MAJOR_VER 0
#define MINOR_VER 3
#define CHANGE_VER 2

struct DictWord{
  int letters[26];
  bool pass;
  std::string word;
  int size;
  DictWord();
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
DictWord process_word(const std::string &here);
bool check_if_pass(const DictWord &source, DictWord &current);
void load_settings(std::ifstream &settings, std::string &Dictionary_file);
void preload_Dictionary(const std::string& Dictionary_file, bool message = true);
void scramble(const std::string &Dictionary_file, bool substring = false);
void change_Dictionary(std::string &Dictionary_file);
