#include "menu.hh"

Menu::Menu(SDL_Renderer *render, SDL_Window *window) : Proton::Scene(render, window)
{
}

void Menu::mouseDown(int x, int y)
{
    this->goNextScene = true;
    this->nextScene = new SecondScene(this->render, this->window);
}

void Menu::keyPressed([[maybe_unused]] Uint16 ev) {}
Proton::Scene *Menu::update(float dt)
{
    clearScene();
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            Proton::Rectangle *rect = new Proton::Rectangle(i * 2, j * 2, 2, 2, Proton::Color(rand() % 256, rand() % 256, rand() % 256));
            addObject(rect);
        }
    }
    addObject(new Proton::Rectangle(480 / 2 - 25, 640 / 2 - 25, 50, 50, Proton::Color()));
    addObject(new Proton::Line(480 / 2, 640 / 2, 480, 640, Proton::Color()));
    addObject(new Proton::Line(0, 640, 480 / 2, 640 / 2, Proton::Color()));
    addObject(new Proton::Circle(480 / 2, 640 / 2 + 100, 50, Proton::Color(0, 255, 0)));

    if (this->goNextScene)
    {
        return this->nextScene;
    }

    return this;
}
