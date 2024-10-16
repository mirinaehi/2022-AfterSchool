#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <string>

// Calculator 클래스
class Calculator {
private:
    std::string displayValue = "0";  // 현재 표시되는 값
    float num1 = 0.0f, num2 = 0.0f, result = 0.0f;  // 입력된 숫자 및 계산 결과
    char operation = 0;  // 현재 선택된 연산자
    bool newInput = true;  // 새로운 입력 여부

    // 숫자 버튼 및 연산자 배열 정의
    const char* buttons[16] = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", "C", "=", "+"
    };

public:
    // 생성자 & 소멸자
    Calculator() = default;
    ~Calculator() = default;

    // 메인 루프
    void Run() {
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

        // 배경 텍스처 로드
        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("background.png")) {
            return;  // 이미지 로드 실패 시 종료
        }
        sf::Sprite backgroundSprite(backgroundTexture);

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

                if (event.type == sf::Event::KeyPressed) {
                    HandleKeyPress(event.key.code);
                }
            }

            ImGui::SFML::Update(window, deltaClock.restart());

            // 1. 그리기 시작 (배경)
            window.clear();
            window.draw(backgroundSprite);

            // 2. IMGUI 요소 그리기
            RenderCalculator();

            // 3. 그린 모든 것 화면에 표시
            ImGui::SFML::Render(window);
            window.display();
        }

        // 리소스 해제 및 종료
        ImGui::SFML::Shutdown();
    }

private:
    // 키보드 입력 처리
    void HandleKeyPress(sf::Keyboard::Key key) {
        switch (key) {
        case sf::Keyboard::Num0:
        case sf::Keyboard::Numpad0: HandleInput('0'); break;
        case sf::Keyboard::Num1:
        case sf::Keyboard::Numpad1: HandleInput('1'); break;
        case sf::Keyboard::Num2:
        case sf::Keyboard::Numpad2: HandleInput('2'); break;
        case sf::Keyboard::Num3:
        case sf::Keyboard::Numpad3: HandleInput('3'); break;
        case sf::Keyboard::Num4:
        case sf::Keyboard::Numpad4: HandleInput('4'); break;
        case sf::Keyboard::Num5:
        case sf::Keyboard::Numpad5: HandleInput('5'); break;
        case sf::Keyboard::Num6:
        case sf::Keyboard::Numpad6: HandleInput('6'); break;
        case sf::Keyboard::Num7:
        case sf::Keyboard::Numpad7: HandleInput('7'); break;
        case sf::Keyboard::Num8:
        case sf::Keyboard::Numpad8: HandleInput('8'); break;
        case sf::Keyboard::Num9:
        case sf::Keyboard::Numpad9: HandleInput('9'); break;
        case sf::Keyboard::Slash: HandleInput('/'); break;
        case sf::Keyboard::Multiply: HandleInput('*'); break;
        case sf::Keyboard::Subtract: HandleInput('-'); break;
        case sf::Keyboard::Add: HandleInput('+'); break;
        case sf::Keyboard::Enter: HandleInput('='); break;
        case sf::Keyboard::Escape: HandleInput('C'); break;
        default: break;
        }
    }

    // 버튼 입력 처리
    void HandleInput(char buttonValue) {
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

    // 계산기 UI를 렌더링하는 함수
    void RenderCalculator() {
        ImGui::Begin("Calculator", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

        // 현재 계산기의 디스플레이 값 표시
        ImGui::Text(displayValue.c_str());

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                int index = i * 4 + j;

                // "C" 버튼에 대해 버튼 색상을 빨간색으로 설정
                if (strcmp(buttons[index], "C") == 0) {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.5f, 0.5f, 1.0f)); // 선택 사항: 호버 색상
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));  // 선택 사항: 클릭 활성 색상
                }

                // 각 버튼에 대한 동작 정의
                if (ImGui::Button(buttons[index], ImVec2(80, 80))) {
                    HandleInput(buttons[index][0]);
                }

                // "C" 버튼 색상 설정을 되돌리기
                if (strcmp(buttons[index], "C") == 0) {
                    ImGui::PopStyleColor(3); // Button, ButtonHovered, ButtonActive 스타일 해제
                }

                if (j < 3) ImGui::SameLine();  // 버튼을 같은 줄에 이어서 표시
            }
        }

        ImGui::End();
    }
};

int main() {
    Calculator calculator;
    calculator.Run();
    return 0;
}
