#include "main.hpp"

dict::dict(){
    //memset(letters, 0, 26 * sizeof(int));
    std::fill(std::begin(letters), std::end(letters), 0);
    pass = true;
}

std::vector<std::string> Functions::param_list = {"-h", "--help", "-s", "--substr"};
std::vector <dict> dictionary;

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

dict process_word(const std::string &here){
    uint64_t i, length = here.length();
    dict temporary;
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
    return temporary;
}

bool check_if_pass(const dict &source, dict &current){
    uint64_t i;
    for(i = 0; i < 26; i++){
        if(current.letters[i] > source.letters[i]){
          current.pass = false;
          break;
        }
    }
    return current.pass;
}

void load_settings(std::ifstream &settings, std::string &dictionary_file){
    if(settings.is_open()){
        getline(settings, dictionary_file);
        settings.close();
    }else{
        std::ofstream new_settings("settings.cfg");
        dictionary_file = "sowpods.txt";
        new_settings << dictionary_file;
        new_settings.close();
        std::cout << "Generating config file..." << std::endl;
    }
}

void preload_dictionary(const std::string& dictionary_file, bool message){
    std::string temporary;
    std::ifstream file(dictionary_file.c_str());

    if(dictionary.size() > 0){
        dictionary.clear();
    }

    if(file.is_open()){
        if(message) {
            std::cout << "Dictionary file found, loading dictionary..." << std::endl;
        }
        while(!file.eof()){
            std::getline(file, temporary);
            if (temporary.size() < 1){
                continue;
            }
            dictionary.push_back(process_word(temporary));
        }
        if(message) {
            std::cout << "Dictionary loaded" << std::endl;
        }
        file.close();
    }else{
        std::cerr << "Error preloading dictionary file!" << std::endl;
    }
}

void scramble(const std::string &dictionary_file, bool substring){
    std::stringstream text_flow;

    std::string scrambled;
    dict scrambled_dict;

    bool check = false;
    uint64_t i, leng;

    std::cout << "Enter string to scramble: ";

    while(true){
        std::cin >> scrambled;
        std::cin.ignore();

        if(scrambled.length() > MAX_SCRAMBLED_SIZE){
          std::cout << "Please re-enter again,(" << MAX_SCRAMBLED_SIZE << ") letters only!: ";
          continue;
        }
        break;
    }

    scrambled_dict = process_word(scrambled);

    //compare?
    leng = dictionary.size();

    if(leng < 1){
        std::cerr << "Looks like dictionary is not loaded or missing..." << std::endl;
    }

    for(i = 0; i < leng; i++){
        check = check_if_pass(scrambled_dict, dictionary.at(i));
        if(check){
          text_flow << "Found word: " << dictionary.at(i).word << std::endl;
        }
    }

    std::cout << text_flow.str();
    text_flow.str("");

    std::cout << "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=" << std::endl;

    dictionary.clear();
}

void change_dictionary(std::string &dictionary_file){
    std::string temp;
    std::cout << "Enter dictionary file: ";
    std::getline(std::cin, temp);
    dictionary_file = temp;
    std::ifstream settings("settings.cfg");
    std::cout << "Setting dictionary file to " << temp << std::endl;
    load_settings(settings, temp);
    preload_dictionary(dictionary_file);
}

void Functions::print_help()
{
    std::cout << "Format: " << std::endl;
    std::cout << "  [PROGRAM] [WORD]" << std::endl;
    std::cout << "  [PROGRAM] [PARAMETERS] [WORD]" << std::endl;
    std::cout << "Options: " << std::endl;
    std::cout << "  -h, --help" << "\t\t\t\t\t" << "Displays help text" << std::endl;
    std::cout << "  -s, --substr" << "\t\t\t\t\t" << "Prints out substrings as well" << std::endl;
}

bool Functions::valid_args(const std::vector<std::string>& args)
{
    std::vector<std::string> param_copy = param_list;
    for(int i = 0; i<args.size(); i++) {
        if(args[i][0] != '-') // ignores non-parameters
            continue;
        bool check = false;
        for(int j = 0; j<param_copy.size(); j++) {
            if(args[i] == param_copy[j]) {
                check = true;
                param_copy.erase(param_copy.begin()+j);
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
    for(int i = 0; i<args.size(); i++) {
        if(args[i] == "-h" || args[i] == "--help")
            help = true;
        else if(args[i] == "-s" || args[i] == "--substr")
            substring = true;
    }
}

int main(int argc, char* argv[]){
    if(argc == 1) {
        dictionary.reserve(100);

        std::cout << "Word unscrambler by pvzzombs" << std::endl;
        std::cout << __DATE__ << ", " << __TIME__ << ", " << __cplusplus << "; " << MAJOR_VER << "." << MINOR_VER << "." << CHANGE_VER << std::endl;

        std::string dictionary_file;
        std::ifstream settings("settings.cfg");

        std::cout << "Finding config file..." << std::endl;
        load_settings(settings, dictionary_file);

        std::cout << "Preloading dictionary..." << std::endl;
        preload_dictionary(dictionary_file);

        while(true){
        std::cout << "Press any key of choice: " << std::endl;
        std::cout << "(A) Scramble. (b) Change dictionary. (c) Exit." << std::endl;
        std::cout << "==============================================" << std::endl;
        char choice;
        std::cin >> choice;
        std::cin.ignore();

        switch(choice){
          case 'a': case 'A':
            scramble(dictionary_file);
          break;
          case 'b': case 'B':
            change_dictionary(dictionary_file);
          break;
          case 'c': case 'C':
            dictionary.clear();
            exit(0);
          break;
          default:
            scramble(dictionary_file);
        }
            std::cout << std::endl << std::endl;
            preload_dictionary(dictionary_file, 0);
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
                std::string dictionary_file;
                std::ifstream settings("settings.cfg");

                load_settings(settings, dictionary_file);

                preload_dictionary(dictionary_file, 0);

                std::stringstream text_flow;
                bool check = false;
                uint64_t i, leng;
                dict scrambled_dict = process_word(scramble);

                //compare?
                leng = dictionary.size();

                if(leng < 1){
                    std::cerr << "Looks like dictionary is not loaded or missing..." << std::endl;
                }

                for(i = 0; i < leng; i++){
                    check = check_if_pass(scrambled_dict, dictionary.at(i));
                    if(func.substring) {
                        if(check) {
                            text_flow << "Found word: " << dictionary.at(i).word << std::endl;
                        }
                    } else {
                        if(check && dictionary.at(i).word.size() == scramble.size()){
                            text_flow << "Found word: " << dictionary.at(i).word << std::endl;
                        }
                    }
                }

                std::cout << text_flow.str();
                text_flow.str("");

                dictionary.clear();
            } else {
                std::cerr << "[NO WORD TO UNSCRAMBLE]" << std::endl;
            }
        }
    }
    return 0;
}
