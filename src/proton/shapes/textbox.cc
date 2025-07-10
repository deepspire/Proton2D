#include "proton/shapes/textbox.hh"
#include "proton/resourcemanager.hh"

namespace Proton
{
TextBox::TextBox(SDL_Window *window, const int boxW, const std::string &text, const float x, const float y,
                 const std::string &fontPath, const int fontSize, const Color color)
    : Text(text, x, y, fontPath, fontSize, color)
{
    this->window = window;
    this->boxW = boxW;
    this->scrollX = 0;
}

void TextBox::setFocused(const bool v)
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

void TextBox::setCursorPosition(const unsigned int v)
{
    if (v > this->labelText.length())
    {
        Log("attempt to set cursor position > than label text length");
    }
    else
    {
        this->currentBlinkTime = 0;
        this->cursorVisible = true;
        this->cursorPosition = static_cast<int>(v);
        this->adjustScrollX();
    }
}

void TextBox::setSelectionAnchor(const int v)
{
    if (v == -1)
    {
        this->selectionAnchor = -1;
        return;
    }

    if (v < 0 || v > static_cast<int>(this->labelText.length()))
    {
        Log("attempt to set selection anchor position > than label text length");
    }
    else
    {
        this->selectionAnchor = v;
    }
}

void TextBox::appendCursorLeft()
{
    if (this->cursorPosition > 0)
    {
        size_t currentPos = this->cursorPosition;
        while (currentPos > 0)
        {
            currentPos--;
            if (const auto firstByte = static_cast<unsigned char>(this->labelText[currentPos]);
                (firstByte & 0xC0) != 0x80)
            {
                if (const int char_len = getCharLength(firstByte);
                    char_len > 0 && (currentPos + char_len <= this->cursorPosition))
                {
                    this->setCursorPosition(currentPos);
                    return;
                }
            }
        }
        this->setCursorPosition(0);
    }
}

void TextBox::appendCursorRight()
{
    if (this->cursorPosition < this->labelText.length())
    {
        const auto firstByte = static_cast<unsigned char>(this->labelText[this->cursorPosition]);
        if (const int char_len = getCharLength(firstByte); char_len > 0)
        {
            this->setCursorPosition(this->cursorPosition + char_len);
        }
    }
}

void TextBox::setText(const std::string &text, const bool dontUpdateCursor)
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

void TextBox::insertSymbol(const char *symbol)
{
    if (this->selectionAnchor != -1 && this->selectionAnchor != this->cursorPosition)
    {
        const int selStart = std::min(this->selectionAnchor, this->cursorPosition);
        const int selEnd = std::max(this->selectionAnchor, this->cursorPosition);
        this->labelText.erase(selStart, selEnd - selStart);
        this->cursorPosition = selStart;
        this->selectionAnchor = -1;
    }
    this->labelText.insert(this->cursorPosition, symbol);
    size_t sizeNewBytes = strlen(symbol);
    this->setCursorPosition(this->cursorPosition + sizeNewBytes);
    this->setText(this->labelText, true);
}

void TextBox::removeAtCursor()
{
    if (this->selectionAnchor != -1 && this->selectionAnchor != this->cursorPosition)
    {
        const int selStart = std::min(this->selectionAnchor, this->cursorPosition);
        const int selEnd = std::max(this->selectionAnchor, this->cursorPosition);
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
            const auto firstByte = static_cast<unsigned char>(this->labelText[scanPos]);
            const int charLen = getCharLength(firstByte);
            if (charLen <= 0)
                return;
            scanPos += charLen;
        }
        if (charStartPos < this->cursorPosition)
        {
            if (const int charLen = getCharLength(static_cast<unsigned char>(this->labelText[charStartPos]));
                charLen > 0 && charStartPos + charLen <= this->labelText.length() && charStartPos + charLen == scanPos)
            {
                this->labelText.erase(charStartPos, charLen);
                this->setCursorPosition(charStartPos);
            }
        }
    }
    this->setText(this->labelText, true);
}

[[nodiscard]] auto TextBox::getCharIndexAt(const int pX) const -> int
{
    const int targetPixelX = pX + this->scrollX;

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
                return static_cast<int>(charIndex);
            }
            else
            {
                return static_cast<int>(charIndex) + charLen;
            }
        }

        charIndex += charLen;
    }

    return static_cast<int>(this->labelText.length());
}

void TextBox::update(const float dt)
{
    this->currentBlinkTime += dt;
    if (this->currentBlinkTime >= this->requiredBlinkTime && this->focused)
    {
        this->currentBlinkTime = 0;
        this->cursorVisible = !this->cursorVisible;
    }
}

void TextBox::paint(SDL_Renderer *render, const float rX, const float rY)
{
    if (this->isDirty)
        this->createTexture(render);
    if (!this->isVisible)
        return;

    const auto box_width = static_cast<float>(this->boxW);
    const auto box_height = static_cast<float>(this->getH());

    SDL_SetRenderDrawColor(render, 200, 200, 200, 255);
    const SDL_FRect bgRect = {this->position.x + rX, this->position.y + rY, box_width, box_height};
    SDL_RenderFillRect(render, &bgRect);

    if (this->textTexture)
    {
        const auto fullTextWidth = static_cast<float>(this->w);
        const auto fullTextHeight = static_cast<float>(this->h);

        SDL_FRect srcRect = {static_cast<float>(this->scrollX), 0.0f, box_width, fullTextHeight};
        if (fullTextWidth - srcRect.x < box_width)
        {
            srcRect.w = fullTextWidth - srcRect.x;
        }
        if (srcRect.w < 0)
            srcRect.w = 0;

        const SDL_FRect destRect = {this->position.x + rX, this->position.y + rY, srcRect.w, srcRect.h};

        if (this->selectionAnchor != -1)
        {
            const int selStart = std::min(this->selectionAnchor, this->cursorPosition);
            if (const int selEnd = std::max(this->selectionAnchor, this->cursorPosition); selStart < selEnd)
            {
                const int selStartPixel = getPixelPosition(selStart);
                const int selEndPixel = getPixelPosition(selEnd);
                const float visibleSelStart = std::max(static_cast<float>(selStartPixel), static_cast<float>(scrollX));
                const float visibleSelEnd =
                    std::min(static_cast<float>(selEndPixel), static_cast<float>(scrollX + boxW));
                if (visibleSelStart < visibleSelEnd)
                {
                    const float drawX = this->position.x + rX + (visibleSelStart - scrollX);
                    const auto drawY = this->position.y + rY;
                    const float drawW = visibleSelEnd - visibleSelStart;
                    const auto drawH = static_cast<float>(this->h);
                    const SDL_FRect selRect = {drawX, drawY, drawW, drawH};
                    SDL_SetRenderDrawColor(render, 173, 216, 230, 255);
                    SDL_RenderFillRect(render, &selRect);
                }
            }
        }

        SDL_RenderTexture(render, this->textTexture, &srcRect, &destRect);
    }

    if (this->focused && this->cursorVisible)
    {
        if (const TTF_Font *font = ResourceManager::getInstance().getFont(this->path, this->fontSize))
        {
            const int fontHeight = TTF_GetFontHeight(font);
            const int cursorPixelX = getCursorPixelPosition();

            if (const auto cursorDrawX = static_cast<float>(this->position.x + rX + cursorPixelX - this->scrollX);
                cursorDrawX >= (this->position.x + rX) && cursorDrawX <= (this->position.x + rX + this->boxW))
            {
                SDL_SetRenderDrawColor(render, this->fillColor.getR(), this->fillColor.getG(), this->fillColor.getB(),
                                       this->fillColor.getA());
                SDL_RenderLine(render, cursorDrawX, static_cast<float>(this->position.y + rY), cursorDrawX,
                               static_cast<float>(this->position.y + rY + fontHeight));
            }
        }
    }
}

void TextBox::adjustScrollX()
{
    TTF_Font *font = ResourceManager::getInstance().getFont(this->path, this->fontSize);
    if (!font)
    {
        this->scrollX = 0;
        return;
    }

    int fullTextWidth = 0;
    TTF_GetStringSize(font, this->labelText.c_str(), this->labelText.length(), &fullTextWidth, nullptr);

    const int cursorPixelX = getCursorPixelPosition();

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

auto TextBox::getCursorPixelPosition() const -> int
{
    if (this->cursorPosition == 0)
        return 0;

    TTF_Font *font = ResourceManager::getInstance().getFont(this->path, this->fontSize);
    if (!font)
    {
        Log("какого хуя у тебя cursorPosition больше 0, если шрифта нет");
        return 0;
    }

    const std::string textBeforeCursor = this->labelText.substr(0, this->cursorPosition);
    int width, height;
    if (TTF_GetStringSize(font, textBeforeCursor.c_str(), textBeforeCursor.length(), &width, &height))
    {
        return width;
    }
    return 0;
}

auto TextBox::getPixelPosition(const int charIndex) -> int
{
    if (charIndex == 0)
        return 0;
    if (charIndex > static_cast<int>(labelText.length()))
        return getPixelPosition(static_cast<int>(labelText.length()));
    TTF_Font *font = ResourceManager::getInstance().getFont(this->path, this->fontSize);
    if (!font)
        return 0;
    const std::string textBefore = this->labelText.substr(0, charIndex);
    int width;
    TTF_GetStringSize(font, textBefore.c_str(), textBefore.length(), &width, nullptr);
    return width;
}
} // namespace Proton