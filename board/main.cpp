#include "BulletinBoard.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), L"게시판");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("HANBatang.ttf")) {
        std::cerr << "Error loading font\n";
        return -1;
    }

    BulletinBoard board;
    std::wstring title_input;
    std::wstring content_input;
    enum PostSeq writing_state = kTitle;
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
                window.close(); // 창 닫기 처리
            }

            bool is_hovered = false;
            if (viewing_content) {
                is_hovered = false;
                window.setMouseCursor(arrow_cursor); // 내용 보기 시 기본 커서
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (event.mouseButton.x >= 50 && event.mouseButton.x <= 200 &&
                        event.mouseButton.y >= 500 && event.mouseButton.y <= 530) {
                        viewing_content = false; // 뒤로가기 버튼 처리
                    }
                }
            }
            else if (is_entering_text) {
                if (writing_state == kTitle) {
                    if (event.type == sf::Event::TextEntered) {
                        if (event.text.unicode == '\b') { // 백스페이스 처리
                            if (!title_input.empty()) {
                                title_input.pop_back();
                            }
                        }
                        else {
                            title_input += static_cast<wchar_t>(event.text.unicode);
                        }
                    }
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Key::Enter) {
                            if (!title_input.empty()) {
                                writing_state = kContent;
                            }
                        }
                    }
                }
                else if (writing_state == kContent) {
                    if (event.type == sf::Event::TextEntered) {
                        if (event.text.unicode == '\b') { // 백스페이스 처리
                            if (!content_input.empty()) {
                                content_input.pop_back();
                            }
                        }
                        else {
                            content_input += static_cast<wchar_t>(event.text.unicode);
                        }
                    }
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Key::Enter) {
                            if (!content_input.empty()) {
                                board.AddPost(title_input, L"새 작성자", content_input, 0, L"2023-10-01");
                                is_entering_text = false;
                                title_input.clear();
                                content_input.clear();
                                writing_state = kTitle;
                            }
                        }
                    }
                }
            }
            else {
                if (event.type == sf::Event::MouseMoved) {
                    float y_mouse = static_cast<float>(event.mouseMove.y);
                    float row_height = 40.0f;
                    float starting_y = 50.0f;
                    is_hovered = false;

                    // 마우스 호버 상태 확인
                    for (size_t i = 0; i < board.GetPostCount(); ++i) {
                        if (y_mouse > starting_y + i * row_height && y_mouse < starting_y + (i + 1) * row_height) {
                            hovered_post_index = i;
                            window.setMouseCursor(hand_cursor); // 호버 시 커서 변경
                            is_hovered = true;
                            break;
                        }
                    }

                    if (!is_hovered) {
                        window.setMouseCursor(arrow_cursor); // 기본 커서로 되돌림
                        hovered_post_index = std::numeric_limits<size_t>::max(); // 호버 상태 초기화
                    }
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (hovered_post_index != std::numeric_limits<size_t>::max()) {
                        board.SetSelected(hovered_post_index); // 선택된 포스트 처리
                        selected_post_index = hovered_post_index;
                        viewing_content = true;
                    }
                    else {
                        // 새로운 글 작성 버튼 클릭 감지
                        if (event.mouseButton.x >= 600 && event.mouseButton.x <= 750 &&
                            event.mouseButton.y >= 10 && event.mouseButton.y <= 40) {
                            is_entering_text = true;
                            title_input.clear();
                            content_input.clear();
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        if (viewing_content) {
            board.DrawContent(window, font, selected_post_index); // 선택된 포스트 내용 보여줌
        }
        else if (is_entering_text) {
            sf::Text title_label(L"제목: ", font, 20);
            title_label.setPosition(50, 100);
            title_label.setFillColor(sf::Color::Black);
            window.draw(title_label);

            sf::Text title_text(title_input, font, 20);
            title_text.setPosition(110, 100);
            title_text.setFillColor(sf::Color::Black);
            window.draw(title_text);

            sf::Text content_label(L"내용: ", font, 20);
            content_label.setPosition(50, 150);
            content_label.setFillColor(sf::Color::Black);
            window.draw(content_label);

            sf::Text content_text(content_input, font, 20);
            content_text.setPosition(110, 150);
            content_text.setFillColor(sf::Color::Black);
            window.draw(content_text);
        }
        else {
            board.DrawList(window, font, hovered_post_index); // 목록 보여줌

            // 새로운 글 작성 버튼 그리기
            sf::RectangleShape new_post_rect(sf::Vector2f(150, 30));
            new_post_rect.setPosition(600, 10);
            new_post_rect.setFillColor(sf::Color(220, 220, 220));
            window.draw(new_post_rect);

            sf::Text new_post_button(L"새 글", font, 20);
            new_post_button.setPosition(610, 15);
            new_post_button.setFillColor(sf::Color::Black);
            window.draw(new_post_button);
        }

        window.display();
    }
}
