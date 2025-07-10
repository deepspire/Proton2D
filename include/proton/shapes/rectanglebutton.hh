#pragma once

#include "buttonarea.hh"
#include "rectangle.hh"

namespace Proton
{
class RectangleButton final : public ButtonArea
{
  public:
    explicit RectangleButton(float X = 0, float Y = 0, float W = 10, float H = 10, Color color = Color(0, 0, 0));

    ~RectangleButton() override { delete this->rectangle; }

    void setPosition(const float x, const float y) override;
    void setRotation(const float angle) override { this->rotation = angle; }

    void resize(const float w, const float h) override;

    void paint(SDL_Renderer *render, const float rX, const float rY) override { rectangle->paint(render, rX, rY); }

    void setFillColor(const Color color) override { rectangle->setFillColor(color); }

  protected:
    Rectangle *rectangle;
};
} // namespace Proton