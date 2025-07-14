#ifndef COMMAND_H
#define COMMAND_H

#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<map>
#include<functional>
#include"arguments.hpp"
namespace Rage{


    


    class command{

                private :

                // meta data
                std::string name;
                std::string short_description;
                std::string long_description;
                
                std::map<std::string, std::string> usedArgs; // raw input
                std::map<std::string, std::vector<std::string>> usedListArgs; //raw input

                std::vector<Argument> positionals; // predefined
                std::vector<Argument>flags; //predefiend
                std::map<std::string, command*> sub_commands; //predefined or registered

                std::function<void(command*)>action;



                command* parent_command=nullptr;
                
                
                public:
                    void add_flag(const Argument& arg);
                    void add_subcommand(command* subcmd);
                    void add_positionalArgs(const Argument& arg);



                std::string getString(const std::string& name);
                    int getInt(const std::string& name);
                    bool getBool(const std::string& name);
                    std::vector<std::string> getList(const std::string& name);
                    



                    //setters
                    command& set_name(std::string);
                    command& set_long_descripion(std::string);
                    command& set_short_description(std::string);
                    command& set_action(std::function<void(command*)>); 



                    command(){ //constructor 

                    }


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