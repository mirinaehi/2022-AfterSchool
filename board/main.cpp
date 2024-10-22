#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <limits>

struct Post {
    std::wstring title;
    std::wstring author;
    std::wstring content;
    int views;
    std::wstring date;
};

class BulletinBoard {
public:
    void addPost(const std::wstring& title, const std::wstring& author, const std::wstring& content, int views, const std::wstring& date) {
        posts.push_back({ title, author, content, views, date });
    }

    void drawList(sf::RenderWindow& window, sf::Font& font, size_t& hoveredPostIndex, size_t& selectedPostIndex) const {
        drawHeader(window, font);

        const int startingY = 50;
        const int lineHeight = 40;

        for (size_t i = 0; i < posts.size(); ++i) {
            float rowY = startingY + i * lineHeight;
            drawPost(window, font, posts[i], rowY, (selectedPostIndex == i), (hoveredPostIndex == i));
            drawRowSeparator(window, rowY + lineHeight - 5);
        }
    }

    void drawContent(sf::RenderWindow& window, sf::Font& font, size_t postIndex) const {
        if (postIndex >= posts.size()) return;

        const Post& post = posts[postIndex];
        sf::Text title(post.title, font, 30);
        title.setPosition(50, 50);
        title.setFillColor(sf::Color::Black);
        window.draw(title);

        sf::Text content(post.content, font, 20);
        content.setPosition(50, 100);
        content.setFillColor(sf::Color::Black);
        window.draw(content);

        sf::RectangleShape backRect(sf::Vector2f(150, 30));
        backRect.setPosition(50, 500);
        backRect.setFillColor(sf::Color(220, 220, 220));
        window.draw(backRect);

        sf::Text backButton(L"뒤로가기", font, 20);
        backButton.setPosition(60, 505);
        backButton.setFillColor(sf::Color::Black);
        window.draw(backButton);
    }

    size_t getPostCount() const { return posts.size(); }

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

        drawRowSeparator(window, 40);
    }

    void drawRowSeparator(sf::RenderWindow& window, float y) const {
        sf::RectangleShape line(sf::Vector2f(window.getSize().x - 50, 1));
        line.setFillColor(sf::Color(200, 200, 200));
        line.setPosition(50, y);
        window.draw(line);
    }

    void drawColumnSeparator(sf::RenderWindow& window, float xStart, float yStart, float yEnd) const {
        sf::RectangleShape line(sf::Vector2f(1, yEnd - yStart));
        line.setFillColor(sf::Color(200, 200, 200));
        line.setPosition(xStart, yStart);
        window.draw(line);
    }

    void drawPost(sf::RenderWindow& window, sf::Font& font, const Post& post, float y, bool isSelected,
        bool isHovered) const {
        sf::Text title(post.title, font, 20);
        title.setPosition(50, y);
        // 선택된 항목은 빨간색, 호버된 항목은 핑크색, 기본은 파란색
        title.setFillColor(isHovered ? sf::Color(255, 105, 180)
            : isSelected ? sf::Color::Red : sf::Color::Blue);
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

        drawColumnSeparator(window, 290, y, y + 35);
        drawColumnSeparator(window, 490, y, y + 35);
        drawColumnSeparator(window, 640, y, y + 35);
    }

    std::vector<Post> posts;
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
    size_t selectedPostIndex = std::numeric_limits<size_t>::max();
    size_t hoveredPostIndex = std::numeric_limits<size_t>::max();
    bool viewingContent = false;

    // 예시 게시물 추가
    board.addPost(L"제목 예시1", L"작성자1", L"첫 번째 게시물 내용입니다.", 123, L"2023-09-15");
    board.addPost(L"제목 예시2", L"작성자2", L"두 번째 게시물 내용입니다.", 45, L"2023-09-16");
    board.addPost(L"제목 예시3", L"작성자3", L"세 번째 게시물 내용입니다.", 67, L"2023-09-17");

    sf::Cursor hand_cursor;  // 커서를 관리할 객체
    if (!hand_cursor.loadFromSystem(sf::Cursor::Hand)) {
        std::cerr << "Error loading hand_cursor\n";
    }
    sf::Cursor arrow_cursor;  // 커서를 관리할 객체
    if (!arrow_cursor.loadFromSystem(sf::Cursor::Arrow)) {
        std::cerr << "Error loading arrow_cursor\n";
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            bool isHovered = false;  // 호버 상태 추적
            if (viewingContent) {
                isHovered = false;
                window.setMouseCursor(arrow_cursor);
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (event.mouseButton.x >= 50 && event.mouseButton.x <= 200 &&
                        event.mouseButton.y >= 500 && event.mouseButton.y <= 530) {
                        viewingContent = false;
                    }
                }
            }
            else {
                if (event.type == sf::Event::MouseMoved) {
                    float yMouse = static_cast<float>(event.mouseMove.y);
                    float rowHeight = 40.0f;
                    float startingY = 50.0f;
                    isHovered = false;

                    for (size_t i = 0; i < board.getPostCount(); ++i) {
                        if (yMouse > startingY + i * rowHeight && yMouse < startingY + (i + 1) * rowHeight) {
                            hoveredPostIndex = i;
                            window.setMouseCursor(hand_cursor);  // 호버 시 커서 변경
                            isHovered = true;
                            break;
                        }
                    }

                    if (!isHovered) {
                        window.setMouseCursor(arrow_cursor);  // 호버가 아니면 기본 커서로 변경
                        hoveredPostIndex = std::numeric_limits<size_t>::max();  // 호버 상태 초기화
                    }
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (hoveredPostIndex != std::numeric_limits<size_t>::max()) {
                        selectedPostIndex = hoveredPostIndex;  // 호버된 상태에서 선택된 상태로 전환합니다.
                        viewingContent = true;
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        if (viewingContent) {
            board.drawContent(window, font, selectedPostIndex);
        }
        else {
            board.drawList(window, font, hoveredPostIndex, selectedPostIndex);
        }

        window.display();
    }

    return 0;
}
