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



void Rage::command::add_boolean_flag(std::string name,std::string long_name,char short_name,bool default_value){


    
    if(this->registeredArguments.find(name)!=this->registeredArguments.end()){
        throw std::runtime_error("a flag with  name "+ name+"is already registered under thsi command");
        return;
    }   

    // first update registeredarguments

    // you can also give error if long_name and short name are empty

    std::string new_default_value=(default_value)?"true":"false";

    auto new_argument =  Flag (name, long_name,short_name,ArgType::Bool,new_default_value,false).cretaeArgument();

    //register this in  map
    this->registeredArguments[name]=new_argument;  // registration done in registered arguments
    this->long_flags_map[long_name]=name;
    this->short_flags_map[short_name]=name;
    flags.push_back(new_argument);

}






