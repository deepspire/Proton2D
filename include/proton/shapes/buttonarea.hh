#pragma once

#include "shape.hh"
#include <functional>

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

    void onClick() const;
    void onClickEnded() const;
    [[nodiscard]] virtual auto containsPoint(float x, float y) const -> bool;
    void setClickListener(const std::function<void()>& f);
    void setClickEndedListener(const std::function<void()>& f);
    [[nodiscard]] auto getIsFocused() const -> bool;
    void setIsFocused(bool a);

  protected:
    float w, h;
    float x, y;
    std::function<void()> clickListener;
    std::function<void()> unclickListener;
    bool isFocused = false;

};
}; // namespace Proton