#include "ss.hh"

Proton::Container *cnt;
Proton::Group *grp;

SecondScene::SecondScene(SDL_Renderer *render, SDL_Window *window) : Proton::Scene(render, window)
{
    this->background = Proton::Color(0, 200, 0);
    cnt = new Proton::Container(0, 0, 200, 500);
    cnt->addObject(new Proton::Rectangle(0, 0, 30, 30, Proton::Color(255, 0, 0)));
    cnt->addObject(new Proton::Circle(35, 35, 10, Proton::Color(0, 255, 0)));
    cnt->addObject(new Proton::Image(Proton::ResourceManager::getInstance().getTexture(this->render, "kachan.png"), 50, 50, 20, 20));
    cnt->addObject(new Proton::Line(0, 0, 30, 30));
    cnt->addObject(new Proton::Text("Huesos!", 70, 70));
    grp = new Proton::Group(50, 50);
    grp->addObject(new Proton::Rectangle(0, 0, 10, 10, Proton::Color(255, 0, 0)));
    grp->addObject(new Proton::Rectangle(30, 0, 10, 10, Proton::Color(0, 0, 255)));


    this->addTextBox(new Proton::TextBox(this->window, "Hello, TextBox!", 50, 250, "fonts/Roboto-Regular.ttf", 20, Proton::Color(255, 255, 255, 255)));

    this->addObject(cnt);
    this->addObject(grp);
}

void SecondScene::mouseDown(int x, int y)
{
    if (x <= 100 && y <= 100)
    {
        this->goNextScene = true;
        this->nextScene = new Menu(this->render, this->window);
    }
    else
    {
        // rect->setPosition(rand() % 700, rand() % 500);
        // img->setPosition(rand() % 200, rand() % 200);
    }
}

void SecondScene::keyPressed(Uint16 key)
{
    if (key == SDLK_A)
    {
        grp->setPosition(rand() % 700, rand() % 500);
    }
}

Proton::Scene* SecondScene::update(float dt)
{
    for (Proton::Shape* shape : this->objects) {
        shape->update(dt);
    }

    if (this->goNextScene)
    {
        return this->nextScene;
    }

    return this;
}
