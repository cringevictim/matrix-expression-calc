#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <stack>
#include <algorithm>

#include "Matrix.h"

enum TokenType {
    Number, Operator, LeftParen, RightParen
};

struct Token {
    int typeT;
    std::string value;
};


std::string matrixToString(const Matrix& matrix) {
    std::ostringstream ss;
    ss << '[';
    for (size_t i = 0; i < matrix.elements.size(); ++i) {
        ss << '[';
        for (size_t j = 0; j < matrix.elements[i].size(); ++j) {
            ss << matrix.elements[i][j];
            if (j < matrix.elements[i].size() - 1) {
                ss << ',';
            }
        }
        ss << ']';
        if (i < matrix.elements.size() - 1) {
            ss << ',';
        }
    }
    ss << ']';
    return ss.str();
}

Matrix stringToMatrix(const std::string& str) {
    // Check for empty string
    if (str.empty()) {
        throw std::invalid_argument("Input string is empty");
    }

    // Check for the starting and ending brackets
    if (str.front() != '[' || str.back() != ']') {
        throw std::invalid_argument("Missing opening or closing bracket");
    }

    std::vector<std::vector<int>> matrix;
    std::istringstream ss(str.substr(1, str.size() - 2)); // Remove the outer brackets
    std::string row;

    // Expected row size, used to ensure all rows have the same length
    std::size_t expectedRowSize = 0;

    while (std::getline(ss, row, ']')) {
        // Remove any leading '[' or ',' from the row
        row.erase(std::remove(row.begin(), row.end(), '['), row.end());
        //row.erase(std::remove(row.begin(), row.end(), ','), row.end()); // Прибирає коми, закоментувати і в циклі розібрати строку 1,2,3 на вектор

        if (row.empty()) continue; // Skip empty rows, if any

        std::istringstream rowStream(row);
        
        std::vector<int> matrixRow;
        
        

        std::stringstream ss(row);
        std::string item;

        while (std::getline(ss, item, ',')) { // Використовуємо кому як роздільник
            try {
                // Спробувати конвертувати строку в число і додати у вектор
                matrixRow.push_back(std::stoi(item));
            }
            catch (const std::invalid_argument& e) {
                // Якщо конвертація не вдалася, можна або ігнорувати це число, або обробити помилку
                // Тут просто ігноруємо
            }
            catch (const std::out_of_range& e) {
                // Якщо число занадто велике для int, можна або ігнорувати, або обробити помилку
                // Тут просто ігноруємо
            }
        }

        // Check if all rows have the same number of elements
        if (expectedRowSize == 0) {
            expectedRowSize = matrixRow.size();
        }
        else if (matrixRow.size() != expectedRowSize) {
            throw std::invalid_argument("Row lengths are not consistent");
        }

        matrix.push_back(matrixRow);
    }
    Matrix result(matrix);
    return result;
}



Token matrixOperation(const Token& left, const Token& op, const Token& right) {
    if (op.value == "+") {
        return Token{ TokenType::Number, matrixToString(stringToMatrix(left.value) + stringToMatrix(right.value)) }; // Використання перевантаженого оператора '+' для додавання матриць
    }
    else if (op.value == "*") {
        return Token{ TokenType::Number, matrixToString(stringToMatrix(left.value) * stringToMatrix(right.value)) }; // Використання перевантаженого оператора '*' для множення матриць
    }
    else if (op.value == "-") {
        return Token{ TokenType::Number, matrixToString(stringToMatrix(left.value) - stringToMatrix(right.value)) };
    }
    else if (op.value == "-") {

    }
    else if (op.value == "-") {

    }
    else if (op.value == "-") {

    }
    else {
        throw std::invalid_argument("Непідтримуваний оператор: " + op.value);
    }
}

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    std::istringstream stream(input);
    char c;

    while (stream >> c) {
        if (std::isdigit(c) || c == '-' || c == '[' || c == ']') {
            // Це початок матриці або числа
            std::string number(1, c);
            while (stream.peek() != '+' && stream.peek() != '*' && stream.peek() != '-' && !std::isspace(stream.peek())) {
                stream >> c;
                number += c;
            }
            Token tmp = { TokenType::Number, number };
            tokens.push_back(tmp);
        }
        else if (c == '+' || c == '*' || c == '-') {
            Token tmp = { TokenType::Operator, std::string(1, c )};
            tokens.push_back(tmp);
        }
        else if (c == '(') {
            Token tmp = { TokenType::LeftParen, "(" };
            tokens.push_back(tmp);
        }
        else if (c == ')') {
            Token tmp;
            tmp.typeT = TokenType::Number;
            tmp.value = ")";
            tokens.push_back(tmp);
        }
        // Пропускаємо пробіли та інші символи, які не входять до складу виразів.
    }
    return tokens;
}

// Функція для перевірки пріоритету операторів
int getPrecedence(const Token& token) {
    if (token.value == "+" || token.value == "-") return 1;
    if (token.value == "*") return 2;
    // Можна додати інші оператори та їхні пріоритети
    return 0;
}

// Функція для перетворення списку токенів у абстрактне синтаксичне дерево (AST)
Token parseExpression(const std::vector<Token>& tokens) {
    // Для спрощення розглянемо лише випадок, коли вся матриця вважається одним "числом"
    std::stack<Token> values; // Стек для чисел та дужок
    std::stack<Token> ops; // Стек для операторів
    for (const auto& token : tokens) {
        if (token.typeT ==  TokenType::Number) {
            values.push(token);
        }
        else if (token.typeT == TokenType::Operator) {
            while (!ops.empty() && getPrecedence(ops.top()) >= getPrecedence(token)) {
                // Виконуємо операцію, яка знаходиться на вершині стеку ops
                Token op = ops.top();
                ops.pop();

                Token right = values.top(); // правий операнд
                values.pop();

                Token left = values.top(); // лівий операнд
                values.pop();

                // Обчислюємо результат операції та додаємо його назад до стеку значень
                // Потрібна функція для виконання матричних операцій, наприклад, matrixOperation(left, op, right)
                Token result = matrixOperation(left, op, right);
                values.push(result);
            }
            // Поміщаємо поточний оператор у стек операторів
            ops.push(token);
        }
        else if (token.typeT == TokenType::LeftParen) {
            ops.push(token);
        }
        else if (token.typeT == TokenType::RightParen) {
            // Виконуємо операції, поки не дійдемо до відповідної відкриваючої дужки
            while (!ops.empty() && ops.top().typeT != TokenType::LeftParen) {
                Token op = ops.top();
                ops.pop();

                Token right = values.top();
                values.pop();

                Token left = values.top();
                values.pop();

                Token result = matrixOperation(left, op, right);
                values.push(result);
            }
            // Видаляємо відкриваючу дужку зі стеку операторів
            if (!ops.empty()) ops.pop();
        }
    }

    // Виконуємо всі залишені операції у стеку ops
    while (!ops.empty()) {
        Token op = ops.top();
        ops.pop();

        Token right = values.top();
        values.pop();

        Token left = values.top();
        values.pop();

        Token result = matrixOperation(left, op, right);
        values.push(result);
    }

    // В кінці в стеку значень має залишитися один токен - це результат виразу
    return values.top();
}

// Функція для оцінки абстрактного синтаксичного дерева (AST) і обчислення результату
Matrix evaluateAST(const Token& astRoot) {
    // Якщо це "число" (матриця), то просто повертаємо його
    if (astRoot.typeT == TokenType::Number) {
        return stringToMatrix(astRoot.value);
    }
    throw std::runtime_error("Невідомий тип вузла в AST");
}

Matrix evaluateMatrixExpression(const std::string& expression) {
    std::vector<Token> tokens = tokenize(expression);

    Token astRoot = parseExpression(tokens);

    Matrix result = evaluateAST(astRoot);

    return result;
}


