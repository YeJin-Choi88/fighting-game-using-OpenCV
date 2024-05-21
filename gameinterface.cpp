#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <chrono>

#ifdef _WIN32
#include <windows.h> // Windows ��� ����
#endif

int main()
{
    const int windowWidth = 1800;
    const int windowHeight = 1000;

    // SFML â ����
    sf::RenderWindow window;

    // ������ ũ��� â ����
    window.create(sf::VideoMode(windowWidth, windowHeight), "My Game", sf::Style::Default);

    // ������ Ÿ��Ʋ �� ���̱�
    window.setMouseCursorVisible(true);

    // ī�޶�
    cv::VideoCapture cap;
    cap = cv::VideoCapture(0, cv::CAP_DSHOW);

    if (!cap.isOpened()) {
        std::cerr << "Error: Camera could not be opened" << std::endl;
        return -1;
    }

    // ī�޶��� �ʺ�� ���̸� ������ â ũ��� ����
    cap.set(cv::CAP_PROP_FRAME_WIDTH, windowWidth);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, windowHeight);

    cv::Mat frame;
    sf::Texture cameraTexture;
    sf::Sprite cameraSprite;

    // ��� �̹��� �ε�
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("bg2.jpg")) {
        std::cerr << "Failed to load background image!" << std::endl;
        return 1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    // ��� ��������Ʈ�� ũ�⸦ ������ â ũ�⿡ ���߱�
    backgroundSprite.setScale(windowWidth / static_cast<float>(backgroundTexture.getSize().x),
        windowHeight / static_cast<float>(backgroundTexture.getSize().y));

    // ���� ���� ��ư �̹��� �ε�
    sf::Texture startButtonTexture;
    if (!startButtonTexture.loadFromFile("gamestart.png")) {
        std::cerr << "Failed to load start button image!" << std::endl;
        return 1;
    }
    sf::Sprite startButtonSprite(startButtonTexture);
    // ��ư ũ�⸦ ���̱� (��: 50% ũ���)
    startButtonSprite.setScale(0.7f, 0.7f);
    startButtonSprite.setPosition((windowWidth - startButtonTexture.getSize().x * 0.7f) / 2, (windowHeight * 2) / 3);

    // �� �Է� ��ư �̹��� �ε�
    sf::Texture faceEnterButtonTexture;
    if (!faceEnterButtonTexture.loadFromFile("face_enter.png")) {
        std::cerr << "Failed to load face enter button image!" << std::endl;
        return 1;
    }
    sf::Sprite faceEnterButtonSprite(faceEnterButtonTexture);
    // ��ư ũ�⸦ ���̱� (��: 50% ũ���)
    faceEnterButtonSprite.setScale(0.7f, 0.7f);
    faceEnterButtonSprite.setPosition((windowWidth - faceEnterButtonTexture.getSize().x * 0.7f) / 2, (windowHeight * 2) / 3 + startButtonTexture.getSize().y * 0.7f + 20); // ���� ��ư �Ʒ��� �� �Է� ��ư ��ġ ����

    // �� ���(��� �ȵǾ� ������) ��ư �̹��� �ε�
    sf::Texture RegisterButtonTexture;
    if (!RegisterButtonTexture.loadFromFile("register.png")) {
        std::cerr << "Failed to load register button image!" << std::endl;
        return 1;
    }

    sf::Sprite RegisterButtonSprite(RegisterButtonTexture);
    RegisterButtonSprite.setScale(0.5f, 0.5f);
    RegisterButtonSprite.setPosition(1650, 20); // ���� ��ư �Ʒ��� �� �Է� ��ư ��ġ ����

    // ��� ȭ�� ��������Ʈ ����
    sf::RectangleShape whiteScreen(sf::Vector2f(windowWidth, windowHeight));
    whiteScreen.setFillColor(sf::Color::White);

    bool startClicked = false;
    bool faceEnterClicked = false;
    bool inGameMode = false; // ���� ������� ����

    // ��Ʈ �ε�
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return 1;
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!inGameMode && event.type == sf::Event::MouseButtonPressed)
            {
                // ���� ���� ��ư Ŭ�� ���� Ȯ��
                sf::FloatRect startButtonBounds = startButtonSprite.getGlobalBounds();
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                if (startButtonBounds.contains(mousePosition.x, mousePosition.y))
                {
                    std::cout << "Game started! Showing GAME START!" << std::endl;
                    startClicked = true;
                    faceEnterClicked = false;  // Ensure only one mode is active
                    inGameMode = true;
                }

                // �� �Է� ��ư Ŭ�� ���� Ȯ��
                sf::FloatRect faceEnterButtonBounds = faceEnterButtonSprite.getGlobalBounds();
                if (faceEnterButtonBounds.contains(mousePosition.x, mousePosition.y))
                {
                    std::cout << "Face enter button clicked! Showing FACE ENTER!" << std::endl;
                    faceEnterClicked = true;
                    startClicked = false;  // Ensure only one mode is active
                    inGameMode = true;
                }
            }

            // ESC Ű�� ������ �� ���� ����� ��쿡�� ó��
            if (inGameMode && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                // ���� ��忡�� ���
                inGameMode = false;

                // ���� ��ư Ŭ�� ���� �ʱ�ȭ
                startClicked = false;
                faceEnterClicked = false;

                // �����츦 ������ ���� ����
                window.create(sf::VideoMode(windowWidth, windowHeight), "My Game");
                window.setMouseCursorVisible(true);
            }
        }

        window.clear();

        if (startClicked || faceEnterClicked)
        {
            cap >> frame;

            if (!frame.empty()) {
                // OpenCV Mat�� SFML Texture�� ����
                cv::cvtColor(frame, frame, cv::COLOR_BGR2RGBA);
                cameraTexture.create(frame.cols, frame.rows);
                cameraTexture.update(frame.data, frame.cols, frame.rows, 0, 0);

                // Texture�� Sprite�� �����Ͽ� SFML â�� ǥ��
                cameraSprite.setTexture(cameraTexture);
                
                window.draw(cameraSprite);
                window.draw(RegisterButtonSprite);
            }
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
