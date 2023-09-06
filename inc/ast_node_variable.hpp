#ifndef AST_NODE_VARIABLE_HPP
#define AST_NODE_VARIABLE_HPP

#include "ast_node.hpp"
#include "variables_environment.hpp"

#include <string> // std string

namespace fgear {

class VariableNode : public ASTNode {
public:
    explicit VariableNode(std::shared_ptr<Environment> a_env, std::string const& a_name, std::string const& a_property);
    ~VariableNode() = default;

    std::string name() const;
    std::string property() const;

    //TODO: make to number through member
    void operator()() override;

private:
    std::shared_ptr<Environment> m_env;
    std::string m_property;
    std::string m_name;
};

} // namespace fgear

#endif // AST_NODE_VARIABLE_HPP