#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include<vector>
#include<sstream>
#include<filesystem>
#include<unordered_set>
#include <bits/basic_string.h>

using namespace std;
namespace fs = std::filesystem;

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

    string command;

    getline(cin, command);
//check if the user wants to exit
    if(command == "exit"){
      exit(0);
    }
//check if the user wants to use the type command   
    if(command.substr(0,5)=="type "){
      unordered_set<string> validCommands ={"exit","echo","type"};
      string inputcommand = command.substr(5);
      bool found = false;
      
      int i =0;
      //check built-in commands
      if(validCommands.contains(inputcommand)){
        cout<<inputcommand<<" is a shell built-in"<<endl;
        found = true;
      }


      //search for user command in PATH directories
      string pathResult = find_in_path(inputcommand);
      if(!pathResult.empty()){
        cout<<inputcommand<<" is "<<pathResult<<endl;
        found = true;
        
      }
      if(!found){
        cout<<inputcommand<<": not found"<<endl;
      }
    }
    //check if the user wants to use the echo command
    else if(command.substr(0,5) == "echo "){
      string output  = command.substr(5);
      cout<<output<<endl;
      
         
    }
    //command not found case
    else{
      cout<< command<< ": command not found"<< endl;
    }

    
  }

}
