#include "controller.hpp"
#include "model.hpp"
#include "view.hpp"
#include <iostream>
#include <string>

int LogicController::run() {
    try {
        std::string line;
        if (!std::getline(std::cin, line)) {
            std::cerr << "Error: no input provide\n";
            return 1;
        }
        if (line.empty()) {
            std::cerr << "Error: empty input\n";
            return 1;
        }
        TruthTableModel model(line);
        TableView::print(model);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}