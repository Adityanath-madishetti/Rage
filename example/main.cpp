#include "command.hpp"

int main () {
    Rage::command root = Rage::command ("rootcmd", "no help to this");
    auto rootComd      = &root;

    Rage::command sub1 = Rage::command ("sub1", "no help to this sub1");
    auto sub1Cmd       = &sub1;

    rootComd->add_subcommand (sub1Cmd);
}