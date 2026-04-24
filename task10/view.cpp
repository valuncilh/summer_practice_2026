#include "view.hpp"
#include <iostream>

void TableView::print(const TruthTableModel& model) {
    const auto& vars = model.getVariables();
    const auto& table = model.getTable();

    for (char v : vars) std::cout << v << " ";
    std::cout << "Result\n";

    for (const auto& row : table) {
        for (int v : row.vars) std::cout << v << " ";
        std::cout << row.result << "\n";
    }
}