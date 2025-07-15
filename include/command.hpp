#ifndef COMMAND_H
#define COMMAND_H

#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<map>
#include<functional>
#include"arguments.hpp"
#include"rageparse.hpp"
#include <type_traits>
#include"Ragetypes.hpp"
namespace Rage{


    


    class command{


                private :

                friend class Rage::Rage_parse;

                // meta data
                std::string name;  //internal use
                std::string short_description;
                std::string long_description;
                std::vector<std::string> aliases;  
                

               // Both use internal name as keys
                std::map<std::string, std::string> usedArgs;  //internamname->userinputvalues for them. (flags and positionasl)
                std::map<std::string, std::vector<std::string>> usedListArgs; //internamname->userinputvalues for them ( as of now only flags are allowed)
                std::vector<std::string>usedPositionals; // to collect the positionals in order even though they are jumbeled between flags




                std::map<std::string, command*> sub_commands_map;  //  (Predefined) subcommands
                std::vector<Argument> positionals; // (predefined)
                std::vector<Argument>flags; //(predefiend )
                std::vector<Argument> persistent_flags; //(predefined) and are injected into child in run time when reaching taht command
                std::map<std::string, Argument> registeredArguments; // (predefined) all argments (flags and  positionals) with their internal names to arg object 


                
                std::map<std::string, std::string> long_flags_map; //(prefefined) registered  with not "name" rateher "long_name" (longname->name)
                std::map<char, std::string> short_flags_map;   // (predefined)  registered with not "name" rateher "short_name" (shortname->name)


                std::function<void(command*)>action;  // this is call back becaus user wriet logic , mostly logic exists only if its leaf
                // when calling this as part of executing , u need to send this as parametere to action.
                std::function<std::string(command*)>help; // send this as parametere
                



                command* parent_command=nullptr;
                
                // --------------------------- adders ------------------------------
                public:
                    void add_boolean_flag(std::string name,std::string long_name,char short_name,bool default_value); // bool is not allowed to be variadic;
                    void add_int_flag(std::string name,std::string long_name,char short_name,Rage::ArgType type,Rage::int64 default_value);
                    void add_string_flag(std::string name,std::string long_name,char short_name,Rage::ArgType type,std::string default_value);
                    void add_subcommand(command* sub_cmd); 
                    void add_positionalArgs(const Rage::PositionalArg&p_arg); // change it as soon as possible
                    
                    //------------------ getters -------------------------------------
                    // for positional arg
                    std::string get_positional_arg(const std::string& name);

                    //for flags
                    std::string get_string_flag(const std::string& name);
                    Rage::int64  get_int_flag(const std::string& name);
                    bool get_bool_flag(const std::string& name);
                    std::vector<std::string> get_string_list_flag(const std::string& name);
                    



                  //------------------ setters -------------------------------------
                    command& set_name(std::string);
                    command& set_long_descripion(std::string);
                    command& set_short_description(std::string);
                    command& set_action(std::function<void(command*)>); 

                    template<typename... varType>
                    command& set_aliases(varType... args){

                        // for each arg  push into aliases vector go to parent  i.e this->parent if not nullptr then put  
                        // alias in this->command->sub_commands[arg]=this;

                        //als is alias
                        auto add_string_alias = [this](const std::string& als){
                            this->push_back(als);
                            if(!this->parent_command) return;
                            this->parent_command->sub_commands_map[als]=this;
                        };


                        (add_string_alias(args),...); //see fold expression guide
                        return *this;
                    }



                    //constructor
                    command(std::string name,std::string short_description){ //constructor 
                            this->name=name;
                            this->short_description=short_description;
                            this->help=[this](Rage::command*cmd)->std::string{
                                // some concatneted logic
                            };
                            this->action=[this](Rage::command*cmd)->void {
                                this->help(this);
                            };
                            
                    }

                    command()=default;


    };






}


#endif






/*
class Command {
public:
    // Basic metadata
    std::string name;               // Command name: e.g., "add", "task"
    std::string description;        // Help text for the command

    // Flags and positional arguments
    std::vector<Argument> arguments; // List of all defined flags and positionals

    // Subcommands
    std::map<std::string, Command*> subcommands;

    // Link to parent (optional but useful)
    Command* parent = nullptr;

    // Execution callback (only for leaf commands)
    std::function<void(const Command&)> action = nullptr;

    // Parsed values
    std::map<std::string, std::string> usedArgs;  // Stores parsed values (as strings)

    // Active child after parsing
    Command* activeSubcommand = nullptr;

    // -----------------------
    // Public Methods
    // -----------------------

    // Adders
    void addFlag(const Argument& arg);
    void addPositional(const Argument& arg);
    void addSubcommand(Command* sub);

    // Getters (type-safe)
    std::string getArg(const std::string& name) const;
    bool getBool(const std::string& name) const;
    int getInt(const std::string& name) const;

    // Utilities
    bool isLeaf() const;                // Has no subcommands
    bool hasArg(const std::string& name) const;
    void setAction(std::function<void(const Command&)> act);
};


*/