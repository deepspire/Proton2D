#pragma once
#include "shape.hh"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

namespace Proton
{
  class Text : public Shape
  {
  public:
    Text(SDL_Renderer *renderer, const std::string &text = "Label", int x = 0,
         int y = 0, const std::string &fontPath = "Roboto-Regular.ttf",
         int fontSize = 10, Color color = Color(255, 255, 255, 255))
        : renderer(renderer), labelText(text), fillColor(color)
    {
      font = TTF_OpenFont(fontPath.c_str(), fontSize);
      if (!font)
      {
        SDL_Log("Failed to load font: %s", SDL_GetError());
      }
      textRect.x = static_cast<float>(x);
      textRect.y = static_cast<float>(y);
      createTexture();
    }

    ~Text() override
    {
      if (textTexture)
        SDL_DestroyTexture(textTexture);
      if (font)
        TTF_CloseFont(font);
    }

    void setPosition(int x, int y) override
    {
      textRect.x = static_cast<float>(x);
      textRect.y = static_cast<float>(y);
    }

    void setFillColor(Color newColor) override
    {
      fillColor = newColor;
      createTexture();
    }

    void setText(const std::string &text)
    {
      labelText = text;
      createTexture();
    }

    void paint(SDL_Renderer * /*unused*/) override
    {
      if (textTexture)
      {
        SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);
      }
    }

  private:
    SDL_Renderer *renderer;
    TTF_Font *font = nullptr;
    std::string labelText;
    Color fillColor;
    SDL_Texture *textTexture = nullptr;
    SDL_FRect textRect = {0.f, 0.f, 0.f, 0.f};

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

      SDL_Surface *surface = TTF_RenderText_Blended(font, labelText.c_str(),
                                                    labelText.length(), sdlColor);
      if (!surface)
      {
        SDL_Log("TTF_RenderText_Blended error: %s", SDL_GetError());
        return;
      }

      textTexture = SDL_CreateTextureFromSurface(renderer, surface);
      if (!textTexture)
      {
        SDL_Log("SDL_CreateTextureFromSurface error: %s", SDL_GetError());
      }

      textRect.w = static_cast<float>(surface->w);
      textRect.h = static_cast<float>(surface->h);

      SDL_DestroySurface(surface);
    }
  };
}