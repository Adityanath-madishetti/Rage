
#include<vector>
#include<string>
#include"command.hpp"
#include"arguments.hpp"
#include"Ragetypes.hpp"
#include<unordered_map>

#include"rageparse.hpp"


//actual parse context implementation
class Rage::Rage_parse::Parse_context{

    public:


        Parse_context()=default;
        Rage::command* parent_command;
        Rage::command* active_command;
        
        std::unordered_map<std::string,command*>persistent_long_flags_till_now; // till the active subcommand collects all defined flags long names
        std::unordered_map<std::string,command*>persistent_short_flags_till_now;

        // for each command loop and update the above 2 maps using their persistent_flag_maps
        

        //   
        //


};



Rage::Rage_parse::Rage_parse(){
    this->context_ptr=std::make_unique<Parse_context>();
}


std::vector<Rage::Rage_parse::Token> Rage::Rage_parse::Tokenizer(std::vector<std::string> RawTokens){


    std::vector<Rage::Rage_parse::Token> tokens;

    for(auto rawToken : RawTokens){

        if( rawToken.size()>2 and rawToken.substr(0,2)=="--"){

            rawToken=rawToken.substr(2);

            //check for equal to , if presnet then its LONG_FLAG_EQUAL

            int i=0;

            for(;i<rawToken.size();i++){
                if(rawToken[i]=='='){
                    
                }
            }






        }


    }


}


