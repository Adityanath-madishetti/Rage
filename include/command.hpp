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
                
                std::vector<std::string> aliases;  
                
                // make it public if u feel its needed
                struct Help {
                        std::string short_description;
                        std::string long_description;
                        std::string usage;
                        std::vector<std::string> examples;

                        Help() = default;

                        Help(std::string short_description,
                            std::string long_description,
                            std::string usage,
                            std::vector<std::string> examples)
                            : short_description(std::move(short_description)),
                            long_description(std::move(long_description)),
                            usage(std::move(usage)),
                            examples(std::move(examples)) {}
                  };

                  Help helpObj;
                

               //  use internal name as keys (these collect every thing that appeared and valid on taht command)
                std::map<std::string, std::string> usedArgs;  //internamname->userinputvalues for them. (flags and positionasl)
                std::map<std::string,std::string> usedFlagMaps;
                // std::map<std::string,std::string>usedPositionalsMap;
                std::map<std::string, std::vector<std::string>> usedListFlags; //internamname->userinputvalues for them ( as of now only flags are allowed)
                std::vector<std::string>usedPositionals; // to collect the positionals in order even though they are jumbeled between flags


                //-------------------------------- localised ----------------------------------------------------------
                std::vector<std::string> cmd_aliases; // these aliases are for command ndependnetly, any parentc ommand may or may not accept tehm based on cosnistency
                std::map<std::string, command*> sub_commands_map;  //  (Predefined) subcommands
                std::vector<Argument> positionals; // (predefined)
                std::vector<Argument>flags; //(predefiend ) //also contains persitant
                std::vector<Argument> persistent_flags; //(predefined) and are injected into child in run time when reaching that command
                std::map<std::string, Argument> registeredArguments; // (predefined) all argments (flags and  positionals) with their internal names to arg object  (loclaised arguments)
                std::map<std::string,Argument> registered_flag_map; //predefined
                std::map<std::string,Argument> registered_positional_arg_map; //predefined
                // ------------------------------------------------------------------------------------------------------




               // ----------------------------------------------------------------- needed in traversals -----------------------------------------------------------------

                std::map<std::string,Argument>persistent_flags_map; //(predefined) this is for  fast look of persistant flag map defined in this command and effective afetr wards.  // fill this when user adds a persistant flag
                  // used when u need arg of persistants declared there
                //----------------------------------------------------------------------------------------------------------------------------------


                // useful while pasring
                std::map<std::string, std::string> long_flags_map; //(prefefined) registered  with not "name" rateher "long_name" (longname->name)
                std::map<char, std::string> short_flags_map;   // (predefined)  registered with not "name" rateher "short_name" (shortname->name)


                std::function<void(command*)>action;  // this is call back becaus user wriet logic , mostly logic exists only if its leaf
                // when calling this as part of executing , u need to send this as parametere to action.
                std::function<std::string(command*)>helpFunc; // send this as parametere
                

                command* parent_command=nullptr;

                //--------------------------------utils----------------------------
                void security_check_flag_creation(std::string name, std::string long_name, char short_name);
                bool has_descendant(command* suspect);
                void  localize_flags(Rage::Argument new_argument,std::string name,std::string long_name,char short_name);

                public:

                  

                // --------------------------- adders ------------------------------
                    void add_boolean_flag(std::string name,std::string long_name,char short_name,bool default_value ,bool arg_req,std::string help_msg); // bool is not allowed to be variadic;
                    void add_int_flag(std::string name,std::string long_name,char short_name,Rage::int64 default_value,bool arg_req,std::string help_msg);
                    void add_string_flag(std::string name,std::string long_name,char short_name,std::string default_value,bool arg_req,bool is_variadic,std::string help_msg);

                    void add_subcommand(command* sub_cmd);  // persistent flag setup is done at run time while entering into new command. not while adding subcommand
                    void add_positional_arg(const Rage::PositionalArg&p_arg,std::string help_msg); 

                    void add_persistent_flag_bool(std::string name,std::string long_name,char short_name,bool default_value,bool arg_req,std::string help_msg);
                    void add_persistent_flag_int(std::string name,std::string long_name,char short_name,Rage::int64 default_value,bool arg_req,std::string help_msg);
                    void add_persistent_flag_string(std::string name,std::string long_name,char short_name,std::string default_value,bool is_variadic,bool arg_req,std::string help_msg);
                    
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
                    command& set_usage(std::string);
                    command& set_example(std::vector<std::string>exmaples);
                    command& set_action(std::function<void(command*)>); 

                    template<typename... varType>
                        command& set_aliases(varType... args) {

                            // lmbda
                            auto validate_string_alias = [this](const std::string& alias) -> void {
                                if (std::find(this->cmd_aliases.begin(), this->cmd_aliases.end(), alias) != this->cmd_aliases.end()) {
                                    throw std::runtime_error("Alias '" + alias + "' is already set for this command.");
                                }
                            };

                            auto add_string_alias = [this](const std::string& alias) {
                                

                                // .. while adding alias here u are checking wethere parent has any registered subcommands or atelast subcommand aliases in it
                                // if no conflicting then. push them to alias vector
                                // while adding this as subcommand to any of the parentcommand it sees its aliases and if possible then uploads to itself 
                                if (this->parent_command) {
                                    if (this->parent_command->sub_commands_map.find(alias) != this->parent_command->sub_commands_map.end()) {
                                        throw std::runtime_error("Alias '" + alias + "' conflicts with existing subcommand or alias in parent.");
                                    }
                                    this->parent_command->sub_commands_map[alias] = this;
                                }



                                this->cmd_aliases.push_back(alias);
                                // DO NOT touch sub_commands_map here. do it in add_subcmmnd
                            };


                            // Validate first
                            (validate_string_alias(args), ...);
                            // Then add
                            (add_string_alias(args), ...);
                            return *this;
                        }

                    //constructor
                    command(std::string name,std::string short_description){ //constructor 
                            this->name=name;
                            this->helpObj.short_description=short_description;
                            this->helpFunc=[this](Rage::command*cmd)->std::string{
                            // some concatneted logic
                            };
                            this->action=[this](Rage::command*cmd)->void {
                                this->helpFunc(this);
                            };
                    }
                    command()=default;
    };

}


#endif






