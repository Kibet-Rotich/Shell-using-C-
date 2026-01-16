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



int main(int argc, char *argv[]) {
  
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;


  while(true){
    std::cout << "$ ";




    string userinput;

    getline(cin, userinput);

    vector<string> tokens = tokenize(userinput);
    string command = tokens.empty() ? "" : tokens[0];
    bool found = false;


    //check if the user wants to exit
    if(command == "exit"){
      exit(0);
    }
    //check if the user wants to use the type command   
    if(command=="type"){
      unordered_set<string> validCommands ={"exit","echo","type"};
      string inputcommand = tokens.size() > 1 ? tokens[1] : "";
      
      
      int i =0;
      //check built-in commands
      if(validCommands.contains(inputcommand)){
        cout<<inputcommand<<" is a shell builtin"<<endl;
        found = true;
      }


      //search for user command in PATH directories
      if(!found){
        string pathResult = find_in_path(inputcommand);
        if(!pathResult.empty()){
          cout<<inputcommand<<" is "<<pathResult<<endl;
          found = true;
          
        }
      }
        if(!found){
          cout<<inputcommand<<": not found"<<endl;
          found = true;
        }
    }
    //check if the user wants to use the echo command
    else if(command == "echo"){
      string output;
      for(size_t i =1; i<tokens.size(); ++i){
        output += tokens[i];
        if(i != tokens.size() -1){
          output += " ";
        }
      }
      cout<<output<<endl;
      found = true;
      
         
    }
    else if(!command.empty()){
      string pathResult = find_in_path(command);
      found = !pathResult.empty();
      //command found in PATH directories
      if(!pathResult.empty()){
        //execute the command
        vector<char*> args;
        for(const string& token : tokens){
          args.push_back(const_cast<char*>(token.c_str()));
        }
        args.push_back(nullptr);

        pid_t pid = fork();
        if(pid == 0){
          //child process
          execv(pathResult.c_str(), args.data());
          //if execv returns, there was an error
          cerr<< "Error executing command"<< endl;
          exit(1);
        }
        else if(pid > 0){
          //parent process
          int status;
          waitpid(pid, &status, 0);
        }
        else{
          cerr<< "Fork failed"<< endl;
        }
      }else{
        cerr<< command <<": command not found"<< endl;
      }
    }
  }
  return 0;
}