#pragma once

#include "shape.hh"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

namespace Proton
{
class Text : public Shape
{
  public:
    explicit Text(std::string text = "Label", const float x = 0, const float y = 0,
                  std::string fontPath = "fonts/Roboto-Regular.ttf", const int fontSize = 12,
                  const Color color = Color(255, 255, 255, 255));

    ~Text() override;

    void resize(const int w, const int h);

    [[nodiscard]] auto getW() const -> float override { return static_cast<float>(this->w); }
    [[nodiscard]] auto getH() const -> float override { return static_cast<float>(this->h); }

    void setFillColor(const Color newColor) override;

    virtual void setText(const std::string &text);

    auto getText() -> const std::string & { return this->labelText; }
    [[nodiscard]] auto getTextLength() const -> int { return static_cast<int>(this->labelText.length()); }

    void paint(SDL_Renderer *render, const float rX, const float rY) override;
    void setAlpha(float alpha);

  protected:
    std::string labelText;
    std::string path;
    int fontSize;
    Color fillColor;
    SDL_Texture *textTexture = nullptr;
    bool isDirty = true;
    int w{}, h{};
    float alpha = 1;

    void createTexture(SDL_Renderer *render);

    [[nodiscard]] auto getFontHeight() const -> int;
};
} // namespace Proton