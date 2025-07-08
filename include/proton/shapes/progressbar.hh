#pragma once
#include "../logman.hh"
#include "shape.hh"

namespace Proton
{
class ProgressBar : public Shape
{
  public:
    explicit ProgressBar(float x = 0, float y = 0, int width = 100, int height = 20, int defaultProgress = 100)
        : width(width), height(height), progress(defaultProgress), texture(nullptr), dirty(true)
    {
        this->position.x = x;
        this->position.y = y;
    }

    ~ProgressBar() override
    {
        if (texture)
            SDL_DestroyTexture(texture);
    }

    void setPosition(float x, float y) override
    {
        this->position.x = x;
        this->position.y = y;
    }

    void setFillColor(const Color newColor) override
    {
        progressBarColor = newColor;
        dirty = true;
    }

    void setProgress(int value)
    {
        if (value >= 0 && value <= 100)
        {
            if (progress != value)
            {
                progress = value;
                dirty = true;
            }
        }
        else
        {
            Log("Progress value can't be lower than 0 or greater than 100! You're trying to set value: ", value);
        }
    }

    [[nodiscard]] int getProgress() const { return progress; }

    void setRotation(const float angle) override { rotation = angle; }

    void paint(SDL_Renderer *render, float pX, float pY) override
    {
        if (!texture || dirty)
            rebuildTexture(render);

        if (!texture)
            return;

        const SDL_FRect dst = {this->position.x + pX, this->position.y + pY, static_cast<float>(width), static_cast<float>(height)};
        SDL_RenderTextureRotated(render, texture, nullptr, &dst, rotation, nullptr, SDL_FLIP_NONE);
    }

  private:
    int width, height;
    int progress;
    Color progressBarColor;
    SDL_Texture *texture;
    bool dirty;

    void rebuildTexture(SDL_Renderer *render)
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }

        texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
        if (!texture)
            return;

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);
        SDL_SetRenderTarget(render, texture);
        SDL_SetRenderDrawColor(render, progressBarColor.getR() > 50 ? progressBarColor.getR() - 50 : 0,
                               progressBarColor.getG() > 50 ? progressBarColor.getG() - 50 : 0,
                               progressBarColor.getB() > 50 ? progressBarColor.getB() - 50 : 0,
                               progressBarColor.getA());
        SDL_FRect bgRect = {0, 0, static_cast<float>(width), static_cast<float>(height)};
        SDL_RenderFillRect(render, &bgRect);
        float progressWidth = (static_cast<float>(progress) / 100.0f) * (width - 4);
        SDL_FRect barRect = {2, 2, progressWidth, static_cast<float>(height - 4)};
        SDL_SetRenderDrawColor(render, progressBarColor.getR(), progressBarColor.getG(), progressBarColor.getB(),
                               progressBarColor.getA());
        SDL_RenderFillRect(render, &barRect);

        SDL_SetRenderTarget(render, nullptr);
        dirty = false;
    }
};
} // namespace Proton
