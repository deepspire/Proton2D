#include "menu.hh"
#include "proton/physics.hh"
#include "proton/resourcemanager.hh"
#include "proton/shapes/circle.hh"
#include "proton/shapes/image.hh"
#include "proton/shapes/rectangle.hh"
#include "ss.hh"

Proton::PhysicsBody *body;
Proton::PhysicsBody *body2;
Proton::PhysicsBody *body3;
Proton::PhysicsBody *body4;
Proton::Image *physicRectangle;
Proton::Rectangle *physicRectangle2;
Proton::Rectangle *physicRectangle3;
Proton::Rectangle *moving;
Proton::Circle *r1;

bool movingflag = false;

Menu::Menu(SDL_Renderer *render, SDL_Window *window) : Scene(render, window)
{
    moving = new Proton::Rectangle(200, 450, 50, 50, Proton::Color());
    moving->setRotation(60);
    addObject(moving);

    physicRectangle =
        new Proton::Image(Proton::ResourceManager::getInstance().getTexture(render, "kachan1.png"), 150, 50, 100, 100);
    body = new Proton::PhysicsBoxBody(Proton::PhysicsBody::Dynamic, 100 * Proton::METERS_PER_PIXEL, 100 * Proton::METERS_PER_PIXEL, 1, 0);
    body->setPositionInPixels(150, 50);
    body->bindShape(physicRectangle);

    physicRectangle2 = new Proton::Rectangle(100, 350, 100, 50, Proton::Color(255, 0, 0));
    body2 = new Proton::PhysicsBoxBody(Proton::PhysicsBody::Static, 100 * Proton::METERS_PER_PIXEL, 50 * Proton::METERS_PER_PIXEL, 1, 0);
    body2->setPositionInPixels(100, 350);
    body2->setRotation(10);
    body2->bindShape(physicRectangle2);

    physicRectangle3 = new Proton::Rectangle(170, 400, 100, 50, Proton::Color(0, 0, 255));
    body3 = new Proton::PhysicsBoxBody(Proton::PhysicsBody::Static, 100 * Proton::METERS_PER_PIXEL, 50 * Proton::METERS_PER_PIXEL, 1, 0);
    body3->setPositionInPixels(170, 400);
    body3->setRotation(50);
    body3->bindShape(physicRectangle3);

    r1 = new Proton::Circle(240, 420, 50, Proton::Color(255, 0, 255), Proton::Circle::Fill);
    body4 = new Proton::PhysicsCircleBody(Proton::PhysicsBody::Static, 50 * Proton::METERS_PER_PIXEL, 1, 0);
    body4->setPositionInPixels(240, 420);
    body4->bindShape(r1);

    addObject(physicRectangle);
    addObject(physicRectangle2);
    addObject(physicRectangle3);
    addObject(r1);
    //addObject(new Proton::Circle(240, 320 + 100, 50, Proton::Color(0, 255, 0), Proton::Circle::Bevel));

    addBody(body);
    addBody(body2);
    addBody(body3);
    addBody(body4);
}

void Menu::mouseDown(const Proton::Point &mPos)
{
    this->goNextScene = true;
    this->nextScene = new SecondScene(this->render, this->window);
}

void Menu::keyPressed([[maybe_unused]] Uint16 ev) {}
Proton::Scene *Menu::update(const float dt)
{
    moving->setRotation(moving->getRotation() + dt * 40);
    body4->setRotation(r1->getRotation() + dt * 100);
    if (movingflag)
    {
        moving->setPosition(moving->getX() - 40 * dt, moving->getY());
        if (moving->getX() <= 0)
        {
            moving->setPosition(600, moving->getY());
            movingflag = false;
        }
    }
    else
    {
        moving->setPosition(moving->getX() + 40 * dt, moving->getY());
        if (moving->getX() >= 600)
        {
            movingflag = true;
        }
    }

    if (this->goNextScene)
    {
        return this->nextScene;
    }

    return this;
}