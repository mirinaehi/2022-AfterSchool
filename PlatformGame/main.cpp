#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <sstream> // 필요 라이브러리 추가
#include <iomanip> // 필요 라이브러리 추가
#include "Player.h" // Player 헤더 파일 포함
#include "Platform.h" // Platform 헤더 파일 포함

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Super Mario Jump Simulation"); // 윈도우 생성
    window.setFramerateLimit(60); // 프레임 속도 설정

    Player player; // 플레이어 객체 생성

    // 플랫폼 객체들을 생성
    Platform platform1(200, 20, sf::Color::Blue, 300, 350); // 첫 번째 플랫폼 생성
    Platform platform2(150, 20, sf::Color::Red, 500, 350 - 50); // 두 번째 플랫폼 생성
    Platform platform3(250, 20, sf::Color::Yellow, 100, 350 - 100); // 세 번째 플랫폼 생성

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

        // 각각의 플랫폼과의 충돌 확인
        if (player.getShape().getGlobalBounds().intersects(platform1.getShape().getGlobalBounds()) ||
            player.getShape().getGlobalBounds().intersects(platform2.getShape().getGlobalBounds()) ||
            player.getShape().getGlobalBounds().intersects(platform3.getShape().getGlobalBounds())) {

            // 플랫폼 위에 있을 때만 떨어지도록
            if (player.getVelocityY() > 0) { // 아래로 떨어질 때만
                if (player.getShape().getGlobalBounds().intersects(platform1.getShape().getGlobalBounds())) {
                    player.getShape().setPosition(player.getShape().getPosition().x, platform1.getShape().getPosition().y - player.getShape().getSize().y);
                }
                else if (player.getShape().getGlobalBounds().intersects(platform2.getShape().getGlobalBounds())) {
                    player.getShape().setPosition(player.getShape().getPosition().x, platform2.getShape().getPosition().y - player.getShape().getSize().y);
                }
                else if (player.getShape().getGlobalBounds().intersects(platform3.getShape().getGlobalBounds())) {
                    player.getShape().setPosition(player.getShape().getPosition().x, platform3.getShape().getPosition().y - player.getShape().getSize().y);
                }
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
        window.draw(platform1.getShape()); // 첫 번째 플랫폼 그리기
        window.draw(platform2.getShape()); // 두 번째 플랫폼 그리기
        window.draw(platform3.getShape()); // 세 번째 플랫폼 그리기
        window.draw(player.getShape()); // 플레이어 그리기
        window.draw(y_text); // Y 좌표 텍스트 그리기
        window.draw(vy_text); // Y 속도 텍스트 그리기
        window.display(); // 화면 업데이트
    }

    return 0; // 프로그램 정상 종료
}
