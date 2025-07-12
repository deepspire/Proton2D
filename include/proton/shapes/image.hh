#pragma once

#include "shape.hh"

namespace Proton
{
class Image : public Shape
{
  public:
    explicit Image(SDL_Texture *texture, const float x = 0, const float y = 0, const float width = 0,
                   const float height = 0);

    ~Image() override = default;

    void setFillColor([[maybe_unused]] Color c) override {}

    void resize(const float width, const float height);

    [[nodiscard]] auto getW() const -> float override { return this->width; }
    [[nodiscard]] auto getH() const -> float override { return this->height; }

    void paint(SDL_Renderer *render, const float rX, const float rY) override;

    void setRotation(const float angle) override { this->rotation = angle; }

    void setTexture(SDL_Texture *texture);

    void setAlpha(float alpha);
    auto getAlpha() -> float;

  private:
    float width, height;
    SDL_Texture *imageTexture;
    float alpha = 1;
};
} // namespace Proton