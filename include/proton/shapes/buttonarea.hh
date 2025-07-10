#pragma once
#include "shape.hh"

namespace Proton
{
class ButtonArea : public Shape
{
  public:
    explicit ButtonArea(const float x = 0, const float y = 0, const float w = 10, const float h = 10);

    void paint(SDL_Renderer *render, float rX, float rY) override {}

    void setPosition(const float x, const float y) override;

    void setFillColor([[maybe_unused]] Color color) override {}

    virtual void resize(const float w, const float h);

    [[nodiscard]] auto getW() const -> float override { return this->w; }
    [[nodiscard]] auto getH() const -> float override { return this->h; }

    void onClick();
    void setClickListener(void (*f)());

  protected:
    float w, h;
    void (*clickListener)() = nullptr;
};
}; // namespace Proton