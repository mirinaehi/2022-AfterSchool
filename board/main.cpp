#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

// 게시판 클래스를 정의합니다.
class BulletinBoard {
public:
    // 게시물을 추가하는 메서드입니다.
    void addPost(const std::wstring& post) {
        posts.push_back(post);  // 게시물 목록에 새로운 게시물을 추가합니다.
    }

    // 게시물을 그리는 메서드입니다.
    void draw(sf::RenderWindow& window, sf::Font& font) {
        const int startingY = 50;  // 처음 시작 Y 좌표
        const int lineHeight = 30; // 각 텍스트 줄의 높이

        // 모든 게시물을 순회하면서 그립니다.
        for (size_t i = 0; i < posts.size(); ++i) {
            sf::Text text(posts[i], font, 20);  // 텍스트를 생성하고 설정합니다.
            text.setPosition(50, startingY + i * lineHeight);  // 텍스트 위치 설정
            text.setFillColor(sf::Color::Black);  // 텍스트 색상을 검정으로 설정
            window.draw(text);  // 텍스트를 윈도우에 그립니다.
        }
    }

private:
    std::vector<std::wstring> posts;  // 게시물을 저장할 벡터입니다.
};

int main() {
    // SFML 윈도우 생성
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bulletin Board");
    window.setFramerateLimit(60);  // 프레임 속도 제한

    // 폰트 로드
    sf::Font font;
    if (!font.loadFromFile("HANBatang.ttf")) {
        std::cerr << "Error loading font\n";  // 폰트 로드 실패 시 오류 메시지 출력
        return -1;  // 프로그램 종료
    }

    BulletinBoard board;  // 게시판 객체 생성
    std::wstring currentInput;  // 현재 사용자 입력을 저장할 문자열
    bool isEnteringText = false;  // 텍스트 입력 모드 여부

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();  // 윈도우가 닫히면 프로그램 종료
            }
            if (event.type == sf::Event::TextEntered && isEnteringText) {
                if (event.text.unicode == '\b' && !currentInput.empty()) {
                    // 백스페이스 처리 (마지막 문자 삭제)
                    currentInput.pop_back();
                }
                else if (event.text.unicode < 128 && event.text.unicode != '\b') {
                    // 입력 문자를 문자열에 추가
                    currentInput += static_cast<wchar_t>(event.text.unicode);
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter && isEnteringText) {
                    // 사용자가 엔터 키를 눌렀을 때 게시물 추가
                    board.addPost(currentInput);
                    currentInput.clear();  // 현재 입력 초기화
                    isEnteringText = false;  // 텍스트 입력 모드 종료
                }
                if (event.key.code == sf::Keyboard::E) {
                    // E 키를 눌러 텍스트 입력 모드 시작
                    isEnteringText = true;
                }
            }
        }

        window.clear(sf::Color::White);  // 화면을 흰색으로 초기화

        // 현재 입력한 텍스트를 화면에 출력
        if (isEnteringText) {
            sf::Text inputText( L"> " + currentInput, font, 20);
            inputText.setPosition(50, 20);  // 텍스트 위치 설정
            inputText.setFillColor(sf::Color::Black);  // 텍스트 색상 설정
            window.draw(inputText);  // 텍스트를 그립니다.
        }

        // 모든 게시물을 그립니다.
        board.draw(window, font);

        window.display();  // 화면에 결과를 보여줍니다.
    }

    return 0;  // 프로그램 종료
}
