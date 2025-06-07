#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include "text.hh"
#include "../logman.hh"
#include "../utf8helper.hh"

namespace Proton
{
    class TextBox : public Text
    {
    public:
        TextBox(SDL_Window *window, const std::string &text = "Label", int x = 0,
                int y = 0, const std::string &fontPath = "fonts/Roboto-Regular.ttf",
                int fontSize = 10, Color color = Color(255, 255, 255, 255)) : Text(text, x, y, fontPath, fontSize, color)
        {
            this->window = window;
            this->resize(static_cast<int>(this->w), static_cast<int>(this->h));
            this->scrollX = 0;
            this->adjustScrollX();
            this->isVisible = true;
        }

        bool isFocused()
        {
            return this->focused;
        }

        void setFocused(bool v)
        {
            if (this->focused == v)
                return;

            this->focused = v;
            if (v)
            {
                SDL_StartTextInput(this->window);
                this->cursorVisible = true;
            }
            else
            {
                SDL_StopTextInput(this->window);
                this->cursorVisible = false;
            }

            this->currentBlinkTime = 0;
        }

        int getCursorPosition()
        {
            return this->cursorPosition;
        }

        void setCursorPosition(unsigned int v)
        {
            if (v > (unsigned int)this->labelText.length())
            {
                Log("attempt to set cursor position > than label text length");
            }
            else
            {
                this->currentBlinkTime = 0;
                this->cursorVisible = true;
                this->cursorPosition = v;
                this->adjustScrollX();
            }
        }

        void setSelectionAnchor(int v)
        {
            if (v == -1)
            {
                this->selectionAnchor = -1;
                return;
            }

            if (v < 0 || v > (int)this->labelText.length())
            {
                Log("attempt to set selection anchor position > than label text length");
            }
            else
            {
                this->selectionAnchor = v;
            }
        }

        void appendCursorLeft()
        {
            if (this->cursorPosition > 0)
            {
                size_t currentPos = this->cursorPosition;
                while (currentPos > 0)
                {
                    currentPos--;
                    unsigned char firstByte = static_cast<unsigned char>(this->labelText[currentPos]);
                    if ((firstByte & 0xC0) != 0x80)
                    {
                        int char_len = getCharLength(firstByte);
                        if (char_len > 0 && (currentPos + char_len <= this->cursorPosition))
                        {
                            this->setCursorPosition(currentPos);
                            return;
                        }
                    }
                }
                this->setCursorPosition(0);
            }
        }

        void appendCursorRight()
        {
            if (this->cursorPosition < this->labelText.length())
            {
                unsigned char firstByte = static_cast<unsigned char>(this->labelText[this->cursorPosition]);
                int char_len = getCharLength(firstByte);
                if (char_len > 0)
                {
                    this->setCursorPosition(this->cursorPosition + char_len);
                }
            }
        }

        void setText(const std::string &text, bool dontUpdateCursor = false)
        {
            Text::setText(text);
            if (!dontUpdateCursor)
            {
                this->setCursorPosition(this->getTextLength());
            }
            else
            {
                this->adjustScrollX();
            }
        }

        void insertSymbol(const char *symbol)
        {
            this->labelText.insert(this->cursorPosition, symbol);
            size_t sizeNewBytes = strlen(symbol);
            this->setCursorPosition(this->cursorPosition + sizeNewBytes);
            this->setText(this->labelText, true);
        }

        void removeAtCursor()
        {
            if (this->cursorPosition == 0 || this->labelText.empty())
                return;

            size_t charStartPos = 0;
            size_t scanPos = 0;
            while (scanPos < this->cursorPosition)
            {
                if (scanPos >= this->labelText.length())
                    return;
                charStartPos = scanPos;
                unsigned char firstByte = static_cast<unsigned char>(this->labelText[scanPos]);
                int charLen = getCharLength(firstByte);
                if (charLen <= 0)
                    return;
                scanPos += charLen;
            }
            if (charStartPos < this->cursorPosition)
            {
                int charLen = getCharLength(static_cast<unsigned char>(this->labelText[charStartPos]));
                if (charLen > 0 && charStartPos + charLen <= this->labelText.length() && charStartPos + charLen == scanPos)
                {
                    this->labelText.erase(charStartPos, charLen);
                    this->setCursorPosition(charStartPos);
                    this->setText(labelText, true);
                }
            }
        }

        int getCharIndexAt(int pX)
        {
            int targetPixelX = pX + this->scrollX;

            TTF_Font *font = ResourceManager::getInstance().getFont(this->path, this->fontSize);
            if (!font)
            {
                return 0;
            }

            if (targetPixelX <= 0)
            {
                return 0;
            }

            if (this->labelText.empty())
            {
                return 0;
            }

            int currentWidth = 0;
            int prevWidth = 0;

            for (size_t charIndex = 0; charIndex < this->labelText.length();)
            {
                prevWidth = currentWidth;

                int charLen = getCharLength(static_cast<unsigned char>(this->labelText[charIndex]));
                if (charLen == 0)
                {
                    charLen = 1;
                }

                std::string sub = this->labelText.substr(0, charIndex + charLen);
                TTF_GetStringSize(font, sub.c_str(), sub.length(), &currentWidth, nullptr);

                if (targetPixelX >= prevWidth && targetPixelX <= currentWidth)
                {
                    if (targetPixelX - prevWidth < currentWidth - targetPixelX)
                    {
                        return charIndex;
                    }
                    else
                    {
                        return charIndex + charLen;
                    }
                }

                charIndex += charLen;
            }

            return this->labelText.length();
        }

        void update(float dt) override
        {
            this->currentBlinkTime += dt;
            if (this->currentBlinkTime >= this->requiredBlinkTime)
            {
                this->currentBlinkTime = 0;
                this->cursorVisible = !this->cursorVisible;
            }
        }

        void paint(SDL_Renderer *render, int rX, int rY) override
        {
            Text::paint(render, rX, rY);

            if (this->focused && this->cursorVisible)
            {
                TTF_Font *font = ResourceManager::getInstance().getFont(this->path, this->fontSize);
                if (!font)
                {
                    return;
                }

                int fontHeight = TTF_GetFontHeight(font);

                int cursorPixelX = getCursorPixelPosition();
                float cursorDrawX = static_cast<float>(this->x + rX + (cursorPixelX - scrollX));

                if (cursorDrawX >= (this->x + rX) && cursorDrawX <= (this->x + rX + this->w))
                {
                    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
                    SDL_RenderLine(
                        render,
                        cursorDrawX,
                        static_cast<float>(this->y + rY),
                        cursorDrawX,
                        static_cast<float>(this->y + rY + fontHeight));
                }
            }
        }

    private:
        SDL_Window *window;
        bool focused = false;
        int cursorPosition = 0;
        float currentBlinkTime = 0;
        float requiredBlinkTime = 0.5; // в секундах
        bool cursorVisible = true;
        int scrollX = 0;
        int selectionAnchor = -1;

        void adjustScrollX()
        {
            int textureW = static_cast<int>(this->w);
            int cursorPixelX = getCursorPixelPosition();

            if (textureW > this->w)
            {

                if (cursorPixelX < scrollX)
                {
                    scrollX = cursorPixelX;
                }

                else if (cursorPixelX > scrollX + this->w - 1)
                {
                    scrollX = cursorPixelX - this->w + 1;
                }
            }
            else
            {
                scrollX = 0;
            }

            if (scrollX < 0)
                scrollX = 0;
            if (textureW > this->w && scrollX > textureW - this->w)
                scrollX = textureW - this->w;
        }

        int getCursorPixelPosition()
        {
            if (this->cursorPosition == 0)
                return 0;

            TTF_Font *font = ResourceManager::getInstance().getFont(this->path, this->fontSize);

            if (!font)
            {
                Proton::Log("какого хуя у тебя cursorPosition больше 0, если шрифта нет");
                return 0;
            }

            std::string textBeforeCursor = this->labelText.substr(0, this->cursorPosition);
            int width, height;
            if (TTF_GetStringSize(font, textBeforeCursor.c_str(), textBeforeCursor.length(), &width, &height))
            {
                return width;
            }
            return 0;
        }
    };
};