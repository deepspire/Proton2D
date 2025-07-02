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
        TextBox(SDL_Window *window, int boxW, const std::string &text = "Label", int x = 0,
                int y = 0, const std::string &fontPath = "fonts/Roboto-Regular.ttf",
                int fontSize = 10, Color color = Color(255, 255, 255, 255)) : Text(text, x, y, fontPath, fontSize, color)
        {
            this->window = window;
            this->boxW = boxW;
            this->scrollX = 0;
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

        int getSelectionAnchor()
        {
            return this->selectionAnchor;
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
            if (this->selectionAnchor != -1 && this->selectionAnchor != this->cursorPosition)
            {
                int selStart = std::min(this->selectionAnchor, this->cursorPosition);
                int selEnd = std::max(this->selectionAnchor, this->cursorPosition);
                this->labelText.erase(selStart, selEnd - selStart);
                this->cursorPosition = selStart;
                this->selectionAnchor = -1;
            }
            this->labelText.insert(this->cursorPosition, symbol);
            size_t sizeNewBytes = strlen(symbol);
            this->setCursorPosition(this->cursorPosition + sizeNewBytes);
            this->setText(this->labelText, true);
        }

        void removeAtCursor()
        {
            if (this->selectionAnchor != -1 && this->selectionAnchor != this->cursorPosition)
            {
                int selStart = std::min(this->selectionAnchor, this->cursorPosition);
                int selEnd = std::max(this->selectionAnchor, this->cursorPosition);
                this->labelText.erase(selStart, selEnd - selStart);
                this->setCursorPosition(selStart);
                this->selectionAnchor = -1;
            }
            else if (this->cursorPosition > 0 && !this->labelText.empty())
            {
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
                    }
                }
            }
            this->setText(this->labelText, true);
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
            if (this->currentBlinkTime >= this->requiredBlinkTime && this->focused)
            {
                this->currentBlinkTime = 0;
                this->cursorVisible = !this->cursorVisible;
            }
        }

        void paint(SDL_Renderer *render, int rX, int rY) override
        {
            if (this->isDirty)
                this->createTexture(render);
            if (!this->isVisible)
                return;

            float box_width = (float)this->boxW;
            float box_height = (float)this->getH();

            SDL_SetRenderDrawColor(render, 200, 200, 200, 255);
            SDL_FRect bgRect = {(float)(this->x + rX), (float)(this->y + rY), box_width, box_height};
            SDL_RenderFillRect(render, &bgRect);

            if (this->textTexture)
            {
                float fullTextWidth = (float)this->w;
                float fullTextHeight = (float)this->h;

                SDL_FRect srcRect = {(float)this->scrollX, 0.0f, box_width, fullTextHeight};
                if (fullTextWidth - srcRect.x < box_width)
                {
                    srcRect.w = fullTextWidth - srcRect.x;
                }
                if (srcRect.w < 0)
                    srcRect.w = 0;

                SDL_FRect destRect = {(float)(this->x + rX), (float)(this->y + rY), srcRect.w, srcRect.h};

                if (this->selectionAnchor != -1)
                {
                    int selStart = std::min(this->selectionAnchor, this->cursorPosition);
                    int selEnd = std::max(this->selectionAnchor, this->cursorPosition);
                    if (selStart < selEnd)
                    {
                        int selStartPixel = getPixelPosition(selStart);
                        int selEndPixel = getPixelPosition(selEnd);
                        float visibleSelStart = std::max((float)selStartPixel, (float)scrollX);
                        float visibleSelEnd = std::min((float)selEndPixel, (float)(scrollX + boxW));
                        if (visibleSelStart < visibleSelEnd)
                        {
                            float drawX = (float)(this->x + rX) + (visibleSelStart - scrollX);
                            float drawY = (float)(this->y + rY);
                            float drawW = visibleSelEnd - visibleSelStart;
                            float drawH = (float)this->h;
                            SDL_FRect selRect = {drawX, drawY, drawW, drawH};
                            SDL_SetRenderDrawColor(render, 173, 216, 230, 255);
                            SDL_RenderFillRect(render, &selRect);
                        }
                    }
                }

                SDL_RenderTexture(render, this->textTexture, &srcRect, &destRect);
            }

            if (this->focused && this->cursorVisible)
            {
                TTF_Font *font = ResourceManager::getInstance().getFont(this->path, this->fontSize);
                if (font)
                {
                    int fontHeight = TTF_GetFontHeight(font);
                    int cursorPixelX = getCursorPixelPosition();
                    float cursorDrawX = (float)(this->x + rX + cursorPixelX - this->scrollX);

                    if (cursorDrawX >= (this->x + rX) && cursorDrawX <= (this->x + rX + this->boxW))
                    {
                        SDL_SetRenderDrawColor(render, this->fillColor.getR(), this->fillColor.getG(), this->fillColor.getB(), this->fillColor.getA());
                        SDL_RenderLine(render, cursorDrawX, (float)(this->y + rY), cursorDrawX, (float)(this->y + rY + fontHeight));
                    }
                }
            }
        }

        int getH() const override
        {
            if (this->h > 0)
            {
                return this->h;
            }
            else
            {
                return this->getFontHeight();
            }
        }

        int getBoxW() const
        {
            return this->boxW;
        }

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

        void adjustScrollX()
        {
            TTF_Font *font = ResourceManager::getInstance().getFont(this->path, this->fontSize);
            if (!font)
            {
                this->scrollX = 0;
                return;
            }

            int fullTextWidth = 0;
            TTF_GetStringSize(font, this->labelText.c_str(), this->labelText.length(), &fullTextWidth, nullptr);

            int cursorPixelX = getCursorPixelPosition();

            if (fullTextWidth <= this->boxW)
            {
                this->scrollX = 0;
                return;
            }

            if (cursorPixelX < this->scrollX)
            {
                this->scrollX = cursorPixelX;
            }
            else if (cursorPixelX > this->scrollX + this->boxW)
            {
                this->scrollX = cursorPixelX - this->boxW;
            }

            if (this->scrollX < 0)
            {
                this->scrollX = 0;
            }

            if (this->scrollX > fullTextWidth - this->boxW)
            {
                this->scrollX = fullTextWidth - this->boxW;
            }
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

        int getPixelPosition(int charIndex)
        {
            if (charIndex == 0)
                return 0;
            if (charIndex > (int)labelText.length())
                return getPixelPosition(labelText.length());
            TTF_Font *font = ResourceManager::getInstance().getFont(this->path, this->fontSize);
            if (!font)
                return 0;
            std::string textBefore = this->labelText.substr(0, charIndex);
            int width;
            TTF_GetStringSize(font, textBefore.c_str(), textBefore.length(), &width, nullptr);
            return width;
        }
    };
};