#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "command.hpp"


namespace Rage
{

 

  enum class ArgType{
    String,
    Int,
    Bool
  };


  // not recomended for user
  class Argument{

    public:

    

    // for a  given command its mandatory to ahve all argument names unique



      std::string name; //for both
      std::string longname;  //for flags
      char shortname; //for flags
      Rage::ArgType type; //for flags, for positional args ist alwasy string
      bool is_variadic = false;  //for flags only
      bool is_positional = false; //for both
      bool required=true; //for positionals
      std::string default_value; //for flags


    //constructor;
    Argument(std::string name,std::string longname,char shortname, Rage::ArgType type,std::string default_value, bool is_variadic=false,bool is_positional=false,bool required=true){
      this->name=name;
      this->longname=longname;
      this->shortname=shortname;
      this->type=type;
      this->is_variadic=is_variadic;
      this->is_positional=is_positional;
      this->default_value=default_value;
      this->required=required;
    }

    Argument()=default;
  };


  // exposed to user

        class Flag {
                        std::string name;
                        std::string long_name;
                        char short_name = '\0';
                        Rage::ArgType type;
                        std::string default_value = "";
                        bool variadic = false;

                      Argument cretaeArgument();
              public:
                      friend class Rage::command;
                      Flag(std::string name,std::string long_name,char short_name,Rage::ArgType type,std::string default_value,bool is_variadic=false){
                        this->name=name;
                        this->long_name=long_name;
                        this->short_name=short_name;
                        this->type=type;
                        this->variadic=is_variadic;
          }
          Flag()=default;
      };
      class PositionalArg {
                std::string name;
                bool required = true;
          
                friend class Rage::command;

                Argument cretaeArgument();
                public:
                
                  

                  PositionalArg(std::string name,bool required=true){
                    this->name=name;
                    this->required=required;
                  }
      };


}

#endif