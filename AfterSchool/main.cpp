#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <SFML/Audio.hpp>

using namespace sf;

struct Player {
	RectangleShape sprite;
	int speed;
	int score;
	int life;
};

struct Enemy {
	RectangleShape sprite;
	int speed;
	int life;
	int score;
	SoundBuffer explosion_buffer;
	Sound explosion_sound;
};

// 전역변수
const int ENEMY_NUM = 10;					// enemy의 최대개수
const int W_WIDTH = 640, W_HEIGHT = 480;	// 창의 크기
const int GO_WIDTH = 320, GO_HEIGHT = 240;	// 게임오버 그림의 크기


int main(void)
{
	// 윈도창 생성
	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool");
	window.setFramerateLimit(60);

	srand(time(0));

	long start_time = clock();	// 게임 시작시간
	long spent_time;			// 게임 진행시간
	int is_gameover = 0;

	// text
	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	
	Text text;
	text.setFont(font);
	text.setCharacterSize(30);		// 글자크기 조절
	text.setFillColor(Color(255, 255, 255));
	text.setPosition(0, 0);
	char info[40];

	// 배경
	Texture bg_texture;
	bg_texture.loadFromFile("./resources/images/background.jpg");
	Sprite bg_sprite;
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setPosition(0, 0);

	// gameover
	Texture gameover_texture;
	gameover_texture.loadFromFile("./resources/images/gameover.png");
	Sprite gameover_sprite;
	gameover_sprite.setTexture(gameover_texture);
	gameover_sprite.setPosition((W_WIDTH-GO_WIDTH)/2, (W_HEIGHT-GO_HEIGHT)/2);

	// 플레이어
	struct Player player;
	player.sprite.setSize(Vector2f(40, 40));
	player.sprite.setPosition(100, 100);
	player.sprite.setFillColor(Color::Red);
	player.speed = 5;
	player.score = 0;
	player.life = 3;

	// 적(enemy)
	struct Enemy enemy[ENEMY_NUM];
	
	// enemy 초기화
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		// TODO : 굉장히 비효율적인 코드이므로 나중에 refactoring
		enemy[i].explosion_buffer.loadFromFile("./resources/sounds/rumble.flac");
		enemy[i].explosion_sound.setBuffer(enemy[i].explosion_buffer);
		enemy[i].score = 100;

		enemy[i].sprite.setSize(Vector2f(70, 70));
		enemy[i].sprite.setFillColor(Color::Yellow);
		enemy[i].sprite.setPosition(rand()%300+300, rand()%380);
		enemy[i].life = 1;
		enemy[i].speed = -(rand() % 10 + 1);
	}
	

	// 윈도가 열려있을 때까지 반복
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				// 종료(x) 버튼을 누르면 Event::Closed(0)
			case Event::Closed:
				window.close();		// 윈도를 닫는다
				break;
				// 키보드를 눌렀을 때(누른 순간만을 감지)
			case Event::KeyPressed:
			{
				// 스페이스 키 누르면 모든 enemy 다시 출현
				if (event.key.code == Keyboard::Space)
				{
					for (int i = 0; i < ENEMY_NUM; i++)
					{
						enemy[i].sprite.setSize(Vector2f(70, 70));
						enemy[i].sprite.setFillColor(Color::Yellow);
						enemy[i].sprite.setPosition(rand() % 300 + 300, rand() % 380);
						enemy[i].life = 1;
						enemy[i].speed = -(rand() % 10 + 1);
					}
				}
				break;
			}

			}
		}

		spent_time = clock() - start_time;

		// 방향키 start
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			player.sprite.move(-player.speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.sprite.move(player.speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			player.sprite.move(0, -player.speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			player.sprite.move(0, player.speed);
		}	// 방향키 end


		
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (enemy[i].life > 0)
			{
				// enemy와의 충돌
				if (player.sprite.getGlobalBounds().intersects(enemy[i].sprite.getGlobalBounds()))
				{
					printf("enemy[%d]과 충돌\n", i);
					enemy[i].life -= 1;
					player.score += enemy[i].score;
					
					// TODO : 코드 refactoring 필요
					if (enemy[i].life == 0)
					{
						enemy[i].explosion_sound.play();
					}
				}
				// 적이 왼쪽 끝에 진입하려는 순간
				else if (enemy[i].sprite.getPosition().x < 0)
				{
					player.life -= 1;
					enemy[i].life = 0;
				}

				enemy[i].sprite.move(enemy[i].speed, 0);
			}
		}
		
		if (player.life <= 0)
		{
			is_gameover = 1;
		}
		

		sprintf(info, "life:%d score:%d time:%d"
			, player.life, player.score, spent_time/1000);
		text.setString(info);

		window.clear(Color::Black);
		window.draw(bg_sprite);

		// draw는 나중에 호출할수록 우선순위가 높아짐
		for(int i=0; i<ENEMY_NUM; i++)
			if (enemy[i].life > 0)
				window.draw(enemy[i].sprite); 
		window.draw(player.sprite);
		window.draw(text);
		
		if (is_gameover)
		{
			window.draw(gameover_sprite);
		}

		window.display();
	}

	return 0;
}