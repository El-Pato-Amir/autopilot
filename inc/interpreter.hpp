#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "commands.hpp"
#include "executor.hpp"
#include "mediator.hpp"

#include <string> // std::string
#include <unordered_map> // variables names
#include <memory> // shared ptr

//TODO: actual design!
// ideas:
//TODO: a Block templated class that will be the basic execution unit
//TODO: maybe the Block will be a variadic template
//TODO: maybe the execute will be variadic template
//TODO: a way to "store" the current script after it is interpreted
//TODO: do the interpreter holds the executor or vice versa?

namespace fgear {

class Interpreter {
public:
    Interpreter(std::string const& a_translator_file,std::shared_ptr<Mediator> const& a_mediator);
    ~Interpreter() = default;

    

private:
    std::unordered_map<std::string, typename Var> m_local_variables;
    std::unordered_map<std::string, typename Var> m_calculations;
    std::shared_ptr<Mediator> m_mediator;
    Executor m_executor;
    //TODO: maybe use a thread pool here

};

} // namespace fgear

#endif // INTERPRETER_HPP