#include"arguments.hpp"
#include"command.hpp"
#include<vector>
#include<string>
#include<unordered_map>




// as of now dont delete them just put aside might be useful some time

Rage::Argument Rage::Flag::createArgument(){
    return Argument(this->name,
                    this->long_name,
                    this->short_name,
                    this->type,
                    this->default_value,
                    this->variadic,
                    false, //is_positional
                    this->arg_req ,// is_required
                    this->short_help_msg
                    );
}



Rage::Argument Rage::PositionalArg::createArgument(){
    return Argument(this->name,
                    this->name, //long name
                    '\0',
                    ArgType::String,
                    "",
                    false,
                    true, //is_positional
                    this->required ,
                    this->short_help_msg
                    );
}