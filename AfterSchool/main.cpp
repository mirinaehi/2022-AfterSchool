#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;

int main(void)
{
	// ����â ����
	RenderWindow window(VideoMode(640, 480), "AfterSchool");
	window.setFramerateLimit(60);

	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100);
	player.setFillColor(Color::Red);
	

	// ������ �������� ������ �ݺ�
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				// ����(x) ��ư�� ������ Event::Closed(0)
			case Event::Closed:
				window.close();		// ������ �ݴ´�
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			player.move(-1, 0);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.move(1, 0);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			player.move(0, -1);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			player.move(0, 1);
		}

		window.clear(Color::Black);

		window.draw(player);

		window.display();
	}

	return 0;
}