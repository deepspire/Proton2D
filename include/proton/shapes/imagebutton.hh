#pragma once

#include "buttonarea.hh"
#include "image.hh"

namespace Proton
{
class ImageButton : public ButtonArea
{
  public:
    explicit ImageButton(SDL_Texture *texture, const float x, const float y, const float w, const float h);
    ~ImageButton() override;

    void setRotation(const float angle) override;

    void setPosition(const float x, const float y) override;

    void resize(const float w, const float h) override;

    void paint(SDL_Renderer *render, const float rX, const float rY) override;

    void setFillColor([[maybe_unused]] Color color) override {}

  protected:
    Image *image;
};
}; // namespace Proton