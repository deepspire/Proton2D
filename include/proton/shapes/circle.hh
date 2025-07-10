#pragma once

#include "shape.hh"
#include <SDL3/SDL.h>
#include <cstdint>

namespace Proton
{
class Circle : public Shape
{
  public:
    enum Style : uint8_t
    {
        Fill,
        Bevel
    };

    explicit Circle(float x = 0, float y = 0, int radius = 40, Color fillColor = Color(), Style style = Fill);

    ~Circle() override;

    void setFillColor(Color newColor) override;

    void setStyle(Style s);

    void paint(SDL_Renderer *render, float rX, float rY) override;

    [[nodiscard]] auto getW() const -> float override { return static_cast<float>(radius * 2); }
    [[nodiscard]] auto getH() const -> float override { return static_cast<float>(radius * 2); }

  private:
    int radius;
    Color color;
    Style style;
    SDL_Texture *texture;
    bool dirty;

    void rebuildTexture(SDL_Renderer *render);
};
} // namespace Proton
