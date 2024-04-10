#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Game Start Text");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return 1;
    }

    sf::Text text("Game Start", font, 48);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    text.setPosition(300, 250);

    bool gameStarted = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // ������ ���۵��� ���� ��쿡�� ��ư Ŭ�� �̺�Ʈ ó��
            if (!gameStarted && event.type == sf::Event::MouseButtonPressed)
            {
                sf::FloatRect textBounds = text.getGlobalBounds();
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                if (textBounds.contains(mousePosition.x, mousePosition.y))
                {
                    std::cout << "Game started!" << std::endl;
                    gameStarted = true;
                    // ���� ���ۿ� �ʿ��� �ڵ带 �߰��ϼ���.
                }
            }
        }

        if (gameStarted) {
            // ���� ���� ������ �� ȭ�� ����� �Ķ������� ����
            window.clear(sf::Color::Blue);
        }
        else {
            // ������ ���۵��� ���� ��� ȭ�� ����� ������� �����ϰ� "���� ����" ��ư�� �׸��ϴ�.
            window.clear(sf::Color::White);
            window.draw(text);
        }

        window.display();
    }

    return 0;
}
