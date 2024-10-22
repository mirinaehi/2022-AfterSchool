#include "BulletinBoard.h"
#include <SFML/Graphics.hpp>
#include <iostream>


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), L"게시판"); // 윈도우 생성
    window.setFramerateLimit(60); // 프레임 제한 설정
    
    sf::Font font;
    if (!font.loadFromFile("HANBatang.ttf")) {
        std::cerr << "Error loading font\n";
        return -1;
    }

    BulletinBoard board;
    std::wstring current_input;
    bool is_entering_text = false;
    bool skip_first_char = false;
    size_t selected_post_index = std::numeric_limits<size_t>::max(); // 선택된 포스트의 인덱스 초기화
    size_t hovered_post_index = std::numeric_limits<size_t>::max(); // 마우스 호버 인덱스 초기화
    bool viewing_content = false; // 내용 보기 상태인지 여부

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
                }
            }
        }

        window.clear(sf::Color::White);

        if (viewing_content) {
            board.DrawContent(window, font, selected_post_index); // 선택된 포스트 내용 보여줌
        }
        else {
            board.DrawList(window, font, hovered_post_index); // 목록 보여줌
        }

        window.display();
    }
}
