#pragma once
#include "shape.hh"

namespace Proton
{
class ProgressBar : public Shape
{
  public:
    explicit ProgressBar(float x = 0, float y = 0, float width = 100, float height = 20, float defaultProgress = 100);

    ~ProgressBar() override;

    void setFillColor(const Color newColor) override;

    void setProgress(float value);
    [[nodiscard]] auto getProgress() const -> float;

    [[nodiscard]] auto getW() const -> float override { return this->width; }
    [[nodiscard]] auto getH() const -> float override { return this->height; }

    void setRotation(const float angle) override;

    void paint(SDL_Renderer *render, float pX, float pY) override;

  private:
    float width, height;
    float progress;
    Color progressBarColor;
    SDL_Texture *texture;
    bool dirty;

    void rebuildTexture(SDL_Renderer *render);
};
} // namespace Proton
