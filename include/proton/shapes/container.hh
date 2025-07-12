#pragma once

#include "proton/shapes/buttonarea.hh"
#include "shape.hh"
#include <vector>

namespace Proton
{
class Container : public Shape
{
  public:
    explicit Container(const float x, const float y, const int width, const int height);

    void paint(SDL_Renderer *render, const float rX, const float rY) override;

    void setFillColor(Color /*unused*/) override {}

    void addObject(Shape *shape) { shapes.push_back(shape); }
    void addButton(ButtonArea *button)
    {
        buttons.push_back(button);
        shapes.push_back(button);
    }

    void clear();

    void setPosition(float x, float y) override;

    void update(const float dt) override;

    [[nodiscard]] auto getW() const -> float override { return this->containerRect.w; }
    [[nodiscard]] auto getH() const -> float override { return this->containerRect.h; }

    auto getButtons() -> std::vector<ButtonArea *> & { return this->buttons; }

    ~Container() override;

  private:
    SDL_FRect containerRect{};

    std::vector<Shape *> shapes;
    std::vector<ButtonArea *> buttons;
};
} // namespace Proton
