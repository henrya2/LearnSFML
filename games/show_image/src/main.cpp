#include <SFML/Graphics.hpp>

#include <learnsfml/paths.hpp>

#include <cstdlib>
#include <iostream>

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "show_image");
    window.setFramerateLimit(60);

    // Assets live in <repo>/assets and resolve relative to this executable,
    // NOT relative to the process working directory. For example:
    //     sf::Texture texture;
    //     if (!texture.loadFromFile(learnsfml::asset_path("my_texture.png")))
    //         return EXIT_FAILURE;
    // Drop a file into /assets and uncomment to try it.

    sf::Texture texture;
    if (!texture.loadFromFile(learnsfml::asset_path("logo.png")))
    {
        std::cerr << "Failed to load texture: " << learnsfml::asset_path("logo.png") << std::endl;
        return EXIT_FAILURE;
    }

    // 3. Create a sprite and assign the texture to it
    // Sprites are lightweight and point to a texture
    sf::Sprite sprite(texture);

    // Optional: Position the sprite
    sprite.setPosition({50.f, 50.f});

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(0x20, 0x20, 0x28));
        window.draw(sprite);
        window.display();
    }

    return EXIT_SUCCESS;
}
