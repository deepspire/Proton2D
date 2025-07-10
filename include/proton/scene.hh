#pragma once
#include <SDL3/SDL_stdinc.h>
#include <vector>
#include "shapes/shape.hh"
#include "shapes/buttonarea.hh"
#include "shapes/textbox.hh"
#include "audio.hh"
#include "physics.hh"

#define ROOT_POSITION 0.0f, 0.0f

namespace Proton
{
    class Scene
    {
    public:
        virtual void keyPressed(Uint16 key) = 0;
        virtual void mouseDown(const Point &mPos) = 0;
        virtual Scene *update(float dt) = 0;

        Scene(SDL_Renderer *render, SDL_Window *window);

        virtual ~Scene();

        void addObject(Shape *shape);

        void addButton(ButtonArea *button);

        void addTextBox(TextBox *textbox);

        void addAudio(Audio *audio);

        void clearScene();

        void paint();

        void handleButtonClick(const Point& mPos);
        void handleButtonClickEnd(const Point& mPos) const;

        void handleKeyDown(SDL_Event event);

        void handleMouseDrag(int mX, int mY);

        void handleTextInput(SDL_Event event);
        void addBody(PhysicsBody* body);
        std::vector<PhysicsBody*> getPhysicsBodies();

        Color getBackgroundColor() const;

    private:
        void destroyObjectMassive();
        std::vector<PhysicsBody*> physicsBodies;

    protected:
        SDL_Renderer *render;
        SDL_Window *window;

        std::vector<Shape *> objects;
        std::vector<ButtonArea *> buttons;
        std::vector<TextBox *> textboxes;
        std::vector<Audio *> audios;

        bool goNextScene = false;
        Scene *nextScene = nullptr;

        TextBox *focusedTextBox = nullptr;

        Color background;
    };
};