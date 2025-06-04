#include <SDL3/SDL_stdinc.h>
#include "ss.hh"
#include "menu.hh"
#include "proton/shapes/rectangle.hh"
#include "proton/shapes/line.hh"
#include "proton/shapes/circle.hh"
#include "proton/shapes/text.hh"
#include "proton/shapes/image.hh"
#include "proton/shapes/rectanglebutton.hh"
#include "proton/shapes/imagebutton.hh"
#include "proton/shapes/container.hh"

Proton::Rectangle *rect;
Proton::Text *text;
Proton::Image *img;
Proton::TextBox *textbox;
Proton::Container *cnt;
Proton::Text *someContainerText;

SecondScene::SecondScene(Proton::Display *display)
{
    this->render = display->getRenderer();
    this->display = display;
    display->setBackground(Proton::Color(0, 0, 255));
    rect = new Proton::Rectangle(this->render, 10, 10, 50, 50);
    addObject(rect);
    addObject(new Proton::Rectangle(this->render, 110, 10, 50, 50, Proton::Color()));
    addObject(new Proton::Rectangle(this->render, 10, 100, 50, 50, Proton::Color()));
    addObject(new Proton::Rectangle(this->render, 60, 150, 50, 50, Proton::Color()));
    addObject(new Proton::Rectangle(this->render, 110, 100, 50, 50, Proton::Color()));
    addObject(new Proton::Line(0, 0, 500, 500));
    addObject(new Proton::Circle(this->render, 250, 250, 50, Proton::Color(255, 0, 0)));
    text = new Proton::Text(this->render, "Hello, world!", 50, 50, "fonts/Roboto-Regular.ttf", 40);
    img = new Proton::Image(this->render, "kachan.png", 50, 50, 250, 120);
    addObject(img);
    addObject(text);

    Proton::RectangleButton *button = new Proton::RectangleButton(this->render, 200, 0, 50, 50, Proton::Color(125, 125, 125));
    addButton(button);

    Proton::ImageButton *imagebutton = new Proton::ImageButton(this->render, 50, 150, 50, 50, "kachan.png");
    addButton(imagebutton);

    textbox = new Proton::TextBox(this->render, display->getNativeWindow(), "Hello, TextBox!", 50, 250, "fonts/Roboto-Regular.ttf", 20, Proton::Color(255, 255, 255, 255));
    addTextBox(textbox);

    cnt = new Proton::Container(display, 50, 50, 200, 500);
    someContainerText = new Proton::Text(this->render, "Качан", 30, 30, "fonts/Roboto-Regular.ttf", 150);
    cnt->addObject(someContainerText);

    addObject(cnt);
}

void SecondScene::mouseDown()
{
    if (display->pointerX <= 100 && display->pointerY <= 100)
    {
        Menu *menu = new Menu(display);
        this->display->setScene(menu);
    }
    else
    {
        rect->setPosition(rand() % 700, rand() % 500);
        //img->setPosition(rand() % 200, rand() % 200);
    }
}

void SecondScene::keyPressed(Uint16 key)
{
    if (key == SDLK_A)
    {
        rect->setVisible(!rect->getVisible());
        rect->setPosition(rand() % 700, rand() % 500);
        text->setPosition(rand() % 700, rand() % 500);
    }
}
void SecondScene::update([[maybe_unused]] Uint64 dt)
{
    //img->setPosition(this->display->pointerX, this->display->pointerY);
}
