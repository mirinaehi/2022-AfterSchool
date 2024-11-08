#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <sstream> // 필요 라이브러리 추가
#include <iomanip> // 필요 라이브러리 추가
#include <vector> // 벡터 라이브러리 추가
#include "Player.h" // Player 헤더 파일 포함
#include "Platform.h" // Platform 헤더 파일 포함
#include "World.h"

// TODO : 뷰에대한 파일분할


const int kWidth = 800;
const int kHeight = 600;

const int kWidthMaxRaito = 4;
const int kHeightMaxRaito = 1;

const int kMiniMapWidth = kWidth * kWidthMaxRaito / 8;
const int kMiniMapHeight = kHeight * kHeightMaxRaito / 8;

const float kMiniMapX = kWidth - kMiniMapWidth;
const float kMinimapY = 10.f;

float view_x;
float view_y;

int main() {
    sf::RenderWindow window(sf::VideoMode(kWidth, kHeight), "Super Mario Jump Simulation"); // 윈도우 생성
    window.setFramerateLimit(60); // 프레임 속도 설정

    Player player; // 플레이어 객체 생성

    // 플랫폼 객체들을 생성
    std::vector<Platform> platforms;
    platforms.emplace_back(300, 20, sf::Color::Blue, 300, 400); // 첫 번째 플랫폼
    platforms.emplace_back(150, 20, sf::Color::Red, 500, 350); // 두 번째 플랫폼
    platforms.emplace_back(250, 20, sf::Color::Yellow, 700, 300); // 세 번째 플랫폼
    platforms.emplace_back(200, 20, sf::Color::Blue, 1000, 250); // 네 번째 플랫폼
    platforms.emplace_back(300, 20, sf::Color::Red, 900, 150); // 다섯 번째 플랫폼
    platforms.emplace_back(180, 20, sf::Color::Yellow, 1200, 400); // 여섯 번째 플랫폼
    platforms.emplace_back(400, 20, sf::Color::Blue, 1500, 420); // 일곱 번째 플랫폼
    platforms.emplace_back(320, 20, sf::Color::Red, 1800, 500); // 여덟 번째 플랫폼
    platforms.emplace_back(840, 20, sf::Color::Yellow, 2020, 470); // 아홉 번째 플랫폼
    platforms.emplace_back(500, 20, sf::Color::Blue, 800, 540); // 마지막 플랫폼

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
    sf::Text iog_text;
    iog_text.setFont(font); // 텍스트에 폰트 설정
    iog_text.setCharacterSize(24); // 글자 크기 설정
    iog_text.setFillColor(sf::Color::White); // 글자 색상 설정

    

    // 카메라 뷰 생성 및 설정
    sf::View camera = window.getDefaultView(); // 기본 뷰 가져오기
    camera.setSize(kWidth, kHeight); // 뷰의 크기 설정

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            player.Jump(); // 점프
        }

        // 플레이어 상태 업데이트
        player.Update();

        // 플랫폼과의 충돌 확인
        for (Platform& platform : platforms) {
            if (player.getShape().getGlobalBounds().intersects(platform.getShape().getGlobalBounds())) {
                // 플랫폼 위에 있을 때만 떨어지도록
                if (player.getVelocityY() > 0) { // 아래로 떨어질 때만
                    player.getShape().setPosition(player.getShape().getPosition().x, platform.getShape().getPosition().y - player.getShape().getSize().y);
                    player.setVelocityY(0); // Y축 속도 초기화
                    player.setOnGround(true); // 플레이어가 플랫폼 위에 있음을 설정
                    break;
                }
            }
            else {
                // 플랫폼을 떠났을 때 바닥에 있는지 체크
                player.setOnGround(player.getPositionY() >= kGroundY); // 바닥 Y좌표 체크
            }
        }

        // 카메라 위치 업데이트 (플레이어를 따라 이동)
        camera.setCenter(player.getShape().getPosition().x + player.getShape().getSize().x / 2,
            player.getShape().getPosition().y + player.getShape().getSize().y / 2); // 카메라 중앙을 플레이어 위치로 조정

        window.setView(camera); // 카메라 뷰 설정

        // 카메라 뷰에 의한 좌표 보정값
        view_x = camera.getCenter().x - kWidth / 2;
        view_y = camera.getCenter().y - kHeight / 2;

        // Y좌표 텍스트 업데이트
        y_text.setString("Y: " + std::to_string(static_cast<int>(player.getPositionY())));
        y_text.setPosition(view_x + 0.f, view_y + 0.f);   

        // Y 속도를 소수점 둘째 자리까지 형식화하여 텍스트에 설정
        std::ostringstream oss; // 텍스트 스트림 생성
        oss << std::fixed << std::setprecision(2) << player.isOnGround(); // 소수점 둘째 자리까지 설정
        iog_text.setString("is_on_ground: " + oss.str()); // 형식화된 문자열을 텍스트에 설정
        iog_text.setPosition(view_x + 0.f, view_y+24.f);

        // 미니맵 설정
        sf::RectangleShape miniMap(sf::Vector2f(kMiniMapWidth*kWidthMaxRaito, kMiniMapHeight));
        miniMap.setFillColor(sf::Color(0, 0, 0, 150)); // 반투명 배경 설정
        miniMap.setPosition(view_x+ kMiniMapX, view_y+ kMinimapY); // 미니맵 위치 설정

        

        sf::RectangleShape miniPlayer = player.getShape();
        miniPlayer.setScale(0.125f, 0.125f); // 미니맵에서 작게 표시
        miniPlayer.setPosition(view_x+ kMiniMapX + player.getShape().getPosition().x / 10, view_y+kMinimapY + player.getShape().getPosition().y / 10); // 미니맵 좌표 설정



        // 윈도우를 지우고 게임 객체를 그리기
        window.clear();
        for (Platform& platform : platforms) {
            window.draw(platform.getShape()); // 각각의 플랫폼 그리기
        }
        window.draw(player.getShape()); // 플레이어 그리기
        window.draw(y_text); // Y 좌표 텍스트 그리기
        window.draw(iog_text); // Y 속도 텍스트 그리기
        window.draw(miniMap);

        // TODO : 리팩토링 + 맨 오른쪽 노란색 플랫폼이 오른쪽 가장자리 부분이 미니맵과 매칭이 안됨
        // 플랫폼 및 플레이어를 미니맵에 그리기
        for (Platform& platform : platforms) {
            sf::RectangleShape miniPlatform = platform.getShape();
            miniPlatform.setScale(0.125f, 0.125f); // 미니맵에서 작게 표시
            miniPlatform.setPosition(view_x + kMiniMapX + platform.getShape().getPosition().x / 10, view_y + kMinimapY + platform.getShape().getPosition().y / 10); // 미니맵 좌표 설정
            window.draw(miniPlatform); // 미니맵에 플랫폼 그리기
        }
        window.draw(miniPlayer); // 미니맵에 플레이어 그리기


        window.display(); // 화면 업데이트
    }

    return 0; // 프로그램 정상 종료
}
