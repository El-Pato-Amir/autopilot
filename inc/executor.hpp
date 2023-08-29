#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "commands.hpp"

#include <unordered_map> // commands map
#include <functional> // std::function

namespace fgear {

class Executor {
public:
    Executor();

    typename Var execute(Command const& a_command, typename Var a_var);

private:
    std::unordered_map<Command, std::function<typename Var(typename Var)>> m_commands;    
};

} // namespace fgear


#endif // EXECUTOR_HPP