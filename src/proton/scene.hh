#pragma once
#include <SDL3/SDL_stdinc.h>
#include <vector>
#include "shapes/shape.hh"
#include "shapes/buttonarea.hh"
#include "shapes/textbox.hh"

#define ROOT_POSITION 0, 0

namespace Proton
{
    class Scene
    {
    public:
        virtual void keyPressed(Uint16 key) = 0;
        virtual void mouseDown() = 0;
        virtual void update(Uint64 dt) = 0;

        virtual ~Scene()
        {
            this->destroyObjectMassive();
            this->buttons.clear();
        }

        void addObject(Shape *shape)
        {
            this->objects.push_back(shape);
        }

        void addButton(ButtonArea *button)
        {
            this->buttons.push_back(button);
            addObject(button);
        }

        void addTextBox(TextBox *textbox)
        {
            this->textboxes.push_back(textbox);
            addObject(textbox);
        }

        void clearScene()
        {
            this->destroyObjectMassive();
        }

        void paint(float dt)
        {
            for (Shape *shape : this->objects)
            {
                shape->update(dt);
                if (shape->getVisible())
                {
                    shape->paint(ROOT_POSITION);
                }
            }
        }

        void handleButtonClick(int mX, int mY)
        {
            for (ButtonArea *button : this->buttons)
            {
                if ((button->getX() <= mX && mX <= button->getX() + button->getW()) &&
                    (button->getY() <= mY && mY <= button->getY() + button->getH()))
                {
                    button->onClick();
                }
            }

            TextBox *clickedTextBox = nullptr;
            for (TextBox *textbox : this->textboxes)
            {
                if ((textbox->getX() <= mX && mX <= textbox->getX() + textbox->getW()) &&
                    (textbox->getY() <= mY && mY <= textbox->getY() + textbox->getH()))
                {
                    clickedTextBox = textbox;
                    break;
                }
            }

            if (this->focusedTextBox != nullptr && this->focusedTextBox != clickedTextBox)
            {
                this->focusedTextBox->setFocused(false);
                this->focusedTextBox = nullptr;
            }

            if (clickedTextBox != nullptr && this->focusedTextBox != clickedTextBox)
            {
                this->focusedTextBox = clickedTextBox;
                this->focusedTextBox->setFocused(true);
            }
        }

        void handleKeyDown(SDL_Event event)
        {
            if (this->focusedTextBox != nullptr)
            {
                switch (event.key.key)
                {
                case (SDLK_HOME):
                    Log("SDLK_HOME");
                    this->focusedTextBox->setCursorPosition(0);
                    break;
                case (SDLK_END):
                    Log("SDLK_END");
                    this->focusedTextBox->setCursorPosition(this->focusedTextBox->getTextLength());
                    break;
                case (SDLK_LEFT):
                    Log("SDLK_LEFT");
                    this->focusedTextBox->appendCursorLeft();
                    break;
                case (SDLK_RIGHT):
                    Log("SDLK_RIGHT");
                    this->focusedTextBox->appendCursorRight();
                    break;
                case (SDLK_BACKSPACE):
                    Log("SDLK_BACKSPACE");
                    this->focusedTextBox->removeAtCursor();
                default:
                    break;
                }
            }
        }

        void handleTextInput(SDL_Event event)
        {
            if (this->focusedTextBox != nullptr)
            {
                this->focusedTextBox->insertSymbol(event.text.text);
            }
        }

        SDL_Renderer *render;

    private:
        void destroyObjectMassive()
        {
            for (Shape *shape : objects)
            {
                delete shape;
            }
            objects.clear();
        }

        TextBox *focusedTextBox = nullptr;

    protected:
        std::vector<Shape *> objects;
        std::vector<ButtonArea *> buttons;
        std::vector<TextBox *> textboxes;
    };
};