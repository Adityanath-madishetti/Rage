#ifndef RAGE_PARSE
#define RAGE_PARSE

#include "arguments.hpp"
#include   "command.hpp"
#include<memory>
#include <string>

 namespace Rage{
    class Rage_parse{

        private:
            Rage::command* cmd;

            enum class TokenType{
                LONG_FLAG, // --name nath
                SHORT_FLAG, //-n name
                LONG_FLAG_WITH_EQUAL, //--name="nath" or --name=""
                SHORT_FLAG_WITH_EQUAL, // -n="aditya" or -n=aditya
                SHORT_FLAG_GROUPED, // -la 2  or -la  or -l2 which is eual to -l=2;
                SHORT_FLAG_GROUPED_WITH_EQUAL, //-la="val"
                IDENTIFIER, // subcommand ,positional arg
                TERMINATOR, // --
                VALUE  // "value"
            };
            class Token{
                    
                public:
                    Token()=default;

                    Rage::Rage_parse::TokenType type;
                    std::string raw_string;

                    std::string value;

                        // flag related datat (these shoudl be used only on flag tokens)

                    std::string flag_name;
                    std::string flag_value;

                        //methods
            };

            class Parse_context;
            std::unique_ptr<Rage::Rage_parse::Parse_context>context_ptr;    

            std::vector<Token> Tokenizer(std::vector<std::string> rawTokens);

        public:
        Rage_parse();

        Rage_parse( Rage::command* cmd){
            this->cmd=cmd;
        }
        
        //argc,argv
        void Parse(int count,std::vector<std::string>raw_args ){

        }


    };
}





#endif