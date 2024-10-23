#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>

// 플랫폼을 나타내는 클래스
class Platform {
private:
    sf::RectangleShape shape_; // 플랫폼의 모양

public:
    // 플랫폼의 속성을 초기화하는 생성자
    Platform(float width, float height, const sf::Color& color, float posX, float posY) {
        shape_.setSize(sf::Vector2f(width, height)); // 플랫폼 크기 설정
        shape_.setFillColor(color); // 플랫폼 색상 설정
        shape_.setPosition(posX, posY); // 초기 위치 설정
    }

    // 플랫폼의 모양 반환
    sf::RectangleShape& getShape() {
        return shape_; // 현재 모양 반환
    }
};

#endif // PLATFORM_H
