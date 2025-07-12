#pragma once
#include "audio.hh"
#include "physics.hh"
#include "proton/shapes/container.hh"
#include "shapes/buttonarea.hh"
#include "shapes/shape.hh"
#include "shapes/textbox.hh"
#include <SDL3/SDL_stdinc.h>
#include <vector>

#define ROOT_POSITION 0.0f, 0.0f

namespace Proton
{
class Scene
{
  public:
    virtual void keyPressed(Uint16 key) = 0;
    virtual void mouseDown(const Point &mPos) = 0;
    virtual auto update(float dt) -> Scene * = 0;

    Scene(SDL_Renderer *render, SDL_Window *window);

    virtual ~Scene();

    void addObject(Shape *shape);
    void addButton(ButtonArea *button);
    void addContainer(Container *container);

    void addTextBox(TextBox *textbox);

    void addAudio(Audio *audio);

    void clearScene();

    void paint();

    void handleButtonClick(const Point &mPos);
    void handleButtonClickEnd(const Point &mPos) const;

    void handleKeyDown(SDL_Event event);

    void handleMouseDrag(int mX, int mY);

    void handleTextInput(SDL_Event event);
    void addBody(PhysicsBody *body);
    auto getPhysicsBodies() -> std::vector<PhysicsBody *>;

    [[nodiscard]] auto getBackgroundColor() const -> Color;

  private:
    void destroyObjectMassive();
    std::vector<PhysicsBody *> physicsBodies;

  protected:
    SDL_Renderer *render;
    SDL_Window *window;

    std::vector<Shape *> objects;
    std::vector<ButtonArea *> buttons;
    std::vector<TextBox *> textboxes;
    std::vector<Container *> containers;
    std::vector<Audio *> audios;

    bool goNextScene = false;
    Scene *nextScene = nullptr;

    TextBox *focusedTextBox = nullptr;

    Color background;
};
}; // namespace Proton