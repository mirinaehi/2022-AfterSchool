#include "Player.h"
#include "World.h"

// 생성자
Player::Player() {
    shape_.setSize(sf::Vector2f(50, 50)); // 플레이어 크기 설정
    shape_.setFillColor(sf::Color::Green); // 플레이어 색상 설정
    shape_.setPosition(100, kGroundY); // 초기 위치 설정
    speed_ = kPlayerSpeed; // 이동 속도 초기화
    is_jumping_ = false; // 점프 상태 초기화
    is_on_ground_ = true; // 초기에는 바닥에 있다고 가정
    jump_height_ = kJumpHeight; // 점프 높이 설정
    gravity_ = kGravity; // 중력 설정
    velocity_y_ = 0.0f; // Y축 속도 초기화
}

// 중력을 기반으로 플레이어를 업데이트
void Player::Update() {
    // 중력을 적용
    if (!is_on_ground_) {
        velocity_y_ += gravity_; // 중력을 속도에 적용
    }

    shape_.move(0, velocity_y_); // Y축으로 플레이어 이동

    // 바닥에 닿았는지 확인
    if (shape_.getPosition().y >= kGroundY) { // 바닥 Y 좌표
        shape_.setPosition(shape_.getPosition().x, kGroundY); // Y 위치를 바닥으로 설정
        velocity_y_ = 0; // Y축 속도 초기화
        setOnGround(true); // 바닥에 있음
        is_jumping_ = false; // 점프 상태 재설정
    }
}

// 주어진 방향으로 플레이어를 이동
void Player::Move(int direction) {
    shape_.move(direction * speed_, 0); // X축으로 이동
}

// 플레이어를 점프시키기
void Player::Jump() {
    if (is_on_ground_) { // 플랫폼 위에 있을 때만 점프 가능
        velocity_y_ = -jump_height_; // 점프 Y축 속도 설정
        is_jumping_ = true; // 점프 상태 설정
        setOnGround(false); // 점프 중이라 바닥에 없게 설정
    }
}

// 현재 플레이어 모양 반환
sf::RectangleShape& Player::getShape() {
    return shape_; // 현재 모양 반환
}

// Y 위치 반환
float Player::getPositionY() const {
    return shape_.getPosition().y; // 플레이어의 Y 위치 반환
}

// Y축 속도 반환
float Player::getVelocityY() const {
    return velocity_y_; // Y축 속도 반환
}

// 바닥에 있는지 여부 반환
bool Player::isOnGround() const {
    return is_on_ground_; // 바닥에 있는지 여부 반환
}

// is_on_ground_의 값을 설정하기 위한 세터
void Player::setOnGround(bool isOnGround) {
    is_on_ground_ = isOnGround; // 바닥 상태 설정
}

// velocity_y_의 값을 설정하기 위한 세터
void Player::setVelocityY(float velocityY) {
    velocity_y_ = velocityY; // Y축 속도 설정
}
