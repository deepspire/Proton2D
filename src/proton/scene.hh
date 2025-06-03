#pragma once
#include <SDL3/SDL_stdinc.h>
#include <vector>
#include "shapes/shape.hh"
#include "shapes/buttonarea.hh"

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
            buttons.clear();
        }

        void addObject(Shape *shape)
        {
            objects.push_back(shape);
        }

        void addButton(ButtonArea *button)
        {
            buttons.push_back(button);
            addObject(button);
        }

        void clearScene()
        {
            this->destroyObjectMassive();
        }

        void paint()
        {
            for (Shape *shape : objects)
            {
                shape->paint(this->render);
            }
        }

        void handleButtonClick(int mX, int mY)
        {
            for (ButtonArea *button : buttons)
            {
                if ((button->getX() <= mX && mX <= button->getX() + button->getW()) &&
                    (button->getY() <= mY && mY <= button->getY() + button->getH()))
                {
                    button->onClick();
                }
            }
        }

        std::vector<Shape *> objects;
        std::vector<ButtonArea *> buttons;
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
    };
};