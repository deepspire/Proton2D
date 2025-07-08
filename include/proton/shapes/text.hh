#pragma once
#include "../logman.hh"
#include "../resourcemanager.hh"
#include "shape.hh"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <utility>

namespace Proton
{
class Text : public Shape
{
  public:
    explicit Text(std::string text = "Label", const float x = 0, const float y = 0,
                  std::string fontPath = "fonts/Roboto-Regular.ttf", const int fontSize = 12,
                  const Color color = Color(255, 255, 255, 255))
        : labelText(std::move(text)), path(std::move(fontPath)), fontSize(fontSize), fillColor(color)
    {
        this->Text::setPosition(x, y);
    }

    ~Text() override
    {
        if (this->textTexture)
            SDL_DestroyTexture(textTexture);
    }

    void setPosition(const float x, const float y) override
    {
        this->position.x = x;
        this->position.y = y;
    }

    void resize(const int w, const int h)
    {
        this->w = w;
        this->h = h;
    }

    [[nodiscard]] int getW() const { return this->w; }

    [[nodiscard]] virtual int getH() const { return this->h; }

    void setFillColor(const Color newColor) override
    {
        this->fillColor = newColor;
        this->isDirty = true;
    }

    virtual void setText(const std::string &text)
    {
        this->labelText = text;
        this->isDirty = true;
    }

    const std::string &getText() { return this->labelText; }

    [[nodiscard]] int getTextLength() const { return this->labelText.length(); }

    void paint(SDL_Renderer *render, const float rX, const float rY) override
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

            SDL_RenderTextureRotated(render, textTexture, nullptr, &rectToRender, this->rotation, nullptr,
                                     SDL_FLIP_NONE);
        }
    }

  protected:
    std::string labelText;
    std::string path;
    int fontSize;
    Color fillColor;
    SDL_Texture *textTexture = nullptr;
    bool isDirty = true;
    int w{}, h{};

    void createTexture(SDL_Renderer *render)
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
        if (!textTexture)
        {
            Log("SDL_CreateTextureFromSurface error: ", SDL_GetError());
        }

        this->w = static_cast<float>(surface->w);
        this->h = static_cast<float>(surface->h);

        SDL_DestroySurface(surface);
        this->isDirty = false;
    }

    [[nodiscard]] int getFontHeight() const
    {
        if (const TTF_Font *font = ResourceManager::getInstance().getFont(this->path, this->fontSize))
        {
            return TTF_GetFontHeight(font);
        }
        return 0;
    }
};
} // namespace Proton