#include "lexer.h"
#include "parser.h"
#include "evaluator.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace script_lang;

void printParserErrors(const std::vector<std::string>& errors) {
    std::cerr << "-parser errors:\n";
    for (const auto& error : errors) {
        std::cerr << "\t" << error << "\n";
    }
}

int runProgram(const std::string& input) {
    Lexer lexer(input);
    Parser parser(std::move(lexer));
    auto program = parser.ParseProgram();
    
    if (parser.hasErrors()) {
        printParserErrors(parser.getErrors());
        return 1;
    }
    
    Evaluator evaluator;
    auto evaluated = evaluator.evaluate(program.get());
    
    if (auto error = std::dynamic_pointer_cast<Error>(evaluated)) {
        std::cerr << error->inspect() << std::endl;
        return 1;
    }
    
    if (evaluated && !std::dynamic_pointer_cast<Null>(evaluated)) {
        std::cout << evaluated->inspect() << std::endl;
    }
    
    return 0;
}

int runREPL() {
    std::cout << "Script Language REPL\n";
    std::cout << "Press Ctrl+C to exit\n\n";
    
    std::string line;
    while (true) {
        std::cout << ">>> ";
        if (!std::getline(std::cin, line)) {
            break;
        }
        
        if (line.empty()) {
            continue;
        }
        
        runProgram(line);
    }
    
    return 0;
}

int runFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return runProgram(buffer.str());
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return runREPL();
    } else if (argc == 2) {
        return runFile(argv[1]);
    } else {
        std::cerr << "Usage: " << argv[0] << " [script_file]" << std::endl;
        return 1;
    }
}