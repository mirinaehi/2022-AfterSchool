#include "CalculatorGUI.h"
#include <iostream>

// 메인 루프
void CalculatorGUI::Run() {
    // SFML 윈도우 생성
    sf::RenderWindow window(sf::VideoMode(600, 800), "ImGui + SFML Calculator");
    window.setFramerateLimit(60); // 프레임 제한 설정

    // ImGui-SFML 초기화
    ImGui::SFML::Init(window);

    // 폰트 로드를 포함한 IO 초기화
    ImGuiIO& io = ImGui::GetIO(); // ImGui IO 참조 얻기
    io.Fonts->Clear(); // 기존 폰트 초기화
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
        return; // 이미지 로드 실패 시 종료
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    // SFML에서 시간 관리를 위한 클럭(clock)
    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close(); // 윈도우가 닫히면 프로그램 종료
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

// 키보드 입력 처리
void CalculatorGUI::HandleKeyPress(sf::Keyboard::Key key) {
    try {
        switch (key) {
        case sf::Keyboard::Num0:
        case sf::Keyboard::Numpad0: logic.HandleInput('0'); break;
        case sf::Keyboard::Num1:
        case sf::Keyboard::Numpad1: logic.HandleInput('1'); break;
        case sf::Keyboard::Num2:
        case sf::Keyboard::Numpad2: logic.HandleInput('2'); break;
        case sf::Keyboard::Num3:
        case sf::Keyboard::Numpad3: logic.HandleInput('3'); break;
        case sf::Keyboard::Num4:
        case sf::Keyboard::Numpad4: logic.HandleInput('4'); break;
        case sf::Keyboard::Num5:
        case sf::Keyboard::Numpad5: logic.HandleInput('5'); break;
        case sf::Keyboard::Num6:
        case sf::Keyboard::Numpad6: logic.HandleInput('6'); break;
        case sf::Keyboard::Num7:
        case sf::Keyboard::Numpad7: logic.HandleInput('7'); break;
        case sf::Keyboard::Num8:
        case sf::Keyboard::Numpad8: logic.HandleInput('8'); break;
        case sf::Keyboard::Num9:
        case sf::Keyboard::Numpad9: logic.HandleInput('9'); break;
        case sf::Keyboard::Slash: logic.HandleInput('/'); break;
        case sf::Keyboard::Multiply: logic.HandleInput('*'); break;
        case sf::Keyboard::Subtract: logic.HandleInput('-'); break;
        case sf::Keyboard::Add: logic.HandleInput('+'); break;
        case sf::Keyboard::Enter: logic.HandleInput('='); break;
        case sf::Keyboard::Escape: logic.HandleInput('C'); break;
        default: break;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// 계산기 UI를 렌더링하는 함수
void CalculatorGUI::RenderCalculator() {
    ImGui::Begin("Calculator", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    // 디스플레이 값의 오른쪽 정렬
    float calculatorWidth = ImGui::GetWindowContentRegionWidth(); // 계산기 창의 너비
    float textWidth = ImGui::CalcTextSize(logic.GetDisplayValue().c_str()).x; // 텍스트의 너비 계산

    ImGui::SetCursorPosX(calculatorWidth - textWidth); // 오른쪽으로 정렬
    ImGui::TextUnformatted(logic.GetDisplayValue().c_str()); // 값 출력

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
                try {
                    logic.HandleInput(buttons[index][0]);
                }
                catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
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

