#include"arguments.hpp"
#include"command.hpp"
#include<vector>
#include<string>
#include<unordered_map>




// as of now dont delete them just put aside might be useful some time

Rage::Argument Rage::Flag::cretaeArgument(){
    return Argument(this->name,
                    this->long_name,
                    this->short_name,
                    this->type,
                    this->default_value,
                    this->variadic,
                    false, //is_positional
                    false // is_required
                    );
}



Rage::Argument Rage::PositionalArg::cretaeArgument(){
    return Argument(this->name,
                    this->name, //long name
                    '\0',
                    ArgType::String,
                    "",
                    false,
                    true, //is_positional
                    this->required 
                    );
}