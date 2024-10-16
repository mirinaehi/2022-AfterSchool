#include "CalculatorLogic.h"

const std::string& CalculatorLogic::GetDisplayValue() const {
    return displayValue;
}

void CalculatorLogic::HandleInput(char buttonValue) {
    if (isdigit(buttonValue)) {
        if (newInput) {
            displayValue = buttonValue;
            newInput = false;
        }
        else {
            displayValue += buttonValue;
        }
    }
    else {
        switch (buttonValue) {
        case 'C':
            Clear();
            break;
        case '=':
            if (operation != 0) {
                num2 = std::stof(displayValue);
                Calculate();
                displayValue = std::to_string(result);
                operation = 0;
                newInput = true;
            }
            break;
        default:
            if (operation == 0) {
                num1 = std::stof(displayValue);
                operation = buttonValue;
                newInput = true;
            }
            break;
        }
    }
}

void CalculatorLogic::Calculate() {
    switch (operation) {
    case '+':
        result = num1 + num2;
        break;
    case '-':
        result = num1 - num2;
        break;
    case '*':
        result = num1 * num2;
        break;
    case '/':
        if (num2 != 0)
            result = num1 / num2;
        else
            throw std::runtime_error("Division by zero"); // 0으로 나눌 때 예외 던지기
        break;
    }
}

void CalculatorLogic::Clear() {
    displayValue = "0";
    num1 = num2 = result = 0.0f;
    operation = 0;
    newInput = true;
}
