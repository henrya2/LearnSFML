#include <SFML/Graphics.hpp>

#include <learnsfml/paths.hpp>

#include <cstdlib>

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "hello_sfml");
    window.setFramerateLimit(60);

    // Assets live in <repo>/assets and resolve relative to this executable,
    // NOT relative to the process working directory. For example:
    //     sf::Texture texture;
    //     if (!texture.loadFromFile(learnsfml::asset_path("my_texture.png")))
    //         return EXIT_FAILURE;
    // Drop a file into /assets and uncomment to try it.

    sf::CircleShape shape(120.f);
    shape.setFillColor(sf::Color(0x4c, 0xaf, 0x50));
    shape.setPosition({340.f, 180.f});

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(0x20, 0x20, 0x28));
        window.draw(shape);
        window.display();
    }

    return EXIT_SUCCESS;
}
