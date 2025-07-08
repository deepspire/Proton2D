#include "ss.hh"
#include "proton/shapes/circle.hh"
#include "proton/shapes/container.hh"
#include "proton/shapes/group.hh"
#include "proton/shapes/image.hh"
#include "proton/shapes/line.hh"
#include "proton/shapes/progressbar.hh"
#include "proton/shapes/rectangle.hh"
#include "proton/shapes/text.hh"

Proton::Container *cnt;
Proton::Group *grp;
Proton::ProgressBar *bar;

Proton::Audio *tmp;

SecondScene::SecondScene(SDL_Renderer *render, SDL_Window *window) : Proton::Scene(render, window)
{

    this->background = Proton::Color(125, 125, 125);
    cnt = new Proton::Container(0, 0, 200, 500);
    cnt->addObject(new Proton::Rectangle(0, 0, 30, 30, Proton::Color(255, 0, 0)));
    cnt->addObject(new Proton::Circle(35.0f, 35.0f, 10, Proton::Color(0, 255, 0)));
    cnt->addObject(new Proton::Image(Proton::ResourceManager::getInstance().getTexture(this->render, "kachan.png"), 50,
                                     50, 20, 20));
    cnt->addObject(new Proton::Line(0, 0, 30, 30));
    cnt->addObject(new Proton::Text("Proton2D text rendering", 70, 70, "fonts/Roboto-Regular.ttf"));
    grp = new Proton::Group(50, 50);
    grp->addObject(new Proton::Rectangle(0, 0, 10, 10, Proton::Color(255, 0, 0)));
    grp->addObject(new Proton::Rectangle(30, 0, 10, 10, Proton::Color(0, 0, 255)));
    bar = new Proton::ProgressBar(100, 100, 200, 50);
    bar->setFillColor(Proton::Color(0, 255, 0));
    bar->setRotation(55.5f);
    grp->addObject(new Proton::Line(0, 0, 50, 50));
    grp->addObject(new Proton::Image(Proton::ResourceManager::getInstance().getTexture(this->render, "kachan.png"), 50,
                                     50, 20, 20));
    tmp = new Proton::Audio("kachan.mp3");
    this->addAudio(tmp);

    this->addTextBox(new Proton::TextBox(this->window, 125, "Hello, TextBox!", 50, 250, "fonts/Roboto-Regular.ttf", 15,
                                         Proton::Color(255, 255, 255, 255)));

    this->addObject(cnt);
    this->addObject(grp);
    grp->addObject(bar);
}

void SecondScene::mouseDown(const Proton::Point &mPos)
{
    float mX = mPos.x;
    float mY = mPos.y;

    if (mX <= 100 && mY <= 100)
    {
        this->goNextScene = true;
        this->nextScene = new Menu(this->render, this->window);
    }
    else
    {
        tmp->play();
        // rect->setPosition(rand() % 700, rand() % 500);
        // img->setPosition(rand() % 200, rand() % 200);
    }
}

void SecondScene::keyPressed(Uint16 key)
{
    if (this->focusedTextBox != nullptr) {
        return;
}

    if (key == SDLK_A)
    {
        grp->setPosition(static_cast<float>(rand() % 700), static_cast<float>(rand() % 500));
        bar->setProgress(rand() % 100);
        tmp->play();
    }
}

SecondScene::~SecondScene() = default;

Proton::Scene *SecondScene::update(const float dt)
{
    grp->setRotation(grp->getRotation() + dt * 300);
    bar->setRotation(bar->getRotation() + dt * 500);
    for (Proton::Shape *shape : this->objects)
    {
        shape->update(dt);
    }

    if (this->goNextScene)
    {
        return this->nextScene;
    }

    return this;
}
