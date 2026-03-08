#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include<vector>
#include<sstream>
#include<filesystem>
#include<unordered_set>
#include <bits/basic_string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;
namespace fs = std::filesystem;


vector<string> tokenize(const string& str) {
    vector<string> tokens;
    string token;
    stringstream ss(str);
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

//function to check if a file is executable
bool isexecutable(const string& path){
  return access(path.c_str(),X_OK)==0;
}

//function to get directories from PATH environment variable
vector<string> getpathdirs(){
  vector<string> dirs;
  const char* PATH = getenv("PATH");
  if(!PATH) return dirs; 
  string path_str(PATH);
  stringstream ss(path_str);
  string segment;


  while(getline(ss,segment,':')){
    if(!segment.empty()){
      dirs.push_back(segment);
    }
  }
  return dirs;
}

//function to find command in PATH directories
string find_in_path(const string& command)
{
  vector<string> pathDirs = getpathdirs();

  for(const string& dir : pathDirs){
    fs::path fullpath = fs::path(dir) / command;
    if(fs::exists(fullpath) && isexecutable(fullpath.string())){
      return fullpath.string();
    }
  }
  return "";
}




void handle_exit(const vector<string>& tokens) {
    if (tokens.size() > 0 && tokens[0] == "exit") {
        exit(0);
    }
}

void handle_type(const vector<string>& tokens) {
    unordered_set<string> validCommands = {"exit", "echo", "type", "pwd"};
    string inputcommand = tokens.size() > 1 ? tokens[1] : "";

    if (validCommands.contains(inputcommand)) {
        cout << inputcommand << " is a shell builtin" << endl;
        return;
    }

    string pathResult = find_in_path(inputcommand);
    if (!pathResult.empty()) {
        cout << inputcommand << " is " << pathResult << endl;
        return;
    }

    cout << inputcommand << ": not found" << endl;
}

void handle_echo(const vector<string>& tokens) {
    string output;
    for (size_t i = 1; i < tokens.size(); ++i) {
        output += tokens[i];
        if (i != tokens.size() - 1) {
            output += " ";
        }
    }
    cout << output << endl;
}

void handle_pwd(const vector<string>& tokens){
  char path[128];
  if (getcwd(path, sizeof(path)) != NULL) {
      printf("%s\n", path);
  }   
}

void handle_external_command(const vector<string>& tokens) {
    string command = tokens.empty() ? "" : tokens[0];
    if (command.empty()) return;

    string pathResult = find_in_path(command);
    if (!pathResult.empty()) {
        vector<char*> args;
        for (const string& token : tokens) {
            args.push_back(const_cast<char*>(token.c_str()));
        }
        args.push_back(nullptr);

        pid_t pid = fork();
        if (pid == 0) {
            execv(pathResult.c_str(), args.data());
            cerr << "Error executing command" << endl;
            exit(1);
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
        } else {
            cerr << "Fork failed" << endl;
        }
    } else {
        cerr << command << ": command not found" << endl;
    }
}

int main(int argc, char *argv[]) {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    while (true) {
        std::cout << "$ ";
        string userinput;
        getline(cin, userinput);
        vector<string> tokens = tokenize(userinput);
        string command = tokens.empty() ? "" : tokens[0];

        if (command == "exit") {
            handle_exit(tokens);
        } else if (command == "type") {
            handle_type(tokens);
        } else if (command == "echo") {
            handle_echo(tokens);
        }else if(command == "pwd") {
            handle_pwd(tokens);
        } else if (!command.empty()) {
            handle_external_command(tokens);
        }
    }
    return 0;
}