#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <limits>

// 게시물을 저장할 구조체 정의
struct Post {
    std::wstring title;    // 게시물 제목
    std::wstring author;   // 작성자
    std::wstring content;  // 게시물 내용
    int views;             // 조회수
    std::wstring date;     // 작성일
};

// 게시판 클래스를 정의합니다.
class BulletinBoard {
public:
    // 게시물을 추가하는 메서드입니다.
    void addPost(const std::wstring& title, const std::wstring& author, const std::wstring& content, int views, const std::wstring& date) {
        posts.push_back({ title, author, content, views, date });
    }

    // 게시물 표를 그리고 리스트를 표시하는 메서드입니다.
    void drawList(sf::RenderWindow& window, sf::Font& font, size_t& selectedPostIndex) const {
        drawHeader(window, font);  // 테이블 헤더를 그립니다.

        const int startingY = 50;  // 처음 시작 Y 좌표
        const int lineHeight = 40; // 각 텍스트 줄의 높이

        // 모든 게시물을 순회하면서 그립니다.
        for (size_t i = 0; i < posts.size(); ++i) {
            float rowY = startingY + i * lineHeight;
            drawPost(window, font, posts[i], rowY, selectedPostIndex == i);
            drawRowSeparator(window, rowY + lineHeight - 5);  // 행 구분선 추가
        }
    }

    // 특정 게시물의 내용을 표시하는 메서드입니다.
    void drawContent(sf::RenderWindow& window, sf::Font& font, size_t postIndex) const {
        if (postIndex >= posts.size()) return;  // 범위 체크

        const Post& post = posts[postIndex];

        // 제목을 화면에 그립니다.
        sf::Text title(post.title, font, 30);
        title.setPosition(50, 50);
        title.setFillColor(sf::Color::Black);
        window.draw(title);

        // 내용 텍스트를 화면에 그립니다.
        sf::Text content(post.content, font, 20);
        content.setPosition(50, 100);
        content.setFillColor(sf::Color::Black);
        window.draw(content);

        // "뒤로가기" 버튼 그리기
        sf::RectangleShape backRect(sf::Vector2f(150, 30));
        backRect.setPosition(50, 500);
        backRect.setFillColor(sf::Color(220, 220, 220));
        window.draw(backRect);

        sf::Text backButton(L"뒤로가기", font, 20);
        backButton.setPosition(60, 505);
        backButton.setFillColor(sf::Color::Black);
        window.draw(backButton);
    }

    // 게시물의 개수를 반환하는 메서드입니다.
    size_t getPostCount() const {
        return posts.size();
    }

private:
    // 테이블의 헤더를 그리는 메서드입니다.
    void drawHeader(sf::RenderWindow& window, sf::Font& font) const {
        sf::Text title(L"제목", font, 20);
        title.setPosition(50, 10);  // "제목"의 시작 위치 설정
        title.setFillColor(sf::Color::Black);
        window.draw(title);

        sf::Text author(L"글쓴이", font, 20);
        author.setPosition(300, 10);  // "글쓴이"의 시작 위치 설정
        author.setFillColor(sf::Color::Black);
        window.draw(author);

        sf::Text views(L"조회수", font, 20);
        views.setPosition(500, 10);  // "조회수"의 시작 위치 설정
        views.setFillColor(sf::Color::Black);
        window.draw(views);

        sf::Text date(L"작성일", font, 20);
        date.setPosition(650, 10);  // "작성일"의 시작 위치 설정
        date.setFillColor(sf::Color::Black);
        window.draw(date);

        drawRowSeparator(window, 40);  // 헤더 하단에 행 구분선 추가
    }

    // 각 행 하단에 구분선을 그리는 메서드입니다.
    void drawRowSeparator(sf::RenderWindow& window, float y) const {
        sf::RectangleShape line(sf::Vector2f(window.getSize().x - 50, 1));  // 구분선 크기와 위치 설정
        line.setFillColor(sf::Color(200, 200, 200));  // 구분선의 색상 설정
        line.setPosition(50, y);
        window.draw(line);
    }

    // 각 열 사이에 세로 구분선을 그리는 메서드입니다.
    void drawColumnSeparator(sf::RenderWindow& window, float xStart, float yStart, float yEnd) const {
        sf::RectangleShape line(sf::Vector2f(1, yEnd - yStart));  // 구분선의 세로 길이 설정
        line.setFillColor(sf::Color(200, 200, 200));  // 구분선 색상 설정
        line.setPosition(xStart, yStart);
        window.draw(line);
    }

    // 각 게시물의 정보를 화면에 그리는 메서드입니다.
    void drawPost(sf::RenderWindow& window, sf::Font& font, const Post& post, float y, bool isSelected) const {
        sf::Text title(post.title, font, 20);  // 제목 텍스트 생성
        title.setPosition(50, y);  // 제목 위치 설정
        title.setFillColor(isSelected ? sf::Color::Red : sf::Color::Blue);  // 선택 여부에 따라 강조 표시
        window.draw(title);

        sf::Text author(post.author, font, 20);  // 글쓴이 텍스트 생성
        author.setPosition(300, y);
        author.setFillColor(sf::Color::Black);
        window.draw(author);

        sf::Text views(std::to_wstring(post.views), font, 20);  // 조회수 텍스트 생성
        views.setPosition(500, y);
        views.setFillColor(sf::Color::Black);
        window.draw(views);

        sf::Text date(post.date, font, 20);  // 작성일 텍스트 생성
        date.setPosition(650, y);
        date.setFillColor(sf::Color::Black);
        window.draw(date);

        drawColumnSeparator(window, 290, y, y + 35);  // 열 사이에 구분선 추가
        drawColumnSeparator(window, 490, y, y + 35);
        drawColumnSeparator(window, 640, y, y + 35);
    }

    std::vector<Post> posts;  // 게시물 리스트 저장
};

int main() {
    // SFML 윈도우 생성
    sf::RenderWindow window(sf::VideoMode(800, 600), L"게시판");
    window.setFramerateLimit(60);  // 프레임 속도 제한

    // 폰트 로드
    sf::Font font;
    if (!font.loadFromFile("HANBatang.ttf")) {
        std::cerr << "Error loading font\n";  // 폰트 로드 실패 시 오류 메시지 출력
        return -1;
    }

    BulletinBoard board;  // 게시판 객체 생성
    std::wstring currentInput;
    bool isEnteringText = false;
    bool skipFirstChar = false;
    size_t selectedPostIndex = std::numeric_limits<size_t>::max();  // 선택된 게시물 인덱스
    bool viewingContent = false;  // 현재 내용 보기 상태인지 표시

    // 예시 게시물 추가
    board.addPost(L"제목 예시1", L"작성자1", L"첫 번째 게시물 내용입니다.", 123, L"2023-09-15");
    board.addPost(L"제목 예시2", L"작성자2", L"두 번째 게시물 내용입니다.", 45, L"2023-09-16");
    board.addPost(L"제목 예시3", L"작성자3", L"세 번째 게시물 내용입니다.", 67, L"2023-09-17");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();  // 윈도우가 닫히면 프로그램 종료
            }

            // 현재 내용 보기 상태일 경우
            if (viewingContent) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    // "뒤로가기" 버튼을 클릭했는지 확인
                    if (event.mouseButton.x >= 50 && event.mouseButton.x <= 200 && event.mouseButton.y >= 500 && event.mouseButton.y <= 530) {
                        viewingContent = false;  // 목록 보기로 돌아가기
                    }
                }
            }
            else {
                // 목록 보기 상태일 경우
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    // 제목을 클릭하여 내용 보기로 전환
                    float yMouse = static_cast<float>(event.mouseButton.y);
                    float rowHeight = 40.0f;
                    float startingY = 50.0f;
                    for (size_t i = 0; i < board.getPostCount(); ++i) {
                        if (yMouse > startingY + i * rowHeight && yMouse < startingY + (i + 1) * rowHeight) {
                            selectedPostIndex = i;
                            viewingContent = true;
                            break;
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        if (viewingContent) {
            // 내용 보기 화면을 렌더링
            board.drawContent(window, font, selectedPostIndex);
        }
        else {
            // 게시물 목록 화면을 렌더링
            board.drawList(window, font, selectedPostIndex);
        }

        window.display();
    }

    return 0;
}
