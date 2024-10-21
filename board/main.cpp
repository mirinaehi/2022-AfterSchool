#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

// 게시물을 저장할 구조체 정의
struct Post {
    std::wstring title;
    std::wstring author;
    int views;
    std::wstring date;
};

// 게시판 클래스를 정의합니다.
class BulletinBoard {
public:
    void addPost(const std::wstring& title, const std::wstring& author, int views, const std::wstring& date) {
        posts.push_back({ title, author, views, date });
    }

    // 게시물과 테이블을 그리는 메서드입니다.
    void draw(sf::RenderWindow& window, sf::Font& font) {
        drawHeader(window, font);

        const int startingY = 50;  // 처음 시작 Y 좌표
        const int lineHeight = 40; // 각 텍스트 줄의 높이

        // 모든 게시물을 순회하면서 그립니다.
        for (size_t i = 0; i < posts.size(); ++i) {
            float rowY = startingY + i * lineHeight;
            drawPost(window, font, posts[i], rowY);
            drawSeparatorLine(window, rowY + 35);  // 35로 조정하여 텍스트와의 간격 좁힘
        }
    }

private:
    void drawHeader(sf::RenderWindow& window, sf::Font& font) {
        sf::Text title(L"제목", font, 20);
        title.setPosition(50, 10);
        title.setFillColor(sf::Color::Black);

        sf::Text author(L"글쓴이", font, 20);
        author.setPosition(300, 10);
        author.setFillColor(sf::Color::Black);

        sf::Text views(L"조회수", font, 20);
        views.setPosition(500, 10);
        views.setFillColor(sf::Color::Black);

        sf::Text date(L"작성일", font, 20);
        date.setPosition(650, 10);
        date.setFillColor(sf::Color::Black);

        window.draw(title);
        window.draw(author);
        window.draw(views);
        window.draw(date);
    }

    void drawPost(sf::RenderWindow& window, sf::Font& font, const Post& post, float y) {
        sf::Text title(post.title, font, 20);
        title.setPosition(50, y);
        title.setFillColor(sf::Color::Blue);

        sf::Text author(post.author, font, 20);
        author.setPosition(300, y);
        author.setFillColor(sf::Color::Black);

        sf::Text views(std::to_wstring(post.views), font, 20);
        views.setPosition(500, y);
        views.setFillColor(sf::Color::Black);

        sf::Text date(post.date, font, 20);
        date.setPosition(650, y);
        date.setFillColor(sf::Color::Black);

        window.draw(title);
        window.draw(author);
        window.draw(views);
        window.draw(date);
    }

    void drawSeparatorLine(sf::RenderWindow& window, float y) {
        sf::RectangleShape line(sf::Vector2f(window.getSize().x - 50, 1));
        line.setFillColor(sf::Color(200, 200, 200));  // 회색 분리선
        line.setPosition(50, y);
        window.draw(line);
    }

    std::vector<Post> posts;  // 게시물 리스트 저장
};

int main() {
    // SFML 윈도우 생성
    sf::RenderWindow window(sf::VideoMode(800, 600), L"게시판");
    window.setFramerateLimit(60);  // 프레임 속도 제한

    // 폰트 로드
    sf::Font font;
    if (!font.loadFromFile("HANBatang.ttf")) {  // 한글 폰트 경로 설정
        std::cerr << "Error loading font\n";  // 폰트 로드 실패 시 오류 메시지 출력
        return -1;  // 프로그램 종료
    }

    BulletinBoard board;  // 게시판 객체 생성
    std::wstring currentInput;  // 현재 사용자 입력을 저장할 문자열
    bool isEnteringText = false;  // 텍스트 입력 모드 여부
    bool skipFirstChar = false;   // 첫 입력 문자('e') 무시 여부

    // 예시 게시물 추가
    board.addPost(L"제목 예시1", L"작성자1", 123, L"2023-09-15");
    board.addPost(L"제목 예시2", L"작성자2", 45, L"2023-09-16");
    board.addPost(L"제목 예시3", L"작성자3", 67, L"2023-09-17");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();  // 윈도우가 닫히면 프로그램 종료
            }
            if (event.type == sf::Event::TextEntered && isEnteringText) {
                if (skipFirstChar) {
                    skipFirstChar = false;  // 첫 글자는 무시하고 나머지는 저장
                    continue;
                }
                if (event.text.unicode == '\b' && !currentInput.empty()) {
                    // 백스페이스 처리 (마지막 문자 삭제)
                    currentInput.pop_back();
                }
                else {
                    // 유니코드 처리를 통해서, 문자를 문자열에 추가
                    currentInput += static_cast<wchar_t>(event.text.unicode);
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter && isEnteringText) {
                    // 사용자가 엔터 키를 눌렀을 때 게시물 추가
                    board.addPost(currentInput, L"새 작성자", 0, L"2023-09-18");
                    currentInput.clear();  // 현재 입력 초기화
                    isEnteringText = false;  // 텍스트 입력 모드 종료
                }
                if (event.key.code == sf::Keyboard::E) {
                    // E 키를 눌러 텍스트 입력 모드 시작
                    isEnteringText = true;
                    skipFirstChar = true;  // 첫번째 입력 무시
                }
            }
        }

        window.clear(sf::Color::White);  // 화면을 흰색으로 초기화

        // 현재 입력한 텍스트를 화면에 출력
        if (isEnteringText) {
            sf::Text inputText(L"> " + currentInput, font, 20);
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
