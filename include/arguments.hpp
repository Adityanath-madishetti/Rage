#ifndef ARGUMENT_H
#define ARGUMENT_H

#include"command.hpp"


namespace Rage{


      enum class ArgType{
                        String,
                        Int,
                        Bool
    };


    
    class  Argument{

        std::string name;

        //for positional arguments

        std::string longname;
        char  shortname;

        Rage::ArgType type;

          bool is_variadic = false; 
        bool is_positional=false; //if not positional then it obviously flag
        std::string default_value;  

    };





}

#endif