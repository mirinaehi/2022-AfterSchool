#ifndef CALCULATORGUI_H
#define CALCULATORGUI_H

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "CalculatorLogic.h"

// GUI를 담당하는 클래스
class CalculatorGUI {
private:
    CalculatorLogic logic; // 계산 논리의 인스턴스

    // 숫자 버튼 및 연산자 배열 정의
    const char* buttons[16] = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", "C", "=", "+"
    };

public:
    CalculatorGUI() = default;
    ~CalculatorGUI() = default;

    void Run(); // 메인 루프

private:
    void HandleKeyPress(sf::Keyboard::Key key); // 키보드 입력 처리
    void RenderCalculator(); // 계산기 UI를 렌더링하는 함수
};

#endif // CALCULATORGUI_H
