#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>
namespace Rage {
class command;

enum class ArgType { String, Int, Bool };

// not recomended for user
class Argument {
    private:
    // for a  given command its mandatory to ahve all argument names unique

    std::string name;               /* for both */
    std::string longname;           /* for flags */
    char shortname;                 /* for flags */
    Rage::ArgType type;             /* for flags, for positional args ist alwasy string */
    bool is_variadic   = false;     /* for flags only */
    bool is_positional = false;     /* for both */
    bool required = true;           /* for positionals its their requirement and for flags its their arg_req */
    std::string default_value;      /* for flags */
    std::string short_help;

    public:
    // constructor;
    Argument (std::string name,
    std::string longname,
    char shortname,
    Rage::ArgType type,
    std::string default_value,
    bool is_variadic           = false,
    bool is_positional         = false,
    bool required              = true,
    std::string short_help_msg = "")
    : name (name), longname (longname), shortname (shortname), type (type),
      is_variadic (is_variadic), is_positional (is_positional), default_value (default_value),
      required (required), short_help (short_help_msg) {
    }

    Argument () = default;
};

// exposed to user

class Flag {
    public:
    Argument create_argument ();
    Flag (std::string name,
    std::string long_name,
    char short_name,
    Rage::ArgType type,
    std::string short_help_msg,
    std::string default_value,
    bool is_variadic = false,
    bool arg_req     = true) {
        this->name           = name;
        this->long_name      = long_name;
        this->short_name     = short_name;
        this->type           = type;
        this->variadic       = is_variadic;
        this->short_help_msg = short_help_msg;
        this->arg_req        = arg_req;
    }
    Flag () = default;

    private:
    std::string name;
    std::string long_name;
    char short_name = '\0';
    Rage::ArgType type;
    std::string default_value = "";
    bool variadic             = false;
    std::string short_help_msg;
    bool arg_req;

    friend class Rage::command;
    friend class command;
};
class PositionalArg {
    public:
    PositionalArg (std::string name, std::string short_help_msg, bool required = true) {
        this->name           = name;
        this->required       = required;
        this->short_help_msg = short_help_msg;
    }

    private:
    std::string name;
    bool required = true;
    std::string short_help_msg;
    friend class Rage::command;

    Argument create_argument ();
};

} // namespace Rage

#endif