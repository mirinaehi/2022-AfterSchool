#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

// 상수 정의
const float kGroundY = 500.0f; // 바닥의 Y 좌표
const float kJumpHeight = 10.0f; // 점프 높이
const float kGravity = 0.5f; // 중력의 값
const float kPlayerSpeed = 5.0f; // 플레이어 이동 속도

// 게임의 플레이어를 나타내는 클래스
class Player {
private:
    sf::RectangleShape shape_; // 플레이어의 모양
    float speed_;              // 이동 속도
    bool is_jumping_;          // 점프 중인지 여부
    bool is_on_ground_;        // 플랫폼 위에 있는지 여부
    float jump_height_;        // 점프 높이
    float gravity_;            // 중력의 힘
    float velocity_y_;         // Y축 속도

public:
    // 생성자
    Player();

    // 중력을 기반으로 플레이어를 업데이트
    void Update();

    // 주어진 방향으로 플레이어를 이동
    void Move(int direction);

    // 플레이어를 점프시키기
    void Jump();

    // 현재 플레이어 모양 반환
    sf::RectangleShape& getShape();

    // Y 위치 반환
    float getPositionY() const;

    // Y축 속도 반환
    float getVelocityY() const;

    // 바닥에 있는지 여부 반환
    bool isOnGround() const;

    // is_on_ground_의 값을 설정하기 위한 세터
    void setOnGround(bool isOnGround);

    // velocity_y_의 값을 설정하기 위한 세터
    void setVelocityY(float velocityY);
};

#endif // PLAYER_H
