#include "BulletinBoard.h"

// 게시물을 추가하는 Overloaded 메소드

void BulletinBoard::AddPost(const Post& post) {
    posts_.push_back(post); // 포스트 객체를 추가합니다.
}

void BulletinBoard::AddPost(const std::wstring& title, const std::wstring& author, const std::wstring& content, int views, const std::wstring& date) {
    posts_.push_back({ title, author, content, views, date }); // 제목, 글쓴이, 내용을 입력해 포스트 객체를 추가합니다.
}

// 게시물 목록을 그립니다.

void BulletinBoard::DrawList(sf::RenderWindow& window, sf::Font& font, size_t& hovered_post_index) const {
    DrawHeader(window, font); // 테이블 헤더를 그립니다.

    const int starting_y = 50; // 시작 y 위치
    const int line_height = 40; // 각 항목의 높이

    // 모든 게시물을 한 줄씩 그립니다.
    for (size_t i = 0; i < posts_.size(); ++i) {
        float row_y = starting_y + i * line_height;
        DrawPost(window, font, posts_[i], row_y, (posts_[i].is_selected), (hovered_post_index == i));
        DrawRowSeparator(window, row_y + line_height - 5); // 행 구분선을 추가합니다.
    }
}

// 선택된 포스트의 내용을 그립니다.

void BulletinBoard::DrawContent(sf::RenderWindow& window, sf::Font& font, size_t post_index) const {
    if (post_index >= posts_.size()) return; // 인덱스가 범위를 벗어나면 리턴

    const Post& post = posts_[post_index]; // 선택된 포스트
    sf::Text title(post.title, font, 30); // 포스트 제목
    title.setPosition(50, 50);
    title.setFillColor(sf::Color::Black);
    window.draw(title);

    sf::Text content(post.content, font, 20); // 포스트 내용
    content.setPosition(50, 100);
    content.setFillColor(sf::Color::Black);
    window.draw(content);

    // "뒤로가기" 버튼을 추가합니다.
    sf::RectangleShape back_rect(sf::Vector2f(150, 30));
    back_rect.setPosition(50, 500);
    back_rect.setFillColor(sf::Color(220, 220, 220));
    window.draw(back_rect);

    sf::Text back_button(L"뒤로가기", font, 20);
    back_button.setPosition(60, 505);
    back_button.setFillColor(sf::Color::Black);
    window.draw(back_button);
}

void BulletinBoard::SetSelected(int idx, bool is_selected) {
    posts_[idx].is_selected = is_selected; // 주어진 인덱스의 게시물 선택 상태를 설정합니다.
}

size_t BulletinBoard::GetPostCount() const {
    return posts_.size(); // 게시물의 개수를 반환합니다.
}

void BulletinBoard::DrawHeader(sf::RenderWindow& window, sf::Font& font) const {
    // 열 제목을 설정하고 그리기
    sf::Text title(L"제목", font, 20);
    title.setPosition(50, 10);
    title.setFillColor(sf::Color::Black);
    window.draw(title);

    sf::Text author(L"글쓴이", font, 20);
    author.setPosition(300, 10);
    author.setFillColor(sf::Color::Black);
    window.draw(author);

    sf::Text views(L"조회수", font, 20);
    views.setPosition(500, 10);
    views.setFillColor(sf::Color::Black);
    window.draw(views);

    sf::Text date(L"작성일", font, 20);
    date.setPosition(650, 10);
    date.setFillColor(sf::Color::Black);
    window.draw(date);

    DrawRowSeparator(window, 40); // 헤더 아래에 구분선 추가
}

void BulletinBoard::DrawRowSeparator(sf::RenderWindow& window, float y) const {
    // 행 하단에 구분선을 그립니다.
    sf::RectangleShape line(sf::Vector2f(window.getSize().x - 50, 1));
    line.setFillColor(sf::Color(200, 200, 200));
    line.setPosition(50, y);
    window.draw(line);
}

void BulletinBoard::DrawColumnSeparator(sf::RenderWindow& window, float x_start, float y_start, float y_end) const {
    // 각 열 사이에 구분선을 그립니다.
    sf::RectangleShape line(sf::Vector2f(1, y_end - y_start));
    line.setFillColor(sf::Color(200, 200, 200));
    line.setPosition(x_start, y_start);
    window.draw(line);
}

void BulletinBoard::DrawPost(sf::RenderWindow& window, sf::Font& font, const Post& post, float y, bool is_selected, bool is_hovered) const {
    // 각 포스트의 정보를 그립니다.
    sf::Text title(post.title, font, 20);
    title.setPosition(50, y);
    // 선택된 항목은 빨간색, 호버된 항목은 핑크색, 기본은 파란색
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

    // 열 구분선 그리기
    DrawColumnSeparator(window, 290, y, y + 35);
    DrawColumnSeparator(window, 490, y, y + 35);
    DrawColumnSeparator(window, 640, y, y + 35);
}
