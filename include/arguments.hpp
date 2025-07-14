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

  class Argument{


   
    public:
    std::string name;

    // for positional arguments

    std::string longname;  // 99% of time longname and name are same, just for readable code we use name variable so now priority flag internally can be priorityInternal but command works when used only "priority"
    char shortname;

    Rage::ArgType type;

    bool is_variadic = false;
    bool is_positional = false; // if not positional then it obviously flag
    std::string default_value;
  };
}

#endif