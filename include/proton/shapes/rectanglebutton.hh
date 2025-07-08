#pragma once
#include "buttonarea.hh"
#include "rectangle.hh"

namespace Proton
{
class RectangleButton final : public ButtonArea
{
  public:
    explicit RectangleButton(int X = 0, int Y = 0, int W = 10, int H = 10, Color color = Color(0, 0, 0))
        : ButtonArea(X, Y, W, H), clickListener(nullptr)
    {
        this->rectangle = new Rectangle(X, Y, W, H, color);
    }

    ~RectangleButton() override { delete this->rectangle; }

    void setPosition(const float x, const float y) override
    {
        this->x = x;
        this->y = y;
        rectangle->setPosition(x, y);
    }

    void setRotation(const float angle) override { this->rotation = angle; }

    void setClickListener(void (*f)()) { this->clickListener = f; }

    void resize(const int w, const int h) override
    {
        this->w = w;
        this->h = h;
        rectangle->resize(w, h);
    }

    void paint(SDL_Renderer *render, const float rX, const float rY) override { rectangle->paint(render, rX, rY); }

    void setFillColor(const Color color) override { rectangle->setFillColor(color); }

    void onClick() override
    {
        if (clickListener)
            clickListener();
    }

  protected:
    Rectangle *rectangle;
    void (*clickListener)();
};
} // namespace Proton