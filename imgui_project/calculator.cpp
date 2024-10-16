#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <string>

// 계산기에서 사용할 전역 변수들
std::string displayValue = "0";  // 현재 표시되는 값
float num1 = 0.0f, num2 = 0.0f, result = 0.0f;  // 입력된 숫자 및 계산 결과
char operation = 0;  // 현재 선택된 연산자
bool newInput = true;  // 새로운 입력 여부

// 함수 프로토타입
void RenderCalculator();
void Calculate();

int main() {
    // SFML 윈도우 생성
    sf::RenderWindow window(sf::VideoMode(600, 800), "ImGui + SFML Calculator");
    window.setFramerateLimit(60);  // 프레임 제한 설정

    // ImGui-SFML 초기화
    ImGui::SFML::Init(window);

    // 폰트 로드를 포함한 IO 초기화
    ImGuiIO& io = ImGui::GetIO();  // ImGui IO 참조 얻기
    io.Fonts->Clear();  // 기존 폰트 초기화
    io.Fonts->AddFontFromFileTTF("Arial.ttf", 36.0f); // 폰트 파일과 크기 설정
    io.Fonts->Build(); // 필요한 경우 폰트 아틀라스 빌딩. 주로 필요하지 않음.

    // 폰트 텍스처 업데이트가 확실할 수 있도록 초기화
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    sf::Texture fontTexture;
    fontTexture.create(width, height);
    fontTexture.update(pixels);
    io.Fonts->TexID = reinterpret_cast<void*>(fontTexture.getNativeHandle());

    // SFML에서 시간 관리를 위한 클록(clock)
    sf::Clock deltaClock;

    // 메인 루프
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();  // 윈도우가 닫히면 프로그램 종료
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // 계산기 UI 렌더링
        RenderCalculator();

        // 렌더링 코드
        window.clear(sf::Color(30, 30, 30));  // 배경 색상 설정
        ImGui::SFML::Render(window);
        window.display();
    }

    // 리소스 해제 및 종료
    ImGui::SFML::Shutdown();
    return 0;
}

void RenderCalculator() {
    ImGui::Begin("Calculator", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    // 현재 계산기의 디스플레이 값 표시
    ImGui::Text(displayValue.c_str());

    // 숫자 버튼 및 연산자 배열 정의
    const char* buttons[] = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", "C", "=", "+"
    };

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int index = i * 4 + j;

            // 각 버튼에 대한 동작 정의
            if (ImGui::Button(buttons[index], ImVec2(80, 80))) {
                char buttonValue = buttons[index][0];

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
                        displayValue = "0";
                        num1 = num2 = result = 0.0f;
                        operation = 0;
                        newInput = true;
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
            if (j < 3) ImGui::SameLine();  // 버튼을 같은 줄에 이어서 표시
        }
    }

    ImGui::End();
}

// 계산 로직을 수행하는 함수
void Calculate() {
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
            displayValue = "Error";  // 0으로 나눌 때 오류 메시지 출력
        break;
    }
}
