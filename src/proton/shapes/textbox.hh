#include "text.hh"
#include "../logman.hh"

namespace Proton
{
    class TextBox : public Text
    {
    public:
        TextBox(SDL_Renderer *renderer, SDL_Window *window, const std::string &text = "Label", int x = 0,
                int y = 0, const std::string &fontPath = "fonts/Roboto-Regular.ttf",
                int fontSize = 10, Color color = Color(255, 255, 255, 255),
                int w = 20, int h = 20) : Text(renderer, text, x, y, fontPath, fontSize, color)
        {
            this->window = window;
            this->resize(w, h);
        }

        bool isFocused()
        {
            return this->focused;
        }

        void setFocused(bool v)
        {
            this->focused = v;

            Log("is focused: ", v);

            if (v)
            {
                SDL_StartTextInput(this->window);
            } else {
                SDL_StopTextInput(this->window);
            }
        }

    private:
        bool focused = false;
        SDL_Window *window;
    };
};