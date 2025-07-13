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
    ~Container() override;

    void paint(SDL_Renderer *render, const float rX, const float rY) override;

    void setFillColor(Color /*unused*/) override {}

    void addObject(Shape *shape)
    {
        shapes.push_back(shape);
        this->calculateContentBounds();
    }
    void addButton(ButtonArea *button)
    {
        buttons.push_back(button);
        shapes.push_back(button);
        this->calculateContentBounds();
    }

    void clear();

    void setPosition(float x, float y) override;

    void update(const float dt) override;

    [[nodiscard]] auto getW() const -> float override { return this->containerRect.w; }
    [[nodiscard]] auto getH() const -> float override { return this->containerRect.h; }

    auto getButtons() -> std::vector<ButtonArea *> & { return this->buttons; }

    void scrollBy(float dx, float dy);

    [[nodiscard]] auto getScrollX() const -> float { return this->scrollX; }
    [[nodiscard]] auto getScrollY() const -> float { return this->scrollY; }

  private:
    void calculateContentBounds();

    SDL_FRect containerRect{};

    std::vector<Shape *> shapes;
    std::vector<ButtonArea *> buttons;

    float scrollX = 0.0f;
    float scrollY = 0.0f;
    float contentWidth = 0.0f;
    float contentHeight = 0.0f;
};
} // namespace Proton
