#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include<vector>
#include<sstream>
#include<filesystem>

using namespace std;
namespace fs = std::filesystem;


bool isexecutable(const string& path){
  return access(path.c_str(),X_OK)==0;
}

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

int main(int argc, char *argv[]) {
  
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // TODO: Uncomment the code below to pass the first stage
  while(true){
    std::cout << "$ ";

    string command;

    getline(cin, command);

    if(command == "exit"){
      exit(0);
    }else if(command.substr(0,5)=="type "){
      string validCommands[] ={"exit","echo","type"};
      string inputcommand = command.substr(5);
      bool found = false;

      for(int i =0; i<size(validCommands);i++){
        if(validCommands[i]==inputcommand){
          cout<<inputcommand<<" is a shell builtin"<<endl;
          found = true;
        }
      }

      if(!found){
        vector<string> dirs = getpathdirs();
        for(const string& dir: dirs){
          string fullpath = dir + "/" + inputcommand;

          if(fs::exists(fullpath)&&fs::is_regular_file(fullpath)){
            if(isexecutable(fullpath)){
              cout<<inputcommand<< " is "<<fullpath<<endl;
            }
            found = true;
            break;
            
          }
        }
      }
      if(!found){
        cout<<inputcommand<<" not found"<<endl;
      }
    }
    else if(command.substr(0,5) == "echo "){
      string output  = command.substr(5);
      cout<<output<<endl;
      
         
    }
    else{
      cout<< command<< ": command not found"<< endl;
    }

    
  }

}
