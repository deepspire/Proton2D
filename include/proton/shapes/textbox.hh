#pragma once
#include "../logman.hh"
#include "../utf8helper.hh"
#include "text.hh"
#include <SDL3_ttf/SDL_ttf.h>

namespace Proton
{
class TextBox : public Text
{
  public:
    explicit TextBox(SDL_Window *window, const int boxW, const std::string &text = "Label", const float x = 0,
                     const float y = 0, const std::string &fontPath = "fonts/Roboto-Regular.ttf",
                     const int fontSize = 10, const Color color = Color(255, 255, 255, 255));

    [[nodiscard]] auto isFocused() const -> bool { return this->focused; }
    [[nodiscard]] auto getCursorPosition() const -> int { return this->cursorPosition; }
    [[nodiscard]] auto getSelectionAnchor() const -> int { return this->selectionAnchor; }

    void setFocused(const bool v);
    void setCursorPosition(const unsigned int v);
    void setSelectionAnchor(const int v);

    void appendCursorLeft();
    void appendCursorRight();

    void setText(const std::string &text, const bool dontUpdateCursor = false);
    void insertSymbol(const char *symbol);
    void removeAtCursor();

    [[nodiscard]] auto getCharIndexAt(const int pX) const -> int;

    void update(const float dt) override;
    void paint(SDL_Renderer *render, const float rX, const float rY) override;

    [[nodiscard]] auto getH() const -> float override
    {
        if (this->h > 0)
        {
            return static_cast<float>(this->h);
        }
        else
        {
            return static_cast<float>(this->getFontHeight());
        }
    }
    [[nodiscard]] auto getW() const -> float override { return static_cast<float>(this->boxW); }
    
    [[nodiscard]] auto getBoxW() const -> float { return static_cast<float>(this->boxW); }

  private:
    SDL_Window *window;
    bool focused = false;
    int cursorPosition = 0;
    float currentBlinkTime = 0;
    float requiredBlinkTime = 0.5; // в секундах
    bool cursorVisible = true;
    int selectionAnchor = -1;
    int scrollX = 0;
    int boxW = 0;

    void adjustScrollX();

    [[nodiscard]] auto getCursorPixelPosition() const -> int;
    [[nodiscard]] auto getPixelPosition(const int charIndex) -> int;
};
}; // namespace Proton