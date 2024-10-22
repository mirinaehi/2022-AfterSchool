#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

// 게임의 플레이어를 나타내는 클래스
class Player {
public:
    sf::RectangleShape shape; // 플레이어의 모양
    float speed; // 플레이어의 이동 속도
    bool is_jumping; // 플레이어가 점프 중인지 여부
    float jump_height; // 플레이어의 점프 높이
    float gravity; // 중력의 힘
    float velocity_y; // 플레이어의 Y축 속도

    // 플레이어의 속성을 초기화하는 생성자
    Player() {
        shape.setSize(sf::Vector2f(50, 50)); // 플레이어 모양의 크기 설정
        shape.setFillColor(sf::Color::Green); // 플레이어 색상을 초록색으로 설정
        shape.setPosition(100, 300); // 초기 위치 설정
        speed = 5.0f; // 속도 초기화
        is_jumping = false; // 점프 상태를 초기화
        jump_height = 10.0f; // 점프 높이를 설정
        gravity = 0.5f; // 중력 값을 설정
        velocity_y = 0.0f; // Y축 속도를 초기화
    }

    // 중력을 기반으로 플레이어의 위치를 업데이트
    void Update() {
        if (shape.getPosition().y <= 300) {
            velocity_y += gravity; // 중력을 속도에 적용
            is_jumping = true; // 점프 상태 설정
        }
        else {
            velocity_y = 0; // 바닥에 떨어지면 속도 초기화
            is_jumping = false; // 점프 상태 초기화
        }
        shape.move(0, velocity_y); // Y축으로 플레이어 이동
    }

    // 주어진 방향으로 플레이어를 이동
    void Move(int direction) {
        shape.move(direction * speed, 0); // X축으로 플레이어 이동
    }

    // 플레이어를 점프시키기
    void Jump() {
        if (!is_jumping) { // 점프 중이 아닐 때만 점프
            velocity_y = -jump_height; // 점프를 위한 Y축 속도를 설정
        }
    }
};

// 게임을 실행하는 메인 함수
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Platform Game"); // 윈도우 생성
    window.setFramerateLimit(60); // 프레임 속도를 60FPS로 제한

    Player player; // 플레이어 객체 생성

    // 플랫폼 생성
    sf::RectangleShape platform(sf::Vector2f(200, 20));
    platform.setFillColor(sf::Color::Blue); // 플랫폼 색상을 파란색으로 설정
    platform.setPosition(300, 400); // 플랫폼 위치 설정

    // 텍스트를 표시할 폰트 로드
    sf::Font font;
    if (!font.loadFromFile("c:/windows/fonts/arial.ttf")) { // Arial 폰트 파일 로드 (경로는 실제 파일 경로로 변경해야 함)
        return -1; // 폰트 로드 실패 시 에러 처리
    }

    // Y 위치를 표시할 텍스트 객체 생성
    sf::Text y_text;
    y_text.setFont(font); // 텍스트에 폰트 설정
    y_text.setCharacterSize(24); // 글자 크기 설정
    y_text.setFillColor(sf::Color::White); // 글자 색상 설정

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
                window.close(); // 창이 닫히면 프로그램 종료
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
        if (player.shape.getGlobalBounds().intersects(platform.getGlobalBounds())) {
            player.shape.setPosition(player.shape.getPosition().x, platform.getPosition().y - player.shape.getSize().y);
            player.velocity_y = 0; // 충돌 시 Y축 속도 초기화
        }

        // Y좌표 텍스트 업데이트
        y_text.setString("Y: " + std::to_string(static_cast<int>(player.shape.getPosition().y)));
        vy_text.setString("vY: " + std::to_string(static_cast<int>(player.velocity_y)));

        // 윈도우를 지우고 게임 객체를 그리기
        window.clear();
        window.draw(platform);
        window.draw(player.shape);
        window.draw(y_text);
        window.draw(vy_text);
        window.display();
    }

    return 0; // 프로그램 정상 종료
}
