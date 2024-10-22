#ifndef BULLETIN_BOARD_H
#define BULLETIN_BOARD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// 포스트 구조체 정의: 각각의 게시물 데이터를 저장합니다.
struct Post {
    std::wstring title;   // 제목
    std::wstring author;  // 글쓴이
    std::wstring content; // 내용
    int views;            // 조회수
    std::wstring date;    // 작성일
    bool is_selected = false; // 게시물이 선택되었는지 여부
};

// 게시판 클래스를 정의합니다.
class BulletinBoard {
public:
    // 게시물을 추가하는 Overloaded 메소드
    void AddPost(const Post& post);

    void AddPost(const std::wstring& title, const std::wstring& author,
        const std::wstring& content, int views, const std::wstring& date);

    // 게시물 목록을 그립니다.
    void DrawList(sf::RenderWindow& window, sf::Font& font, size_t& hovered_post_index) const;

    // 선택된 포스트의 내용을 그립니다.
    void DrawContent(sf::RenderWindow& window, sf::Font& font, size_t post_index) const;

    void SetSelected(int idx, bool is_selected = true);

    size_t GetPostCount() const;

private:
    void DrawHeader(sf::RenderWindow& window, sf::Font& font) const;

    void DrawRowSeparator(sf::RenderWindow& window, float y) const;

    void DrawColumnSeparator(sf::RenderWindow& window, float x_start, float y_start, float y_end) const;

    void DrawPost(sf::RenderWindow& window, sf::Font& font, const Post& post, float y, bool is_selected,
        bool is_hovered) const;

    std::vector<Post> posts_; // 게시물 저장 벡터
};

#endif // BULLETIN_BOARD_H