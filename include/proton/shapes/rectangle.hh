#pragma once

#include "shape.hh"
#include <SDL3/SDL.h>

namespace Proton
{

class Rectangle final : public Shape
{
  public:
    enum Style : uint8_t
    {
        Fill,
        Bevel
    };

    Rectangle(const float x = 0, float y = 0, float w = 10, float h = 10, const Color color = Color(),
              Style style = Fill);

    ~Rectangle() override;

    void setFillColor(const Color newColor) override;

    void resize(const float width, const float height);

    [[nodiscard]] auto getW() const -> float override { return w; }
    [[nodiscard]] auto getH() const -> float override { return h; }

    void setStyle(const Style newStyle);

    [[nodiscard]] auto getStyle() const -> Style { return style; }

    void paint(SDL_Renderer *renderer, const float rX, const float rY) override;

  private:
    float w{}, h{};
    Color color{};
    Style style{};
    SDL_Texture *texture{};
    bool dirty{true};

    void rebuildTexture(SDL_Renderer *renderer);
};

} // namespace Proton
