#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "shape.hh"
#include "../logman.hh"

namespace Proton
{
  class Text : public Shape
  {
  public:
    Text(SDL_Renderer *render, const std::string &text = "Label", int x = 0,
         int y = 0, const std::string &fontPath = "fonts/Roboto-Regular.ttf",
         int fontSize = 10, Color color = Color(255, 255, 255, 255))
        : labelText(text), fillColor(color)
    {
      this->render = render;
      this->font = TTF_OpenFont(fontPath.c_str(), fontSize);
      if (!this->font)
      {
        Proton::Log("Failed to load font: ", SDL_GetError());
      }
      this->setPosition(x, y);
      createTexture();
      this->isVisible = true;
    }

    ~Text() override
    {
      if (this->textTexture)
        SDL_DestroyTexture(textTexture);
      if (this->font)
        TTF_CloseFont(this->font);
    }

    void setPosition(int x, int y) override
    {
      this->x = x;
      this->y = y;
    }

    void resize(int w, int h)
    {
      this->w = w;
      this->h = h;
    }

    int getW() const
    {
      return this->w;
    }

    int getH() const
    {
      return this->h;
    }

    void setFillColor(Color newColor) override
    {
      this->fillColor = newColor;
      createTexture();
    }

    virtual void setText(const std::string &text)
    {
      this->labelText = text;
      createTexture();
    }

    const std::string &getText()
    {
      return this->labelText;
    }

    int getTextLength() const
    {
      return this->labelText.length();
    }

    virtual void paint(int rX, int rY) override
    {
      if (textTexture)
      {
        float drawX = static_cast<float>(rX + this->x);
        float drawY = static_cast<float>(rY + this->y);

        SDL_FRect rectToRender = {drawX, drawY, (float)this->w, (float)this->h};

        SDL_RenderTexture(this->render, textTexture, nullptr, &rectToRender);
      }
    }

  protected:
    TTF_Font *font = nullptr;
    std::string labelText;
    Color fillColor;
    SDL_Texture *textTexture = nullptr;
    int w, h;

  private:
    void createTexture()
    {
      if (textTexture)
      {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
      }

      SDL_Color sdlColor = {static_cast<Uint8>(fillColor.getR()),
                            static_cast<Uint8>(fillColor.getG()),
                            static_cast<Uint8>(fillColor.getB()),
                            static_cast<Uint8>(fillColor.getA())};

      SDL_Surface *surface = TTF_RenderText_Blended(font, labelText.c_str(), labelText.length(), sdlColor);
      if (!surface)
      {
        SDL_Log("TTF_RenderText_Blended error: %s", SDL_GetError());
        return;
      }

      textTexture = SDL_CreateTextureFromSurface(this->render, surface);
      if (!textTexture)
      {
        SDL_Log("SDL_CreateTextureFromSurface error: %s", SDL_GetError());
      }

      this->w = static_cast<float>(surface->w);
      this->h = static_cast<float>(surface->h);

      SDL_DestroySurface(surface);
    }
  };
}