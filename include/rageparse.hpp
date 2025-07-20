#ifndef RAGE_PARSE
#define RAGE_PARSE

#include "arguments.hpp"
#include "command.hpp"
#include <memory>
#include <string>

namespace Rage {
class RageParse {
    private:
    Rage::command* cmd;

    enum class TokenType {
        LONG_FLAG,                       /* --name nath */
        SHORT_FLAG,                      /* -n name */
        // EQUAL,                          // = 
        SHORT_FLAG_GROUPED,              /* -la 2  or -la  or -l2 which is equal to -l=2; */
        IDENTIFIER,                      /* subcommand ,positional arg, flags */
        TERMINATOR,                      /* -- */
        VALUE                            /* "value"  can be for positional args or flags */
    };


    class Token {
        public:
        Token () = default;

        Rage::RageParse::TokenType type;
        std::string raw_string;
        std::string value; //flag name or terminator or any thing

        Token(TokenType type,std::string rawToken,std::string token_value){
            this->type=type;
            this->raw_string=rawToken;
            this->value=token_value;
        }



        // methods
    };


    class ParseContext;
    std::unique_ptr<Rage::RageParse::ParseContext> context_ptr;

    std::vector<Token> Tokenizer (std::vector<std::string> rawTokens);

    public:
    RageParse ();

    RageParse (Rage::command* cmd) {
        this->cmd = cmd;
    }

    // argc,argv
    void Parse (int count, std::vector<std::string> raw_args) {
    }
};
} // namespace Rage

#endif