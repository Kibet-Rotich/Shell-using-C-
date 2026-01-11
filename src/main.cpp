#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  using namespace std;
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
      for(int i =0; i<validCommands->size();i++){
        if(validCommands[i]==inputcommand){
          cout<<inputcommand<<" is a shell builtin"<<endl;
          found = true;
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
