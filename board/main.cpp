#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <limits>

// 게시물을 저장할 구조체 정의
struct Post {
    std::wstring title;
    std::wstring author;
    std::wstring content; // 추가된 내용 필드
    int views;
    std::wstring date;
};

// 게시판 클래스를 정의합니다.
class BulletinBoard {
public:
    void addPost(const std::wstring& title, const std::wstring& author, const std::wstring& content, int views, const std::wstring& date) {
        posts.push_back({ title, author, content, views, date });
    }

    // 게시물과 테이블을 그리는 메서드입니다.
    void draw(sf::RenderWindow& window, sf::Font& font, size_t& selectedPostIndex) const {
        drawHeader(window, font);

        const int startingY = 50;
        const int lineHeight = 40;

        for (size_t i = 0; i < posts.size(); ++i) {
            float rowY = startingY + i * lineHeight;
            drawPost(window, font, posts[i], rowY, selectedPostIndex == i);

            if (selectedPostIndex == i) {
                drawContent(window, font, posts[i].content, rowY + 35);
            }
        }
    }

    size_t getPostCount() const {
        return posts.size();
    }

private:
    void drawHeader(sf::RenderWindow& window, sf::Font& font) const {
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

    void drawPost(sf::RenderWindow& window, sf::Font& font, const Post& post, float y, bool isSelected) const {
        sf::Text title(post.title, font, 20);
        title.setPosition(50, y);
        title.setFillColor(isSelected ? sf::Color::Red : sf::Color::Blue);
        window.draw(title);

        sf::Text author(post.author, font, 20);
        author.setPosition(300, y);
        author.setFillColor(sf::Color::Black);
        window.draw(author);

        sf::Text views(std::to_wstring(post.views), font, 20);
        views.setPosition(500, y);
        views.setFillColor(sf::Color::Black);
        window.draw(views);

        sf::Text date(post.date, font, 20);
        date.setPosition(650, y);
        date.setFillColor(sf::Color::Black);
        window.draw(date);
    }

    void drawContent(sf::RenderWindow& window, sf::Font& font, const std::wstring& content, float y) const {
        sf::Text contentText(content, font, 20);
        contentText.setPosition(50, y);
        contentText.setFillColor(sf::Color::Black);
        window.draw(contentText);
    }

    std::vector<Post> posts;  // 게시물 리스트 저장
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), L"게시판");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("HANBatang.ttf")) {
        std::cerr << "Error loading font\n";
        return -1;
    }

    BulletinBoard board;
    std::wstring currentInput;
    bool isEnteringText = false;
    bool skipFirstChar = false;
    size_t selectedPostIndex = std::numeric_limits<size_t>::max(); // 선택된 게시물

    // 예시 게시물 추가
    board.addPost(L"제목 예시1", L"작성자1", L"첫 번째 게시물 내용입니다.", 123, L"2023-09-15");
    board.addPost(L"제목 예시2", L"작성자2", L"두 번째 게시물 내용입니다.", 45, L"2023-09-16");
    board.addPost(L"제목 예시3", L"작성자3", L"세 번째 게시물 내용입니다.", 67, L"2023-09-17");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                float yMouse = static_cast<float>(event.mouseButton.y);
                float rowHeight = 40.0f;  // 줄 높이
                float startingY = 50.0f;  // 시작 Y 좌표
                for (size_t i = 0; i < board.getPostCount(); ++i) {
                    if (yMouse > startingY + i * rowHeight && yMouse < startingY + (i + 1) * rowHeight) {
                        selectedPostIndex = i;  // 선택된 게시물 활성화
                        break;
                    }
                }
            }
        }

        window.clear(sf::Color::White);
        board.draw(window, font, selectedPostIndex);
        window.display();
    }

    return 0;
}
