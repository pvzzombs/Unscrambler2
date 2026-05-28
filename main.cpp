#include "main.hpp"

DictWord::DictWord(){
  //memset(letters, 0, 26 * sizeof(int));
  std::fill(std::begin(letters), std::end(letters), 0);
  pass = true;
  size = 0;
}

std::vector<std::string> Functions::param_list = {"-h", "--help", "-s", "--substr"};
std::vector <DictWord> Dictionary;

int get_index(const char &c){
  int temp = -1;
  if(c >= 'A' && c <= 'Z'){
    temp = static_cast<int>(c - 'A');
  }
  if(c >= 'a' && c <= 'z'){
    temp = static_cast<int>(c - 'a');
  }
  return temp;
}

DictWord process_word(const std::string &here){
  uint64_t i, length = here.length();
  DictWord temporary;
  temporary.word = here;

  for(i = 0; i < length; i++){
    int index = get_index(here[i]);
    if(index == -1){
      std::cerr << "Error happened, :(" << std::endl;
      break;
    }else{
      ++temporary.letters[index];
    }
  }
  temporary.size = length;
  return temporary;
}

bool check_if_pass(const DictWord &source, DictWord &current){
  uint64_t i;
  for(i = 0; i < 26; i++){
    if(current.letters[i] > source.letters[i]){
      current.pass = false;
      break;
    }
  }
  return current.pass;
}

void load_settings(std::ifstream &settings, std::string &Dictionary_file){
  if(settings.is_open()){
    getline(settings, Dictionary_file);
    settings.close();
  }else{
    std::ofstream new_settings("settings.cfg");
    Dictionary_file = "sowpods.txt";
    new_settings << Dictionary_file;
    new_settings.close();
    std::cout << "Generating config file..." << std::endl;
  }
}

void preload_Dictionary(const std::string& Dictionary_file, bool message){
  std::string temporary;
  std::ifstream file(Dictionary_file.c_str());

  if(Dictionary.size() > 0){
    Dictionary.clear();
  }

  if(file.is_open()){
    if(message) {
      std::cout << "Dictionary file found, loading Dictionary..." << std::endl;
    }
    while(!file.eof()){
      std::getline(file, temporary);
      if (temporary.size() < 1){
        continue;
      }
      Dictionary.push_back(process_word(temporary));
    }
    if(message) {
      std::cout << "Dictionary loaded" << std::endl;
    }
    file.close();
  }else{
    std::cerr << "Error preloading Dictionary file!" << std::endl;
  }
}

void scramble(){
  std::stringstream text_flow;

  std::string scrambled;
  DictWord scrambled_DictWord;

  bool check = false;
  uint64_t i, leng;
  
  std::vector<DictWord> wordsThatPassed;
  int wordSize = 0;

  std::cout << "enter string to scramble: ";

  while(true){
    std::cin >> scrambled;
    std::cin.ignore();

    if(scrambled.length() > MAX_SCRAMBLED_SIZE){
      std::cout << "please re-enter again,(" << MAX_SCRAMBLED_SIZE << ") letters only!: ";
      continue;
    }
    break;
  }

  scrambled_DictWord = process_word(scrambled);

  //compare?
  leng = Dictionary.size();

  if(leng < 1){
    std::cerr << "looks like Dictionary is not loaded or missing..." << std::endl;
  }

  for(i = 0; i < leng; i++){
    check = check_if_pass(scrambled_DictWord, Dictionary.at(i));
    if(check){
      wordsThatPassed.push_back(Dictionary.at(i));
    }
  }
  
  std::sort(wordsThatPassed.begin(), wordsThatPassed.end(), [](const DictWord &a, const DictWord &b ){
    return a.size < b.size;
  });
  
  for (i = 0; i < wordsThatPassed.size(); i++) {
    if (wordSize != wordsThatPassed.at(i).size) {
      wordSize = wordsThatPassed.at(i).size;
      text_flow << "Word Size: " << wordSize << std::endl;
    }
    text_flow << wordsThatPassed.at(i).word << std::endl;
  }

  std::cout << text_flow.str();
  text_flow.str("");

  std::cout << "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=" << std::endl;
  for(i = 0; i < leng; i++){
    Dictionary.at(i).pass = true;
  }
  //Dictionary.clear();
}

void change_Dictionary(std::string &Dictionary_file){
  std::string temp;
  std::cout << "Enter Dictionary file: ";
  std::getline(std::cin, temp);
  Dictionary_file = temp;
  std::ifstream settings("settings.cfg");
  std::cout << "Setting Dictionary file to " << temp << std::endl;
  load_settings(settings, temp);
  preload_Dictionary(Dictionary_file);
}

void Functions::print_help()
{
  std::cout << "Format: " << std::endl;
  std::cout << "	[PROGRAM] [WORD]" << std::endl;
  std::cout << "	[PROGRAM] [PARAMETERS] [WORD]" << std::endl;
  std::cout << "Options: " << std::endl;
  std::cout << "	-h, --help" << "\t\t\t\t\t" << "Displays help text" << std::endl;
  std::cout << "	-s, --substr" << "\t\t\t\t\t" << "Prints out substrings as well" << std::endl;
}

bool Functions::valid_args(const std::vector<std::string>& args)
{
  std::vector<std::string> param_copy = param_list;
  for(size_t i = 0; i<args.size(); i++) {
    if(args[i][0] != '-') // ignores non-parameters
      continue;
    bool check = false;
    for(size_t j = 0; j<param_copy.size(); j++) {
      if(args[i] == param_copy[j]) {
        check = true;
        param_copy.erase(param_copy.begin()+static_cast<long>(j));
        break;
      }
    }
    if(!check) {
      std::cout << "[Invalid Parameter]: " << args[i] << std::endl;
      return false;
    }
  }
  return true;
}

void Functions::set_functions(const std::vector<std::string>& args)
{
  for(size_t i = 0; i<args.size(); i++) {
    if(args[i] == "-h" || args[i] == "--help")
      help = true;
    else if(args[i] == "-s" || args[i] == "--substr")
      substring = true;
  }
}

int main(int argc, char* argv[]){
  if(argc == 1) {
    Dictionary.reserve(100);

    std::cout << "Word unscrambler by pvzzombs" << std::endl;
    std::cout << __DATE__ << ", " << __TIME__ << ", " << __cplusplus << "; " << MAJOR_VER << "." << MINOR_VER << "." << CHANGE_VER << std::endl;

    std::string Dictionary_file;
    std::ifstream settings("settings.cfg");

    std::cout << "Finding config file..." << std::endl;
    load_settings(settings, Dictionary_file);

    std::cout << "Preloading Dictionary..." << std::endl;
    preload_Dictionary(Dictionary_file);

    while(true){
      std::cout << "Press any key of choice: " << std::endl;
      std::cout << "(A) Scramble. (b) Change Dictionary. (c) Exit." << std::endl;
      std::cout << "==============================================" << std::endl;
      char choice;
      std::cin >> choice;
      std::cin.ignore();

      switch(choice){
        case 'a': case 'A':
          scramble();
          break;
        case 'b': case 'B':
          change_Dictionary(Dictionary_file);
          break;
        case 'c': case 'C':
          Dictionary.clear();
          exit(0);
          break;
        default:
          scramble();
      }
      std::cout << std::endl << std::endl;
      //preload_Dictionary(Dictionary_file, 0);
    }

    return 0;
  } else {
    std::vector<std::string> args;
    args.assign(argv+1, argv+argc); // puts all arguments in vector of string
    Functions func;

    if(func.valid_args(args)) {
      std::string scramble;
      if(args.back()[0] != '-') {
        scramble = args.back();
        args.pop_back();
      }
      func.set_functions(args);
      if(func.help) {
        func.print_help();
        return 0;
      } else if(!scramble.empty()) {
        std::string Dictionary_file;
        std::ifstream settings("settings.cfg");

        load_settings(settings, Dictionary_file);

        preload_Dictionary(Dictionary_file, 0);

        std::stringstream text_flow;
        bool check = false;
        uint64_t i, leng;
        DictWord scrambled_DictWord = process_word(scramble);

        //compare?
        leng = Dictionary.size();

        if(leng < 1){
          std::cerr << "Looks like Dictionary is not loaded or missing..." << std::endl;
        }

        for(i = 0; i < leng; i++){
          check = check_if_pass(scrambled_DictWord, Dictionary.at(i));
          if(func.substring) {
            if(check) {
              text_flow << "Found word: " << Dictionary.at(i).word << std::endl;
            }
          } else {
            if(check && Dictionary.at(i).word.size() == scramble.size()){
              text_flow << "Found word: " << Dictionary.at(i).word << std::endl;
            }
          }
        }

        std::cout << text_flow.str();
        text_flow.str("");

        Dictionary.clear();
      } else {
        std::cerr << "[NO WORD TO UNSCRAMBLE]" << std::endl;
      }
    }
  }
  return 0;
}
