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
    bool is_selected = false;
};

class BulletinBoard {
public:
    void AddPost(const Post& post) {
        posts_.push_back(post);
    }
    void AddPost(const std::wstring& title, const std::wstring& author, const std::wstring& content, int views, const std::wstring& date) {
        posts_.push_back({ title, author, content, views, date });
    }

    void DrawList(sf::RenderWindow& window, sf::Font& font, size_t& hovered_post_index) const {
        DrawHeader(window, font);

        const int starting_y = 50;
        const int line_height = 40;

        for (size_t i = 0; i < posts_.size(); ++i) {
            float row_y = starting_y + i * line_height;
            DrawPost(window, font, posts_[i], row_y, (posts_[i].is_selected), (hovered_post_index == i));
            DrawRowSeparator(window, row_y + line_height - 5);
        }
    }

    void DrawContent(sf::RenderWindow& window, sf::Font& font, size_t post_index) const {
        if (post_index >= posts_.size()) return;

        const Post& post = posts_[post_index];
        sf::Text title(post.title, font, 30);
        title.setPosition(50, 50);
        title.setFillColor(sf::Color::Black);
        window.draw(title);

        sf::Text content(post.content, font, 20);
        content.setPosition(50, 100);
        content.setFillColor(sf::Color::Black);
        window.draw(content);

        sf::RectangleShape back_rect(sf::Vector2f(150, 30));
        back_rect.setPosition(50, 500);
        back_rect.setFillColor(sf::Color(220, 220, 220));
        window.draw(back_rect);

        sf::Text back_button(L"뒤로가기", font, 20);
        back_button.setPosition(60, 505);
        back_button.setFillColor(sf::Color::Black);
        window.draw(back_button);
    }

    void SetSelected(int idx, bool b = true) { posts_[idx].is_selected = b; }
    size_t GetPostCount() const { return posts_.size(); }

private:
    void DrawHeader(sf::RenderWindow& window, sf::Font& font) const {
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

        DrawRowSeparator(window, 40);
    }

    void DrawRowSeparator(sf::RenderWindow& window, float y) const {
        sf::RectangleShape line(sf::Vector2f(window.getSize().x - 50, 1));
        line.setFillColor(sf::Color(200, 200, 200));
        line.setPosition(50, y);
        window.draw(line);
    }

    void DrawColumnSeparator(sf::RenderWindow& window, float x_start, float y_start, float y_end) const {
        sf::RectangleShape line(sf::Vector2f(1, y_end - y_start));
        line.setFillColor(sf::Color(200, 200, 200));
        line.setPosition(x_start, y_start);
        window.draw(line);
    }

    void DrawPost(sf::RenderWindow& window, sf::Font& font, const Post& post, float y, bool is_selected,
        bool is_hovered) const {
        sf::Text title(post.title, font, 20);
        title.setPosition(50, y);
        title.setFillColor(is_hovered ? sf::Color(255, 105, 180)
            : is_selected ? sf::Color::Red : sf::Color::Blue);
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

        DrawColumnSeparator(window, 290, y, y + 35);
        DrawColumnSeparator(window, 490, y, y + 35);
        DrawColumnSeparator(window, 640, y, y + 35);
    }

    std::vector<Post> posts_;
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
    std::wstring current_input;
    bool is_entering_text = false;
    bool skip_first_char = false;
    size_t selected_post_index = std::numeric_limits<size_t>::max();
    size_t hovered_post_index = std::numeric_limits<size_t>::max();
    bool viewing_content = false;

    // 예시 게시물 추가
    board.AddPost(L"제목 예시1", L"작성자1", L"첫 번째 게시물 내용입니다.", 123, L"2023-09-15");
    board.AddPost(L"제목 예시2", L"작성자2", L"두 번째 게시물 내용입니다.", 45, L"2023-09-16");
    board.AddPost(L"제목 예시3", L"작성자3", L"세 번째 게시물 내용입니다.", 67, L"2023-09-17");

    sf::Cursor hand_cursor;
    if (!hand_cursor.loadFromSystem(sf::Cursor::Hand)) {
        std::cerr << "Error loading hand_cursor\n";
    }
    sf::Cursor arrow_cursor;
    if (!arrow_cursor.loadFromSystem(sf::Cursor::Arrow)) {
        std::cerr << "Error loading arrow_cursor\n";
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            bool is_hovered = false;
            if (viewing_content) {
                is_hovered = false;
                window.setMouseCursor(arrow_cursor);
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (event.mouseButton.x >= 50 && event.mouseButton.x <= 200 &&
                        event.mouseButton.y >= 500 && event.mouseButton.y <= 530) {
                        viewing_content = false;
                    }
                }
            }
            else {
                if (event.type == sf::Event::MouseMoved) {
                    float y_mouse = static_cast<float>(event.mouseMove.y);
                    float row_height = 40.0f;
                    float starting_y = 50.0f;
                    is_hovered = false;

                    for (size_t i = 0; i < board.GetPostCount(); ++i) {
                        if (y_mouse > starting_y + i * row_height && y_mouse < starting_y + (i + 1) * row_height) {
                            hovered_post_index = i;
                            window.setMouseCursor(hand_cursor);
                            is_hovered = true;
                            break;
                        }
                    }

                    if (!is_hovered) {
                        window.setMouseCursor(arrow_cursor);
                        hovered_post_index = std::numeric_limits<size_t>::max();
                    }
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (hovered_post_index != std::numeric_limits<size_t>::max()) {
                        board.SetSelected(hovered_post_index);
                        selected_post_index = hovered_post_index;
                        viewing_content = true;
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        if (viewing_content) {
            board.DrawContent(window, font, selected_post_index);
        }
        else {
            board.DrawList(window, font, hovered_post_index);
        }

        window.display();
    }
}
