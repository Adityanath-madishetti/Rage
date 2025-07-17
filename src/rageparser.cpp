
#include<vector>
#include<string>
#include"command.hpp"
#include"arguments.hpp"
#include"Ragetypes.hpp"
#include<unordered_map>

#include"rageparse.hpp"


//actual parse context implementation
class Rage::Rage_parse::Parse_conetxt{

    public:
        Rage::command* parent_command;
        Rage::command* active_command;
        
        std::unordered_map<std::string,Argument>persistent_flags_till_now; // till the active subcommand
        

        //   
        //







};


