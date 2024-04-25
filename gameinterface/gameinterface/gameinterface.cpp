#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#ifdef _WIN32
#include <windows.h> // Windows ��� ����
#endif

int main()
{
    // ������� �ػ� ��������
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    // SFML â ����
    sf::RenderWindow window;

    // ����� �ػ󵵿� �°� ��üȭ������ â ����
    window.create(sf::VideoMode(desktopMode.width, desktopMode.height), "My Game", sf::Style::Fullscreen);

    // ������ Ÿ��Ʋ �� ���̱�
    window.setMouseCursorVisible(true);

    // ��� �̹��� �ε�
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("init_screen.png")) {
        std::cerr << "Failed to load background image!" << std::endl;
        return 1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(window.getSize().x / static_cast<float>(backgroundTexture.getSize().x),
        window.getSize().y / static_cast<float>(backgroundTexture.getSize().y));

    // ���� ���� ��ư �̹��� �ε�
    sf::Texture startButtonTexture;
    if (!startButtonTexture.loadFromFile("gamestart.png")) {
        std::cerr << "Failed to load start button image!" << std::endl;
        return 1;
    }
    sf::Sprite startButtonSprite(startButtonTexture);
    startButtonSprite.setPosition((desktopMode.width - startButtonTexture.getSize().x) / 2, (desktopMode.height * 2) / 3);

    // �� �Է� ��ư �̹��� �ε�
    sf::Texture faceEnterButtonTexture;
    if (!faceEnterButtonTexture.loadFromFile("face_enter.png")) {
        std::cerr << "Failed to load face enter button image!" << std::endl;
        return 1;
    }
    sf::Sprite faceEnterButtonSprite(faceEnterButtonTexture);
    faceEnterButtonSprite.setPosition((desktopMode.width - faceEnterButtonTexture.getSize().x) / 2, (desktopMode.height * 2) / 3 + startButtonTexture.getSize().y + 20); // ���� ��ư �Ʒ��� �� �Է� ��ư ��ġ ����

    // ���� ȭ�� ��������Ʈ ����
    sf::RectangleShape blackScreen(sf::Vector2f(desktopMode.width, desktopMode.height));
    blackScreen.setFillColor(sf::Color::Black);

    // ��� ȭ�� ��������Ʈ ����
    sf::RectangleShape whiteScreen(sf::Vector2f(desktopMode.width, desktopMode.height));
    whiteScreen.setFillColor(sf::Color::White);

    // �Ķ� ȭ�� ��������Ʈ ����
    sf::RectangleShape blueScreen(sf::Vector2f(desktopMode.width, desktopMode.height));
    blueScreen.setFillColor(sf::Color::Blue);

    bool startClicked = false;
    bool faceEnterClicked = false;

    // ��Ʈ �ε�
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return 1;
    }

    // "GAME START!" �ؽ�Ʈ ����
    sf::Text startText("GAME START!", font, 200);
    startText.setFillColor(sf::Color::White);
    startText.setStyle(sf::Text::Bold);
    startText.setPosition((desktopMode.width - startText.getLocalBounds().width) / 2, (desktopMode.height - startText.getLocalBounds().height) / 2);

    // "FACE ENTER!" �ؽ�Ʈ ����
    sf::Text faceEnterText("FACE ENTER!", font, 200);
    faceEnterText.setFillColor(sf::Color::Black);
    faceEnterText.setStyle(sf::Text::Bold);
    faceEnterText.setPosition((desktopMode.width - faceEnterText.getLocalBounds().width) / 2, (desktopMode.height - faceEnterText.getLocalBounds().height) / 2);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                // ���� ���� ��ư Ŭ�� ���� Ȯ��
                sf::FloatRect startButtonBounds = startButtonSprite.getGlobalBounds();
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                if (startButtonBounds.contains(mousePosition.x, mousePosition.y))
                {
                    std::cout << "Game started! Showing GAME START!" << std::endl;
                    startClicked = true;
                }

                // �� �Է� ��ư Ŭ�� ���� Ȯ��
                sf::FloatRect faceEnterButtonBounds = faceEnterButtonSprite.getGlobalBounds();
                if (faceEnterButtonBounds.contains(mousePosition.x, mousePosition.y))
                {
                    std::cout << "Face enter button clicked! Showing FACE ENTER!" << std::endl;
                    faceEnterClicked = true;
                }
            }

            // ESC Ű�� ������ �� â�� ����� ������
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                // �����츦 ������ ���� ����
                window.create(sf::VideoMode(desktopMode.width, desktopMode.height), "My Game");
            }
        }

        window.clear();

        // ���� ���� ��ư Ŭ�� �� �Ķ� ȭ�鿡 "GAME START!" ǥ��
        if (startClicked)
        {
            window.draw(blueScreen);
            window.draw(startText);
            // ���� ���� ���� �߰�
            // ��: ���� ���� ����
        }
        // �� �Է� ��ư Ŭ�� �� �Ķ� ȭ�鿡 "FACE ENTER!" ǥ��
        else if (faceEnterClicked)
        {
            window.draw(whiteScreen);
            window.draw(faceEnterText);
            // �� �Է� ���� �߰�
            // ��: �� �Է� �������̽� ����
        }
        // �� �ܿ��� ���� ��ư �׸���
        else
        {
            window.draw(backgroundSprite);
            window.draw(startButtonSprite);
            window.draw(faceEnterButtonSprite);
        }

        window.display();
    }

    return 0;
}
