#include "model.hpp"
#include <sstream>
#include <algorithm>

TruthTableModel::TruthTableModel(const std::string& expr) {
    if (expr.empty()) throw std::invalid_argument("Empty expression");
    tokenize(expr);
    pos_ = 0;
    root_ = parseExpression();
    if (pos_ != tokens_.size()) throw std::invalid_argument("Nil tokens after statement");
    generateTable();
}

void TruthTableModel::tokenize(const std::string& expr) {
    std::istringstream iss(expr);
    std::string token;
    while (iss >> token) {
        std::string cleaned;
        for (char c : token) {
            if (c == '(' || c == ')') {
                if (!cleaned.empty()) {
                    tokens_.push_back(cleaned);
                    cleaned.clear();
                }
                tokens_.emplace_back(1, c);
            } else {
                cleaned += c;
            }
        }
        if (!cleaned.empty()) tokens_.push_back(cleaned);
    }

    for (const auto& t : tokens_) {
        if (t == "AND" || t == "OR" || t == "NOT" || t == "(" || t == ")") continue;
        if (t.length() == 1 && t[0] >= 'A' && t[0] <= 'Z') {
            variables_.push_back(t[0]);
        } else {
            throw std::invalid_argument("Inccorect token: " + t);
        }
    }
    std::sort(variables_.begin(), variables_.end());
    variables_.erase(std::unique(variables_.begin(), variables_.end()), variables_.end());
}

std::unique_ptr<TruthTableModel::Node> TruthTableModel::parseExpression() {
    auto left = parseTerm();
    while (pos_ < tokens_.size() && tokens_[pos_] == "OR") {
        pos_++;
        auto right = parseTerm();
        auto node = std::make_unique<Node>();
        node->type = NodeType::OR;
        node->left = std::move(left);
        node->right = std::move(right);
        left = std::move(node);
    }
    return left;
}

std::unique_ptr<TruthTableModel::Node> TruthTableModel::parseTerm() {
    auto left = parseFactor();
    while (pos_ < tokens_.size() && tokens_[pos_] == "AND") {
        pos_++;
        auto right = parseFactor();
        auto node = std::make_unique<Node>();
        node->type = NodeType::AND;
        node->left = std::move(left);
        node->right = std::move(right);
        left = std::move(node);
    }
    return left;
}

std::unique_ptr<TruthTableModel::Node> TruthTableModel::parseFactor() {
    if (pos_ >= tokens_.size()) throw std::invalid_argument("Foolish end of expression");
    std::string token = tokens_[pos_];

    if (token == "NOT") {
        pos_++;
        auto child = parseFactor();
        auto node = std::make_unique<Node>();
        node->type = NodeType::NOT;
        node->left = std::move(child);
        return node;
    }
    if (token == "(") {
        pos_++;
        auto expr = parseExpression();
        if (pos_ >= tokens_.size() || tokens_[pos_] != ")")
            throw std::invalid_argument("Missing closing square");
        pos_++;
        return expr;
    }
    if (token.length() == 1 && token[0] >= 'A' && token[0] <= 'Z') {
        pos_++;
        auto node = std::make_unique<Node>();
        node->type = NodeType::VAR;
        node->var_name = token[0];
        return node;
    }
    throw std::invalid_argument("unexpected token: " + token);
}

bool TruthTableModel::evaluate(const std::unique_ptr<Node>& node, const std::map<char, bool>& assignment) const {
    if (!node) return false;
    switch (node->type) {
        case NodeType::VAR: return assignment.at(node->var_name);
        case NodeType::NOT: return !evaluate(node->left, assignment);
        case NodeType::AND: return evaluate(node->left, assignment) && evaluate(node->right, assignment);
        case NodeType::OR:  return evaluate(node->left, assignment) || evaluate(node->right, assignment);
    }
    return false;
}

void TruthTableModel::generateTable() {
    size_t n = variables_.size();
    size_t total = 1ULL << n;
    for (size_t mask = 0; mask < total; ++mask) {
        std::map<char, bool> assignment;
        Row row;
        for (size_t i = 0; i < n; ++i) {
            bool val = (mask >> (n - 1 - i)) & 1;
            assignment[variables_[i]] = val;
            row.vars.push_back(val ? 1 : 0);
        }
        row.result = evaluate(root_, assignment) ? 1 : 0;
        table_.push_back(row);
    }
}

const std::vector<char>& TruthTableModel::getVariables() const { return variables_; }
const std::vector<TruthTableModel::Row>& TruthTableModel::getTable() const { return table_; }