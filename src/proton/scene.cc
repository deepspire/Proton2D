#include "proton/scene.hh"
#include "proton/shapes/container.hh"

namespace Proton
{
Scene::Scene(SDL_Renderer *render, SDL_Window *window)
{
    this->render = render;
    this->window = window;
    this->background = Color(255, 255, 255);
}

Scene::~Scene()
{
    this->destroyObjectMassive();
    this->buttons.clear();
}

void Scene::addObject(Shape *shape) { this->objects.push_back(shape); }
void Scene::addButton(ButtonArea *button)
{
    this->buttons.push_back(button);
    this->addObject(button);
}
void Scene::addTextBox(TextBox *textbox)
{
    this->textboxes.push_back(textbox);
    this->addObject(textbox);
}
void Scene::addContainer(Container *container)
{
    this->containers.push_back(container);
    this->addObject(container);
}

// только для того, чтобы после сцены очистить звуки
void Scene::addAudio(Audio *audio) { this->audios.push_back(audio); }

void Scene::clearScene() { this->destroyObjectMassive(); }

void Scene::paint()
{
    for (Shape *shape : this->objects)
    {
        if (shape->getVisible())
        {
            shape->paint(this->render, ROOT_POSITION);
        }
    }
}

void Scene::handleButtonClick(const Point &mPos)
{
    float mX = mPos.x;
    float mY = mPos.y;

    for (ButtonArea *button : this->buttons)
    {
        if ((button->getX() <= mX && mX <= button->getX() + button->getW()) &&
            (button->getY() <= mY && mY <= button->getY() + button->getH()))
        {
            button->onClick();
            button->setIsFocused(true);
        }
    }

    for (Container *container : this->containers)
    {
        if ((container->getX() <= mX && mX <= container->getX() + container->getW()) &&
            (container->getY() <= mY && mY <= container->getY() + container->getH()))
        {
            const float mouseInContentX = mX - container->getX() + container->getScrollX();
            const float mouseInContentY = mY - container->getY() + container->getScrollY();

            for (ButtonArea *button : container->getButtons())
            {
                if ((button->getX() <= mouseInContentX && mouseInContentX <= button->getX() + button->getW()) &&
                    (button->getY() <= mouseInContentY && mouseInContentY <= button->getY() + button->getH()))
                {
                    button->onClick();
                    button->setIsFocused(true);
                }
            }
        }
    }

    TextBox *clickedTextBox = nullptr;
    for (TextBox *textbox : this->textboxes)
    {
        if ((textbox->getX() <= mX &&
             mX <= textbox->getX() + static_cast<float>(textbox->getBoxW())) && // TODO make size as float
            (textbox->getY() <= mY && mY <= textbox->getY() + static_cast<float>(textbox->getH())))
        {
            clickedTextBox = textbox;
            break;
        }
    }

    if (clickedTextBox)
    {
        if (this->focusedTextBox && this->focusedTextBox != clickedTextBox)
        {
            this->focusedTextBox->setFocused(false);
            this->focusedTextBox->setSelectionAnchor(-1);
            this->focusedTextBox->setCursorPosition(0);
        }

        this->focusedTextBox = clickedTextBox;
        this->focusedTextBox->setFocused(true);

        int relativeX = static_cast<int>(mX - clickedTextBox->getX());
        int charIndex = clickedTextBox->getCharIndexAt(relativeX);
        if (relativeX > clickedTextBox->getW() || charIndex >= clickedTextBox->getTextLength())
        {
            charIndex = clickedTextBox->getTextLength();
        }
        clickedTextBox->setCursorPosition(charIndex);
        clickedTextBox->setSelectionAnchor(-1);
    }
    else
    {
        if (this->focusedTextBox)
        {
            this->focusedTextBox->setFocused(false);
            this->focusedTextBox->setCursorPosition(0);
            this->focusedTextBox->setSelectionAnchor(-1);
            this->focusedTextBox = nullptr;
        }
    }
}

void Scene::handleButtonClickEnd(const Point &mPos) const
{
    const float mX = mPos.x;
    const float mY = mPos.y;

    for (const ButtonArea *button : this->buttons)
    {
        if ((button->getX() <= mX && mX <= button->getX() + button->getW()) &&
            (button->getY() <= mY && mY <= button->getY() + button->getH()) || button->getIsFocused())
        {
            button->onClickEnded();
        }
    }

    for (Container *container : this->containers)
    {
        if ((container->getX() <= mX && mX <= container->getX() + container->getW()) &&
            (container->getY() <= mY && mY <= container->getY() + container->getH()))
        {
            for (ButtonArea *button : container->getButtons())
            {
                const float buttonAbsX = container->getX() + button->getX();
                const float buttonAbsY = container->getY() + button->getY();

                if ((buttonAbsX <= mX && mX <= buttonAbsX + button->getW()) &&
                    (buttonAbsY <= mY && mY <= buttonAbsY + button->getH()) || button->getIsFocused())
                {
                    button->onClickEnded();
                }
            }
        }
    }
}

void Scene::handleKeyDown(SDL_Event event) const
{
    if (this->focusedTextBox != nullptr)
    {
        if ((event.key.mod & SDL_KMOD_CTRL))
        {
            if (event.key.key == SDLK_V)
            {
                if (this->focusedTextBox->getSelectionAnchor() != -1 &&
                    this->focusedTextBox->getSelectionAnchor() != this->focusedTextBox->getCursorPosition())
                {
                    this->focusedTextBox->removeAtCursor();
                }
                char *clipboardText = SDL_GetClipboardText();
                if (clipboardText)
                {
                    this->focusedTextBox->insertSymbol(clipboardText);
                    SDL_free(clipboardText);
                }
                return;
            }
            else if (event.key.key == SDLK_C)
            {
                if (this->focusedTextBox->getSelectionAnchor() != -1)
                {
                    int selStart =
                        std::min(this->focusedTextBox->getSelectionAnchor(), this->focusedTextBox->getCursorPosition());
                    int selEnd =
                        std::max(this->focusedTextBox->getSelectionAnchor(), this->focusedTextBox->getCursorPosition());
                    if (selStart < selEnd)
                    {
                        std::string selectedText = this->focusedTextBox->getText().substr(selStart, selEnd - selStart);
                        SDL_SetClipboardText(selectedText.c_str());
                    }
                }
                return;
            }
            else if (event.key.key == SDLK_X)
            {
                if (this->focusedTextBox->getSelectionAnchor() != -1)
                {
                    int selStart =
                        std::min(this->focusedTextBox->getSelectionAnchor(), this->focusedTextBox->getCursorPosition());
                    int selEnd =
                        std::max(this->focusedTextBox->getSelectionAnchor(), this->focusedTextBox->getCursorPosition());
                    if (selStart < selEnd)
                    {
                        std::string selectedText = this->focusedTextBox->getText().substr(selStart, selEnd - selStart);
                        SDL_SetClipboardText(selectedText.c_str());
                        this->focusedTextBox->removeAtCursor();
                    }
                }
                return;
            }
            else if (event.key.key == SDLK_A)
            {
                this->focusedTextBox->setSelectionAnchor(0);
                this->focusedTextBox->setCursorPosition(this->focusedTextBox->getTextLength());
                return;
            }
        }

        bool shiftPressed = (event.key.mod & SDL_KMOD_SHIFT) != 0;
        switch (event.key.key)
        {
        case SDLK_HOME:
            if (shiftPressed && this->focusedTextBox->getSelectionAnchor() == -1)
                this->focusedTextBox->setSelectionAnchor(this->focusedTextBox->getCursorPosition());
            else if (!shiftPressed)
                this->focusedTextBox->setSelectionAnchor(-1);
            this->focusedTextBox->setCursorPosition(0);
            break;
        case SDLK_END:
            if (shiftPressed && this->focusedTextBox->getSelectionAnchor() == -1)
                this->focusedTextBox->setSelectionAnchor(this->focusedTextBox->getCursorPosition());
            else if (!shiftPressed)
                this->focusedTextBox->setSelectionAnchor(-1);
            this->focusedTextBox->setCursorPosition(this->focusedTextBox->getTextLength());
            break;
        case SDLK_LEFT:
            if (shiftPressed && this->focusedTextBox->getSelectionAnchor() == -1)
                this->focusedTextBox->setSelectionAnchor(this->focusedTextBox->getCursorPosition());
            else if (!shiftPressed)
                this->focusedTextBox->setSelectionAnchor(-1);
            this->focusedTextBox->appendCursorLeft();
            break;
        case SDLK_RIGHT:
            if (shiftPressed && this->focusedTextBox->getSelectionAnchor() == -1)
                this->focusedTextBox->setSelectionAnchor(this->focusedTextBox->getCursorPosition());
            else if (!shiftPressed)
                this->focusedTextBox->setSelectionAnchor(-1);
            this->focusedTextBox->appendCursorRight();
            break;
        case SDLK_BACKSPACE:
            this->focusedTextBox->removeAtCursor();
            break;
        default:
            break;
        }
    }
}

void Scene::handleMouseWheel(SDL_Event event)
{
    float mX, mY;
    SDL_GetMouseState(&mX, &mY);

    const float scrollSpeed = 15.0f;
    auto wheelY = static_cast<float>(event.wheel.y);

    for (Container *container : this->containers)
    {
        if ((container->getX() <= mX && mX <= container->getX() + container->getW()) &&
            (container->getY() <= mY && mY <= container->getY() + container->getH()))
        {
            container->scrollBy(0, -wheelY * scrollSpeed);
            break;
        }
    }
}
std::vector<Container *> Scene::getContainers()
{
    return containers;
}
void Scene::handleMouseDrag(int mX, int mY)
{
    if (this->focusedTextBox)
    {
        if (this->focusedTextBox->getSelectionAnchor() == -1)
        {
            this->focusedTextBox->setSelectionAnchor(this->focusedTextBox->getCursorPosition());
        }

        int relativeX = static_cast<int>(static_cast<float>(mX) - focusedTextBox->getX());
        if (relativeX < 0)
        {
            focusedTextBox->setCursorPosition(0);
        }
        else if (relativeX > focusedTextBox->getW())
        {
            focusedTextBox->setCursorPosition(focusedTextBox->getTextLength());
        }
        else
        {
            int charIndex = focusedTextBox->getCharIndexAt(relativeX);
            focusedTextBox->setCursorPosition(charIndex);
        }
    }
}

void Scene::handleTextInput(SDL_Event event)
{
    if (this->focusedTextBox != nullptr)
    {
        this->focusedTextBox->insertSymbol(event.text.text);
    }
}

void Scene::addBody(PhysicsBody *body) { this->physicsBodies.push_back(body); }

auto Scene::getPhysicsBodies() -> std::vector<PhysicsBody *> { return this->physicsBodies; }

auto Scene::getBackgroundColor() const -> Color { return this->background; }

void Scene::destroyObjectMassive()
{
    for (Shape *shape : this->objects)
    {
        delete shape;
    }
    this->objects.clear();

    for (Audio *audio : this->audios)
    {
        delete audio;
    }
    this->audios.clear();

    for (PhysicsBody *body : physicsBodies)
    {
        delete body;
    }
    physicsBodies.clear();
}
}; // namespace Proton