#ifndef MISSILE_H
#define MISSILE_H

#include <SFML/Graphics.hpp>
#include "World.h"

class Missile {
private:
    sf::RectangleShape shape_; // 미사일의 모양
    float velocity_x_;         // X축 속도
    float velocity_y_;         // Y축 속도

public:
    // 미사일 생성자
    Missile(float startX, float startY, float angle) {
        shape_.setSize(sf::Vector2f(10, 5)); // 미사일 크기 설정
        shape_.setFillColor(sf::Color::Red); // 미사일 색상 설정
        shape_.setPosition(startX, startY); // 시작 위치 설정

        // 각도를 라디안으로 변환하여 속도 계산
        float speed = 10.0f; // 초기 속도
        velocity_x_ = speed * cos(angle); // 초기 X축 속도
        velocity_y_ = speed * sin(angle); // 초기 Y축 속도
    }

    // 미사일 업데이트 함수
    void Update() {
        // 중력을 적용
        velocity_y_ += kGravity; // Y속도에 중력 추가
        shape_.move(velocity_x_, velocity_y_); // 속도에 따라 미사일 이동
    }

    // 미사일의 모양을 반환
    const sf::RectangleShape& getShape() const {
        return shape_;
    }
};

#endif // MISSILE_H
