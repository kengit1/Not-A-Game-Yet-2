#include "Game.h"
#include "Components.hpp"
#include "EntityManager.hpp"
#include "Vec2.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cstddef>
#include <cstdio>
#include <iostream>

Game::Game(const std::string& config)
{
    init(config);
}

void Game::setDifficulty(Difficulty difficulty = Easy)
{
    // it will be usefuk with the imGui
    //* right now , we adjust the cCollision
    const auto& entityVec = m_entities.getEntities();
    //! making the EnitityMnaager same name as the EntityVec is Wild
    // simply , making every tangible entity its default cCollision is the same as its cShape
    //! a simple but deprecated use , redius * enum value
    for (const auto& entity : entityVec)
    {
        if (entity->isActive() && entity->getComponent<CCollision>().exists)
        {
            entity->getComponent<CCollision>().radius = entity->getComponent<CShape>().circle.getRadius() * difficulty;
        }
    }
}

void Game::init(const std::string& config)
{
    // TODO: read in config file here
    //       use the premade PlayerConfig, EnemyConfig, BulletConfig variables
    //?      i think we should consider adding the 'default difficulty'

    // set up default window parameters
    m_window.create(sf::VideoMode({1000, 500}), "Assignment 2");
    m_window.setFramerateLimit(60);

    spawnPlayer();
}

void Game::run()
{
    // TODO: add pause functionality in here
    //       some systems should function while paused (rendering)
    //       some systems shouldn't (movement / input)
    while (m_running)
    {
        setDifficulty();
        m_entities.update();

        sEnemySpawner();
        sMovement();
        sCollision();
        sUserInput();
        sLifespan();
        m_player->getComponent<CTransform>().angle += 10.0f;
        sRender();

        // increment the current frame
        // may need to be moved when pause implemented
        m_currentFrame++;
    }
}

void Game::setPaused(bool paused)
{
    // TODO
}

// respawn the player in the middle of the screen
void Game::spawnPlayer()
{
    // TODO: Finish adding all properties of the player with the correct values from the config

    // We create every entity by calling EntityManager.addEntity(tag)*
    // This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
    auto entity = m_entities.addEntity("player");

    // Give this entity a Transform so it spawns at (200,200) with velocity (1,1) and angle 0
    entity->addComponent<CTransform>(Vec2<float>(150.0f, 150.0f), Vec2<float>(3.0f, 3.0f), 0.0f);

    // The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
    entity->addComponent<CShape>(32.0f, 10, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);
    entity->addComponent<CInput>();
    entity->addComponent<CCollision>();
    //?entity->addComponent<CLifespan>(120); was just testing the removal of dead entities
    // Add an input component to the player so that we can use inputs
    // Since we want this Entity to be our player, set our Game's player variable to be this Entity
    // This goes slightly against the EntityManager paradigm, but we use the player so much it's worth it
    m_player = entity;
}

// spawn an enemy at a random position
void Game::spawnEnemy()
{
    // TODO: make sure the enemy is spawned properly with the m_enemyConfig variables
    //       the enemy must be spawned completely within the bounds of the window
    //
    auto entity = m_entities.addEntity("enemy");

    // record when the most recent enemy was spawned
    m_lastEnemySpawnTime = m_currentFrame;
}

// spawns the small enemies when a large one (input entity e) explodes
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
    // TODO: spawn small enemies at the location of the input enemy e

    // when we create the smaller enemy, we have to read the values of the original enemy
    // - spawn a number of small enemies equal to the vertices of the original enemy
    // - set each small enemy to the same color as the original, half the size
    // - small enemies are worth double points of the original enemy
}

// spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2<float>& target)
{
    // TODO: implement the spawning of a bullet which travels toward target
    //       - bullet speed is given as a scalar speed
    //       - you must set the velocity by using formula in notes
    auto bullet = m_entities.addEntity("bullet");
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
    // TODO: implement your own special weapon
}

void Game::sMovement()
{
    // TODO: implement all entity movement in this function
    //       you should read the m_player->cInput component to determine if the player is moving

    m_player->getComponent<CTransform>().pos += m_player->getComponent<CTransform>().velocity;
}

void Game::sLifespan()
{
    for (const auto& e : m_entities.getEntities())
    {
        auto& lifespan = e->getComponent<CLifespan>();
        if (lifespan.exists)
        {
            if (lifespan.remaining > 0)
            {
                // TODO: implement the alpha channel transformation
                lifespan.remaining--;
            }
            if (lifespan.remaining <= 0 && e->isActive())
            {
                e->destroy();
            }
        }
    }
}

void Game::sCollision()
{
    // TODO: implement all proper collisions between entities
    //       be sure to use the collision radius, NOT the shape radius
    /*
    i could've doen this  :
    EntityManager manager;
    auto player = manager.getEntities("Player");
    auto bulletVec = manager.getEntities("Bullet");
    auto enemyVec = manager.getEntities("Enemy");
    */
    // for sake of memory , Vec class has methods to see the length of the Vec & distance between Vecs
    // Player - Wall
    auto& posPlayer = m_player->getComponent<CTransform>().pos;
    auto& velPlayer = m_player->getComponent<CTransform>().velocity;
    auto radiusPlayer = m_player->getComponent<CShape>().circle.getRadius();
    auto sizeWindow = m_window.getSize();
    if (posPlayer.x + radiusPlayer > sizeWindow.x)
    {
        // initial logic , with inputs its better to make the up = false or zeroing the velocity
        velPlayer.x *= -1;
        // posPlayer.x = sizeWindow.x - radiusPlayer;
    }

    if (posPlayer.x - radiusPlayer < 0)
    {
        velPlayer.x *= -1;
        // posPlayer.x = radiusPlayer;
    }

    if (posPlayer.y + radiusPlayer > sizeWindow.y)
    {
        // initial logic , with inputs its better to make the up = false or zerong the velocity
        velPlayer.y *= -1;
        // posPlayer.y = sizeWindow.y - radiusPlayer;
    }
    if (posPlayer.y - radiusPlayer < 0)
    {
        velPlayer.y *= -1;
        // posPlayer.y = radiusPlayer;
    }
    // Bullet - Enemy
    // Player - Enemy
}

void Game::sEnemySpawner()
{
    // TODO: code which implements enemy spawning should go here
    //
    //      (use m_currentFrame - m_lastEnemySpawnTime) to determine
    //      how long it has been since the last enemy spawned
}

void Game::sRender()
{
    m_window.clear();

    // TODO: change the code below to draw ALL of the entities
    //       sample drawing of the player Entity that we have created

    for (auto& entity : m_entities.getEntities())
    {
        if (entity->getComponent<CShape>().exists && entity->isActive())
        {
            entity->getComponent<CShape>().circle.setPosition(entity->getComponent<CTransform>().pos);
            entity->getComponent<CShape>().circle.setRotation(sf::degrees(entity->getComponent<CTransform>().angle));

            m_window.draw(entity->getComponent<CShape>().circle);
        }
    }
    m_window.display();
}

void Game::sUserInput()
{
    // TODO: handle user input here
    //       note that you should only be setting the player's input component variables here
    //       you should not implement the player's movement logic here
    //       the movement system will read the variables you set in this function

    while (const std::optional event = m_window.pollEvent())
    {
        // this event triggers when the window is closed
        if (event->is<sf::Event::Closed>())
        {
            m_running = false;
        }

        // this event is triggered when a key is pressed
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            switch (keyPressed->code)
            {
                case sf::Keyboard::Key::W:
                    std::cout << "W Key Pressed\n";
                    m_player->getComponent<CInput>().up = true;
                    break;

                default:
                    break;
            }
        }

        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            switch (keyReleased->code)
            {
                case sf::Keyboard::Key::W:
                    std::cout << "W Key Released\n";
                    m_player->getComponent<CInput>().up = false;
                    break;

                default:
                    break;
            }
        }

        if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left)
            {
                std::cout << "Left Mouse Button Clicked at (" << mouseButtonPressed->position.x << ","
                          << mouseButtonPressed->position.y << ")\n";
                // call spawnBullet here
            }

            if (mouseButtonPressed->button == sf::Mouse::Button::Right)
            {
                std::cout << "Right Mouse Button Clicked at (" << mouseButtonPressed->position.x << ","
                          << mouseButtonPressed->position.y << ")\n";
                // call spawnSpecialWeapon here
            }
        }
    }
}