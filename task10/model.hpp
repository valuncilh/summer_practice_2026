#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>

class TruthTableModel {
public:
    struct Row {
        std::vector<int> vars;
        int result;
    };

    explicit TruthTableModel(const std::string& expr);
    const std::vector<char>& getVariables() const;
    const std::vector<Row>& getTable() const;

private:
    enum class NodeType { VAR, NOT, AND, OR };
    struct Node {
        NodeType type;
        char var_name = 0;
        std::unique_ptr<Node> left, right;
    };

    void tokenize(const std::string& expr);
    std::unique_ptr<Node> parseExpression();
    std::unique_ptr<Node> parseTerm();
    std::unique_ptr<Node> parseFactor();
    bool evaluate(const std::unique_ptr<Node>& node, const std::map<char, bool>& assignment) const;
    void generateTable();

    std::vector<std::string> tokens_;
    size_t pos_ = 0;
    std::vector<char> variables_;
    std::unique_ptr<Node> root_;
    std::vector<Row> table_;
};

#endif // MODEL_HPP