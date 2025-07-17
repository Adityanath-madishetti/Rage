#ifndef RAGE_PARSE
#define RAGE_PARSE

#include "arguments.hpp"
#include   "command.hpp"
#include<memory>
#include <string>;

 namespace Rage{
    class Rage_parse{

        private:
            Rage::command* cmd;
            class Parse_conetxt;
            std::unique_ptr<Rage::Rage_parse::Parse_conetxt>context_ptr;
            

        public:

        Rage_parse(){

        }


        Rage_parse( Rage::command* cmd){
            this->cmd=cmd;
        }
        
        void Parse(int count,std::vector<std::string>raw_args ){

        }


    };
}




#endif