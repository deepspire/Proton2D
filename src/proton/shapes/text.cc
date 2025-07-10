#include "proton/shapes/text.hh"
#include "proton/logman.hh"
#include "proton/resourcemanager.hh"
#include <utility>

namespace Proton
{
Text::Text(std::string text, const float x, const float y, std::string fontPath, const int fontSize, const Color color)
    : labelText(std::move(text)), path(std::move(fontPath)), fontSize(fontSize), fillColor(color)
{
    this->Text::setPosition(x, y);
}

Text::~Text()
{
    if (this->textTexture)
        SDL_DestroyTexture(textTexture);
}

void Text::resize(const int w, const int h)
{
    this->w = w;
    this->h = h;
}

void Text::setFillColor(const Color newColor)
{
    this->fillColor = newColor;
    this->isDirty = true;
}

void Text::setText(const std::string &text)
{
    this->labelText = text;
    this->isDirty = true;
}

void Text::paint(SDL_Renderer *render, const float rX, const float rY)
{
    if (this->isDirty)
    {
        this->createTexture(render);
    }

    if (textTexture)
    {
        const auto drawX = rX + this->position.x;
        const auto drawY = rY + this->position.y;

        const SDL_FRect rectToRender = {drawX, drawY, static_cast<float>(this->w), static_cast<float>(this->h)};

        SDL_RenderTextureRotated(render, textTexture, nullptr, &rectToRender, this->rotation, nullptr, SDL_FLIP_NONE);
    }
}

void Text::createTexture(SDL_Renderer *render)
{
    if (textTexture)
    {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }

    this->w = 0;
    this->h = 0;

    if (this->labelText.empty())
    {
        this->isDirty = false;
        return;
    }

    TTF_Font *font = ResourceManager::getInstance().getFont(this->path, fontSize);
    if (!font)
        return;

    const SDL_Color sdlColor = {(fillColor.getR()), (fillColor.getG()), (fillColor.getB()), (fillColor.getA())};

    SDL_Surface *surface = TTF_RenderText_Blended(font, labelText.c_str(), labelText.length(), sdlColor);
    if (!surface)
    {
        Log("TTF_RenderText_Blended error: ", SDL_GetError());
        return;
    }

    textTexture = SDL_CreateTextureFromSurface(render, surface);
    SDL_SetTextureScaleMode(textTexture, SDL_SCALEMODE_NEAREST);
    if (!textTexture)
    {
        Log("SDL_CreateTextureFromSurface error: ", SDL_GetError());
    }

    this->w = surface->w;
    this->h = surface->h;

    SDL_DestroySurface(surface);
    this->isDirty = false;
}

[[nodiscard]] auto Text::getFontHeight() const -> int
{
    if (const TTF_Font *font = ResourceManager::getInstance().getFont(this->path, this->fontSize))
    {
        return TTF_GetFontHeight(font);
    }
    return 0;
}
} // namespace Proton