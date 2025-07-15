#include "command.hpp"
#include "arguments.hpp"
#include<string>



Rage::command&   Rage::command::set_action (  std::function<void(command*)> callback  ){
    this->action=callback;
    return *this;
}


Rage::command& Rage::command::set_long_descripion(std::string longText){
    this->long_description=longText;
    return *this;
}



Rage::command& Rage::command::set_name(std::string name){
    this->name=name;
     return *this;
}




Rage::command& Rage::command::set_short_description(std::string shortText){
    this->short_description=shortText;
     return *this;

}


bool Rage::command::get_bool_flag(const std::string& arg_name) {
    // Check registration
    if (registeredArguments.find(arg_name) == registeredArguments.end()) {
        throw std::runtime_error("No argument with given name: " + arg_name);
    }

    auto argument_definition = registeredArguments[arg_name];
    // Type check
    if (argument_definition.type != ArgType::Bool) {
        throw std::runtime_error("Argument is not of (flag or boolean or  non-variadic) type" + arg_name);
        return;
    }
    auto it = usedArgs.find(arg_name);
    // If not provided, fall back to default
    if (it == usedArgs.end()) {
        return argument_definition.default_value == "true";
    }
    std::string val = it->second;
    std::transform(val.begin(), val.end(), val.begin(), ::tolower);
    if (val.empty()) {
        return true;  // this means user just passed --flag
    }
    if (val == "true") return true;
    if (val == "false") return false;
    throw std::runtime_error("Invalid boolean value for flag " + arg_name + ": " + val);
}


Rage::int64 Rage::command::get_int_flag(const std::string& arg_name){

    if (registeredArguments.find(arg_name) == registeredArguments.end()) {
        throw std::runtime_error("No argument with given name: " + arg_name);
    }
    auto argument_definition = registeredArguments[arg_name];
    // Type check
    if (argument_definition.type != ArgType::Int) {
        throw std::runtime_error("Argument is not of (int64 or boolean or  non-variadic) type" + arg_name);
        return -1;
    }
    auto it = usedArgs.find(arg_name);
    if (it == usedArgs.end()) {
        return std::stoll(argument_definition.default_value);
    }
    return std::stoll(it->second);
}

std::string Rage::command::get_string_flag(const std::string& arg_name){

    if (registeredArguments.find(arg_name) == registeredArguments.end()) {
        throw std::runtime_error("No argument with given name: " + arg_name);
    }
    auto argument_definition = registeredArguments[arg_name];
    // Type check
    if ( argument_definition.is_positional or  argument_definition.type != ArgType::Int or argument_definition.is_variadic ) {
        throw std::runtime_error("Argument is not of (string or boolean or  non-variadic) type  : " + arg_name);
        return "";
    }
    auto it = usedArgs.find(arg_name);
    if(it==usedArgs.end()){
        return argument_definition.default_value;
    }
    return  usedArgs[arg_name];
}


//scope for imporvement is seperate variadics from the usual args and add customize delimiter

std::vector<std::string> Rage::command::get_string_list_flag(const std::string& arg_name){

    if (registeredArguments.find(arg_name) == registeredArguments.end()) {
        throw std::runtime_error("No argument with given name: " + arg_name);
    }
    auto argument_definition = registeredArguments[arg_name];

    if (argument_definition.is_positional or  (not argument_definition.is_variadic) ){
        throw std::runtime_error("No Variadic argument with given name: " + arg_name);
        return {};
    }
    auto it=usedListArgs.find(arg_name);
    if(it==usedListArgs.end()){
        return {argument_definition.default_value};
    }
    return usedListArgs[arg_name];
}

std::string Rage::command::get_positional_arg(const std::string& arg_name){

    if (registeredArguments.find(arg_name) == registeredArguments.end()) {
        throw std::runtime_error("No argument with given name: " + arg_name);
    }
    auto argument_definition = registeredArguments[arg_name];
    if(!argument_definition.is_positional){
        throw std::runtime_error("no positional argument :"+arg_name+"is registered");
        return "";
    }
    auto it = usedArgs.find(arg_name);

    if(it==usedArgs.end()){  // assuming required restriction is handeled while parsing, need not to check here
        return ""; // ifeel returning erro is meaning lesscause username cna be given eithe rfrom flags or positioanlargs
    }
    return it->second;

}




void Rage::command::security_check_flag_creation(std::string name, std::string long_name, char short_name){

    if ( (long_name.empty() or short_name == '\0') or name.empty() ) {
            throw std::runtime_error("long and short names cannot be empty for a flag.");
    }
    if(this->registeredArguments.find(name)!=this->registeredArguments.end()){
        throw std::runtime_error("a flag or argument with  name "+ name+"is already registered under this command");
    }   
    if (!long_name.empty() && long_flags_map.find(long_name) != long_flags_map.end()) {
        throw std::runtime_error("Long flag name '--" + long_name + "' already in use.");
    }
    if (short_name != '\0' && short_flags_map.find(short_name) != short_flags_map.end()) {
        throw std::runtime_error("Short flag name '-" + std::string(1, short_name) + "' already in use.");
    }

}



void Rage::command::add_boolean_flag(std::string name,std::string long_name,char short_name,bool default_value){


    this->security_check_flag_creation(name,long_name,short_name);
    
    // first update registeredargument
    
    std::string new_default_value=(default_value)?"true":"false";

    auto new_argument =  Flag (name, long_name,short_name,ArgType::Bool,new_default_value,false).createArgument();

    //register this in  map
    this->registeredArguments[name]=new_argument;  // registration done in registered arguments
    this->long_flags_map[long_name]=name;
    this->short_flags_map[short_name]=name;
    flags.push_back(new_argument);

}



void Rage::command::add_int_flag(std::string name,std::string long_name,char short_name,Rage::ArgType type,Rage::int64 default_value){
        
        this->security_check_flag_creation(name,long_name,short_name);

        std::string new_default_value = std::to_string(default_value);

        auto new_argument = Flag(name,long_name,short_name,ArgType::Int,new_default_value,false).createArgument();

        this->registeredArguments[name]=new_argument;  // registration done in registered arguments
        this->long_flags_map[long_name]=name;
        this->short_flags_map[short_name]=name;
        flags.push_back(new_argument);

}

void Rage::command::add_string_flag(std::string name,std::string long_name,char short_name,Rage::ArgType type,std::string default_value,bool is_variadic){
    
    this->security_check_flag_creation(name,long_name,short_name);

    std::string new_default_value = default_value;
    auto new_argument = Flag(name,long_name,short_name,ArgType::String,new_default_value,is_variadic).createArgument();
    
    this->registeredArguments[name]=new_argument;  // registration done in registered arguments
    this->long_flags_map[long_name]=name;
    this->short_flags_map[short_name]=name;
    flags.push_back(new_argument);

}


void Rage::command::add_positional_arg(const Rage::PositionalArg&p_arg){


    if(p_arg.name.empty()){
        throw std::runtime_error("name can not be empty for positional argument declaration");
    }

    if(this->registeredArguments.find(p_arg.name)!=this->registeredArguments.end()){
        throw std::runtime_error("a flag or argument with  name "+ name+"is already registered under this command");
        return;
    }

    auto new_argument =PositionalArg(p_arg.name , p_arg.required).createArgument();

    this->registeredArguments[p_arg.name] = new_argument;
    this->positionals.push_back(new_argument);
}


bool Rage::command::has_descendant(command* suspect) {
    if (this == suspect) return true;
    for (auto& [name, subcmd] : sub_commands_map) {
        if (subcmd->has_descendant(suspect)) {
            return true;
        }
    }
    return false;
}



void Rage::command::add_subcommand(command* subcmd) {

    // check for existing subcommands 
    // check for alias of newly enetring command conflct
    // check for cycle
    // tehn do work


            if (this->sub_commands_map.find(subcmd->name) != this->sub_commands_map.end()) {
                throw std::runtime_error("Subcommand or else some other subcommand alias name '" + subcmd->name + "' already exists. for this parent");
            }




            for (const auto& alias : subcmd->cmd_aliases) {
                if (this->sub_commands_map.find(alias) != this->sub_commands_map.end()) {
                        throw std::runtime_error("Alias '" + alias + "' for subcommand '" + subcmd->name + "' conflicts with an existing command or alias.");
                }
            }


            if(subcmd->has_descendant(this)){
                throw std::runtime_error("loop detected between  the subcommand: "+subcmd->name+"parent_command: "+this->name);
            }

            // nwo it means parent is ready to accept the subcomamnd

            // now loop and upload aliases to subcommandx

             for (const auto& alias : subcmd->cmd_aliases) {
                this->sub_commands_map[alias]=subcmd;
            }



}



