#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "shape.hh"
#include "../logman.hh"
#include "../resourcemanager.hh"

namespace Proton
{
  class Text : public Shape
  {
  public:
    Text(const std::string &text = "Label", int x = 0,
         int y = 0, const std::string &fontPath = "fonts/Roboto-Regular.ttf",
         int fontSize = 12, Color color = Color(255, 255, 255, 255))
        : labelText(text), fillColor(color), path(fontPath), fontSize(fontSize)
    {
      this->setPosition(x, y);
    }

    ~Text() override
    {
      if (this->textTexture)
        SDL_DestroyTexture(textTexture);
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

    virtual int getH() const
    {
      return this->h;
    }

    void setFillColor(Color newColor) override
    {
      this->fillColor = newColor;
      this->isDirty = true;
    }

    virtual void setText(const std::string &text)
    {
      this->labelText = text;
      this->isDirty = true;
    }

    const std::string &getText()
    {
      return this->labelText;
    }

    int getTextLength() const
    {
      return this->labelText.length();
    }

    virtual void paint(SDL_Renderer *render, int rX, int rY) override
    {
      if (this->isDirty)
      {
        this->createTexture(render);
      }

      if (textTexture)
      {
        float drawX = static_cast<float>(rX + this->x);
        float drawY = static_cast<float>(rY + this->y);

        SDL_FRect rectToRender = {drawX, drawY, (float)this->w, (float)this->h};

        SDL_RenderTexture(render, textTexture, nullptr, &rectToRender);
      }
    }

  protected:
    std::string labelText;
    std::string path;
    int fontSize;
    Color fillColor;
    SDL_Texture *textTexture = nullptr;
    bool isDirty = true;
    int w, h;

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

      SDL_Color sdlColor = {static_cast<Uint8>(fillColor.getR()),
                            static_cast<Uint8>(fillColor.getG()),
                            static_cast<Uint8>(fillColor.getB()),
                            static_cast<Uint8>(fillColor.getA())};

      SDL_Surface *surface = TTF_RenderText_Blended(font, labelText.c_str(), labelText.length(), sdlColor);
      if (!surface)
      {
        Proton::Log("TTF_RenderText_Blended error: ", SDL_GetError());
        return;
      }

      textTexture = SDL_CreateTextureFromSurface(render, surface);
      if (!textTexture)
      {
        Proton::Log("SDL_CreateTextureFromSurface error: ", SDL_GetError());
      }

      this->w = static_cast<float>(surface->w);
      this->h = static_cast<float>(surface->h);

      SDL_DestroySurface(surface);
      this->isDirty = false;
    }

    int getFontHeight() const
    {
      TTF_Font *font = ResourceManager::getInstance().getFont(this->path, this->fontSize);
      if (font)
      {
        return TTF_GetFontHeight(font);
      }
      return 0;
    }
  };
}