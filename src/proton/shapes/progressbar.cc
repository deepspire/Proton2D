#include "proton/shapes/progressbar.hh"
#include "proton/logman.hh"

namespace Proton
{
ProgressBar::ProgressBar(float x, float y, float width, float height, float defaultProgress)
    : width(width), height(height), progress(defaultProgress), texture(nullptr), dirty(true)
{
    this->position.x = x;
    this->position.y = y;
}
ProgressBar::~ProgressBar()
{
    if (texture)
        SDL_DestroyTexture(texture);
}

void ProgressBar::setFillColor(const Color newColor)
{
    progressBarColor = newColor;
    dirty = true;
}

void ProgressBar::setProgress(float value)
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
        Log("Progress value can't be lower than 0.0 or greater than 100.0! You're trying to set value: ", value);
    }
}
auto ProgressBar::getProgress() const -> float { return this->progress; }

void ProgressBar::setRotation(const float angle) { this->rotation = angle; }

void ProgressBar::paint(SDL_Renderer *render, float pX, float pY)
{
    if (!texture || dirty)
        rebuildTexture(render);

    if (!texture)
        return;

    const SDL_FRect dst = {this->position.x + pX, this->position.y + pY, width,
                           height};
    SDL_RenderTextureRotated(render, texture, nullptr, &dst, rotation, nullptr, SDL_FLIP_NONE);
}

void ProgressBar::rebuildTexture(SDL_Renderer *render)
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }

        texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, static_cast<int>(width), static_cast<int>(height));
        if (!texture)
            return;

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);
        SDL_SetRenderTarget(render, texture);
        SDL_SetRenderDrawColor(render, progressBarColor.getR() > 50 ? progressBarColor.getR() - 50 : 0,
                               progressBarColor.getG() > 50 ? progressBarColor.getG() - 50 : 0,
                               progressBarColor.getB() > 50 ? progressBarColor.getB() - 50 : 0,
                               progressBarColor.getA());
        SDL_FRect bgRect = {0, 0, width, height};
        SDL_RenderFillRect(render, &bgRect);
        float progressWidth = (progress / 100.0f) * (width - 4);
        SDL_FRect barRect = {2, 2, progressWidth, height - 4};
        SDL_SetRenderDrawColor(render, progressBarColor.getR(), progressBarColor.getG(), progressBarColor.getB(),
                               progressBarColor.getA());
        SDL_RenderFillRect(render, &barRect);

        SDL_SetRenderTarget(render, nullptr);
        dirty = false;
    }
} // namespace Proton