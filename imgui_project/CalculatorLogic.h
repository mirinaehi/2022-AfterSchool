#ifndef CALCULATORLOGIC_H
#define CALCULATORLOGIC_H

#include <string>
#include <cctype>
#include <stdexcept>

// 계산기 로직을 담당하는 클래스
class CalculatorLogic {
private:
    std::string displayValue = "0"; // 현재 표시되는 값
    float num1 = 0.0f, num2 = 0.0f, result = 0.0f; // 입력된 숫자 및 계산 결과
    char operation = 0; // 현재 선택된 연산자
    bool newInput = true; // 새로운 입력 여부

public:
    const std::string& GetDisplayValue() const; // 디스플레이 값을 반환
    void HandleInput(char buttonValue); // 버튼 입력 처리

private:
    void Calculate(); // 계산 로직을 수행하는 함수
    void Clear(); // 계산기를 초기 상태로 클리어
};

#endif // CALCULATORLOGIC_H
