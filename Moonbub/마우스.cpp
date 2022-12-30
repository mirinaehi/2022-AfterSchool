#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <SFML/Audio.hpp>

using namespace sf;

// obj1과 obj2의 충돌여부. 충돌하면 1을 반환, 충돌안하면 0을 반환
int is_collide(RectangleShape obj1, RectangleShape obj2)
{
	return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}

const int W_WIDTH = 1200, W_HEIGHT = 600;	// 창의 크기


int main(void)
{
	// 윈도창 생성
	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool");
	window.setFramerateLimit(60);

	srand(time(0));

	long start_time = clock();	// 게임 시작시간
	long spent_time;			// 게임 진행시간

	// text
	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(30);		// 글자크기 조절
	text.setFillColor(Color(255, 255, 255));
	text.setPosition(0, 0);
	char info[40];

	Vector2i mouse_pos;
	int click_cnt = 0;

	// 윈도가 열려있을 때까지 반복
	while (window.isOpen())
	{
		spent_time = clock() - start_time;
		mouse_pos = Mouse::getPosition(window);

		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();	
				break;

			case Event::KeyPressed:
				break;

			case Event::MouseButtonPressed:
				if(event.mouseButton.button == Mouse::Right)
					click_cnt++;
			}
		}

		if (Mouse::isButtonPressed(Mouse::Left))
			click_cnt++;

		sprintf(info, "(%3d, %3d) time:%d click:%d\n"
			, mouse_pos.x, mouse_pos.y, spent_time / 1000, click_cnt);
		text.setString(info);

		window.clear(Color::Black);
		window.draw(text);

		window.display();
	}

	return 0;
}