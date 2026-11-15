#include <SFML/Graphics.hpp>
#include <iostream>
#include <deque>
#include <array>
#include <random>

namespace HexColors
{
constexpr std::array<sf::Color, 10> hexColors =
    {
    // --- Vivid / Vibrant ---
    sf::Color(0xE7, 0x4C, 0x3C), // 0. Crimson Red      (#E74C3C)
    sf::Color(0xE6, 0x7E, 0x22), // 1. Pumpkin Orange   (#E67E22)
    sf::Color(0xF1, 0xC4, 0x0F), // 2. Sunflower Yellow (#F1C40F)
    sf::Color(0x2E, 0xCC, 0x71), // 3. Emerald Green    (#2ECC71)
    sf::Color(0x34, 0x98, 0xDB), // 4. Peter River Blue (#3498DB)
    sf::Color(0x9B, 0x59, 0xB6), // 5. Amethyst Purple  (#9B59B6)
    // --- Pastels ---
    sf::Color(0xFF, 0x9A, 0x9E), // 6. Soft Rose Pink   (#FF9A9E)
    sf::Color(0xA8, 0xE6, 0xCF), // 7. Mint Green       (#A8E6CF)
    sf::Color(0xDC, 0xD6, 0xCD), // 8. Warm Gray        (#DCD6CD)
    // --- Dark / Deep ---
    sf::Color(0x1A, 0x25, 0x2C), // 9. Midnight Dark    (#1A252C)
    };
}; // hexColor namespace

struct ParticlesElements 
{
    sf::Color    ColorParticle;
    sf::Vector2f PositionParticle;
    const int    Radius;
};

struct Generators
{
    const static int indexGenerator()
    {
        constexpr int color_limit = 10;
        // Obtains a seed from hardware
        std::random_device rd;
        std::mt19937 gen(rd());
        // Set a range
        std::uniform_int_distribution<int> seed_number(1, color_limit);
        return seed_number(gen);
    }
};

struct Particle : Generators
{
    sf::CircleShape    particle;
    float              radius;
};

struct ParticlesManager : Particle
{
    static std::deque<ParticlesElements> list_particle;

    static void addParticles(sf::RenderWindow& window)
    {
        int index = indexGenerator();
        // add particles in the particle list
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            // Set Color and position
            list_particle.push_back({
                HexColors::hexColors[index - 1], 
                sf::Vector2f(sf::Mouse::getPosition(window)),
                index * 10
            });
        }
    }

    void draw(sf::RenderWindow& window)
    {
        for (const auto& single_particle : list_particle)
        {
            // Add gravity in here.

            /**
             * Particles values
             * ---------------------
             * Color sf::Color
             * Position sf::Vector2f
             * Radius int 
             */
            particle.setFillColor(single_particle.ColorParticle);
            particle.setPosition(single_particle.PositionParticle);
            particle.setRadius(single_particle.Radius);

            // Drawing each particle
            window.draw(particle);
        }
    }
};

std::deque<ParticlesElements> ParticlesManager::list_particle;

int main()
{
    const unsigned int width  = 800;
    const unsigned int height = 600;

    sf::RenderWindow window(sf::VideoMode({width, height}), "Particles");
    ParticlesManager particle;

    window.setFramerateLimit(60);
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // Loop for event that requires just one click
            if  (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            // Creates the particles based on the mouse's clicks
            if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePress->button == sf::Mouse::Button::Left)
                {
                    particle.addParticles(window);
                }
            }
        }

        window.clear();
        particle.draw(window);
        window.display();
    }
}
