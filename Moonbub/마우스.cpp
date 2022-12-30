#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <SFML/Audio.hpp>

using namespace sf;

// obj1�� obj2�� �浹����. �浹�ϸ� 1�� ��ȯ, �浹���ϸ� 0�� ��ȯ
int is_collide(RectangleShape obj1, RectangleShape obj2)
{
	return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}

const int W_WIDTH = 1200, W_HEIGHT = 600;	// â�� ũ��


int main(void)
{
	// ����â ����
	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool");
	window.setFramerateLimit(60);

	srand(time(0));

	long start_time = clock();	// ���� ���۽ð�
	long spent_time;			// ���� ����ð�

	// text
	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(30);		// ����ũ�� ����
	text.setFillColor(Color(255, 255, 255));
	text.setPosition(0, 0);
	char info[40];

	Vector2i mouse_pos;
	int click_cnt = 0;

	// ������ �������� ������ �ݺ�
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