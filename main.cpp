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
};

struct ParticlesElements 
{
    sf::Color          ColorParticle;
    sf::Vector2f       PositionParticle;
    const int          Radius;
    float y_velocity  = 150.f;
    int x_velocity  = static_cast<float>((Generators::indexGenerator() - 5.f) * 50.f);
};
struct ParticlesManager : Particle
{
    static std::deque<ParticlesElements> list_particle;
    const float energy_loss = 0.95f;
    const float fps         = 60;
    const float delta_time  = 1 / fps;

    static void addParticles(sf::RenderWindow& window)
    {
        int index = indexGenerator();
        // add particles in the particle list
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            // Set Color, position and radius to a particle.
            list_particle.push_back({
                // Color
                HexColors::hexColors[index - 1], 
                //Position
                sf::Vector2f(sf::Mouse::getPosition(window)),
                // Radius
                index * 5
            });
        }
    }
    sf::Vector2f aplyingGravity(ParticlesElements& elements,
        int width,
        int height,
        float gravity)
    {
        elements.y_velocity += gravity * delta_time;

        elements.PositionParticle.x += elements.x_velocity * delta_time;
        elements.PositionParticle.y += elements.y_velocity * delta_time;

        if (elements.PositionParticle.y < 0)
        {
            elements.PositionParticle.y = 0;
            elements.y_velocity = -elements.y_velocity * energy_loss;
        } else if (elements.PositionParticle.y > height - (elements.Radius * 2)) 
        {
            elements.y_velocity = -elements.y_velocity * energy_loss;
        } else if (elements.PositionParticle.x < 0)
        {
            elements.PositionParticle.x = 0;
            elements.x_velocity = -elements.x_velocity * energy_loss;
        } else if (elements.PositionParticle.x > width - (elements.Radius * 2))
        {
            elements.x_velocity = -elements.x_velocity * energy_loss;
        }

        return elements.PositionParticle;
    }

    void draw(sf::RenderWindow& window, float gravity)
    {
        for (auto& single_particle : list_particle)
        {
            /**
             * Particles values
             * ---------------------
             * Color sf::Color
             * Position sf::Vector2f
             * Radius int 
             */
            particle.setFillColor(single_particle.ColorParticle);

            particle.setPosition(aplyingGravity(single_particle, 800, 600, gravity));

            particle.setRadius(single_particle.Radius);
            // Drawing each particle
            window.draw(particle);
        }
    }
    static void ballsCreated()
    {
        // It prints how many was created
        std::cout << "Balls created: " << list_particle.size() << "\n";
    }
};

std::deque<ParticlesElements> ParticlesManager::list_particle;

int main()
{
    const unsigned int width  = 800;
    const unsigned int height = 600;

    const float gravity = 500;

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
        particle.draw(window, gravity);
        window.display();
    }
    particle.ballsCreated();
    return 0;
}
