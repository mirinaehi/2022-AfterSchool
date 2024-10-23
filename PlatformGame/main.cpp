#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <sstream> // 필요 라이브러리 추가
#include <iomanip> // 필요 라이브러리 추가

// 상수 정의
const float kGroundY = 400.0f; // 바닥의 Y 좌표
const float kPlatformY = 350.0f; // 플랫폼의 Y 좌표
const float kJumpHeight = 10.0f; // 점프 높이
const float kGravity = 0.5f; // 중력의 값
const float kPlayerSpeed = 5.0f; // 플레이어 이동 속도

// 게임의 플레이어를 나타내는 클래스
class Player {
public:
    // 플레이어의 속성을 초기화하는 생성자
    Player() {
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
    void Update() {
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
    void Move(int direction) {
        shape_.move(direction * speed_, 0); // X축으로 이동
    }

    // 플레이어를 점프시키기
    void Jump() {
        if (is_on_ground_) { // 플랫폼 위에 있을 때만 점프 가능
            velocity_y_ = -jump_height_; // 점프 Y축 속도 설정
            is_jumping_ = true; // 점프 상태 설정
            setOnGround(false); // 점프 중이라 바닥에 없게 설정
        }
    }

    // 현재 플레이어 모양 반환
    sf::RectangleShape& getShape() {
        return shape_; // 현재 모양 반환
    }

    // Y 위치 반환
    float getPositionY() const {
        return shape_.getPosition().y; // 플레이어의 Y 위치 반환
    }

    // Y축 속도 반환
    float getVelocityY() const {
        return velocity_y_; // Y축 속도 반환
    }

    // 바닥에 있는지 여부 반환
    bool isOnGround() const {
        return is_on_ground_; // 바닥에 있는지 여부 반환
    }

    // is_on_ground_의 값을 설정하기 위한 세터
    void setOnGround(bool isOnGround) {
        is_on_ground_ = isOnGround; // 바닥 상태 설정
    }

    // velocity_y_의 값을 설정하기 위한 세터
    void setVelocityY(float velocityY) {
        velocity_y_ = velocityY; // Y축 속도 설정
    }
private:
    sf::RectangleShape shape_; // 플레이어의 모양
    float speed_;              // 이동 속도
    bool is_jumping_;          // 점프 중인지 여부
    bool is_on_ground_;        // 플랫폼 위에 있는지 여부
    float jump_height_;        // 점프 높이
    float gravity_;            // 중력의 힘
    float velocity_y_;         // Y축 속도
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Super Mario Jump Simulation"); // 윈도우 생성
    window.setFramerateLimit(60); // 프레임 속도 설정

    Player player; // 플레이어 객체 생성

    // 플랫폼 생성
    sf::RectangleShape platform(sf::Vector2f(200, 20));
    platform.setFillColor(sf::Color::Blue); // 플랫폼 색상 설정
    platform.setPosition(300, kPlatformY); // 플랫폼 위치 설정

    // 텍스트를 표시할 폰트 로드
    sf::Font font;
    if (!font.loadFromFile("c:/windows/fonts/arial.ttf")) {
        return -1; // 폰트 로드 실패 시 에러 처리
    }

    // Y 위치를 표시할 텍스트 객체 생성
    sf::Text y_text;
    y_text.setFont(font); // 텍스트에 폰트 설정
    y_text.setCharacterSize(24); // 글자 크기 설정
    y_text.setFillColor(sf::Color::White); // 글자 색상 설정

    // Y축 속도를 표시할 텍스트 객체 생성
    sf::Text vy_text;
    vy_text.setPosition(0.f, 24.f);
    vy_text.setFont(font); // 텍스트에 폰트 설정
    vy_text.setCharacterSize(24); // 글자 크기 설정
    vy_text.setFillColor(sf::Color::White); // 글자 색상 설정

    // 메인 게임 루프
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close(); // 창 닫기
            }
        }

        // 플레이어 입력 처리
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player.Move(-1); // 왼쪽으로 이동
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player.Move(1); // 오른쪽으로 이동
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            player.Jump(); // 점프
        }

        // 플레이어 상태 업데이트
        player.Update();

        // 플랫폼과의 충돌 확인
        if (player.getShape().getGlobalBounds().intersects(platform.getGlobalBounds())) {
            // 플랫폼 위에 있을 때만 떨어지도록
            if (player.getVelocityY() > 0) { // 아래로 떨어질 때만
                player.getShape().setPosition(player.getShape().getPosition().x, platform.getPosition().y - player.getShape().getSize().y);
                player.setVelocityY(0); // Y축 속도 초기화
                player.setOnGround(true); // 플레이어가 플랫폼 위에 있음을 설정
            }
        }
        else {
            // 플랫폼을 떠났을 때 바닥에 있는지 체크
            player.setOnGround(player.getPositionY() >= kGroundY); // 바닥 Y좌표 체크
        }

        // Y좌표 텍스트 업데이트
        y_text.setString("Y: " + std::to_string(static_cast<int>(player.getPositionY())));

        // Y 속도를 소수점 둘째 자리까지 형식화하여 텍스트에 설정
        std::ostringstream oss; // 텍스트 스트림 생성
        oss << std::fixed << std::setprecision(2) << player.getVelocityY(); // 소수점 둘째 자리까지 설정
        vy_text.setString("vY: " + oss.str()); // 형식화된 문자열을 텍스트에 설정

        // 윈도우를 지우고 게임 객체를 그리기
        window.clear();
        window.draw(platform); // 플랫폼 그리기
        window.draw(player.getShape()); // 플레이어 그리기
        window.draw(y_text); // Y 좌표 텍스트 그리기
        window.draw(vy_text); // Y 속도 텍스트 그리기
        window.display(); // 화면 업데이트
    }

    return 0; // 프로그램 정상 종료
}
