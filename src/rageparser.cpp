
#include "Ragetypes.hpp"
#include "arguments.hpp"
#include "command.hpp"
#include <string>
#include <unordered_map>
#include <vector>

#include "rageparse.hpp"


// actual parse context implementation
class Rage::RageParse::ParseContext {

    public:
    ParseContext () = default;
    Rage::command* parent_command;
    Rage::command* active_command;

    std::unordered_map<std::string, command*> persistent_long_flags_till_now; // till the active subcommand collects all defined flags long names
    std::unordered_map<std::string, command*> persistent_short_flags_till_now;

    // for each command loop and update the above 2 maps using their persistent_flag_maps


    //
    //
};


Rage::RageParse::RageParse () {
    this->context_ptr = std::make_unique<ParseContext> ();
}


std::vector<Rage::RageParse::Token> Rage::RageParse::Tokenizer 
(std::vector<std::string> RawTokens) {

    std::vector<Rage::RageParse::Token> tokens;

    auto processValue = [](std::string value)->std::string {
        auto index1 = value.find("\"");

        if(index1!=std::string::npos){
             value = value.substr(index1+1); 
             index1 = value.find("\"");
             value = value.substr(0,index1-0+1);
             return value;
        }else{
            return value;
        }


    };

    for (auto rawToken : RawTokens) {

        //long flags
        if( rawToken.size()>=2 and rawToken.substr(0,2)=="--"){// long-flag
            // START IF
            //it can be = syntax
            // it can be flag-argument syntax without argument
            rawToken=rawToken.substr(2);
            if(rawToken.find('=')!=std::string::npos){
                //left of it is flag right of it is value
                int it=rawToken.find('=');
                std::string flag_name = rawToken.substr(0,it);
                std::string flag_value = rawToken.substr(it);
                tokens.emplace_back(TokenType::LONG_FLAG,rawToken,flag_name);
                // processing the value is needed 
                flag_value=processValue(flag_value);
                tokens.emplace_back(TokenType::VALUE,rawToken,flag_value);
            }else{
                // no equal to is found
                // so just a flag (though may not be valid)
                tokens.emplace_back(TokenType::LONG_FLAG,rawToken,rawToken);
            }   
            // END IF
        }else if(rawToken.size()>=2 and rawToken[0]=='-' and rawToken[1]!='-'){ //short flags
            //START IF
            rawToken=rawToken.substr(1); //remove '-'

            if(rawToken.find('=')!=std::string::npos){
                 //left of it is (grouped or jsut one)flag right of it is value
                  int it=rawToken.find('=');
                std::string actual_flags = rawToken.substr(0,it);
                std::string flag_value = rawToken.substr(it);

                if(actual_flags.size()==1){
                     tokens.emplace_back(TokenType::SHORT_FLAG,rawToken,actual_flags);
                }else{
                    // >=1 size
                    tokens.emplace_back(TokenType::SHORT_FLAG_GROUPED,rawToken,actual_flags);
                }

                // processing the value i.e removing "" if needed 
                flag_value= processValue(flag_value);
                  tokens.emplace_back(TokenType::VALUE,rawToken,flag_value);
            }else{
                if(rawToken.size()>1){
                    tokens.emplace_back(TokenType::SHORT_FLAG_GROUPED,rawToken,rawToken);
                }else{
                    tokens.emplace_back(TokenType::SHORT_FLAG,rawToken,rawToken);
                }
            }
            // ENDIF
        }
    }
}
