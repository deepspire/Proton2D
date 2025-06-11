#include "../logman.hh"
#include "shape.hh"

namespace Proton
{
    class ProgressBar : public Shape
    {
        public:
        ProgressBar(int x=0, int y=0, int width=100, int height=20, int defaultProgress=100)
        {
            this->isVisible = true;
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
            this->progress = defaultProgress;
        }

        void setPosition(int x, int y) override
        {
            this->x = x;
            this->y = y;
        }

        void setFillColor(Color newColor) override
        {
            this->progressBarColor = newColor;
        }

        void setProgress(int progress)
        {
            if (progress >= 0 && progress <= 100)
            {
                this->progress = progress;
            } else
            {
                Proton::Log("Progress value can't be lower than 0 and greater than 100! You're trying to set value: ", progress);
            }
        }
        
        int getProgress()
        {
            return this->progress;
        }

        void paint(SDL_Renderer* render, int pX, int pY)
        {
            float progressWidth = (static_cast<float>(progress)/100.0f)*this->width-4;
            SDL_FRect bgRect = {static_cast<float>(this->x+pX), static_cast<float>(this->y+pY), static_cast<float>(this->width), static_cast<float>(this->height)};
            SDL_FRect progressRect = {static_cast<float>(this->x+pX+2), static_cast<float>(this->y+pY+2), static_cast<float>(progressWidth), static_cast<float>(this->height-4)};
            SDL_SetRenderDrawColor(render, progressBarColor.getR() > 0 ? progressBarColor.getR()-50 : 0, progressBarColor.getG() > 0 ? progressBarColor.getG()-50 : 0, progressBarColor.getB() > 0 ? progressBarColor.getB()-50 : 0, progressBarColor.getA());
            SDL_RenderFillRect(render, &bgRect);
            SDL_SetRenderDrawColor(render, progressBarColor.getR(), progressBarColor.getG(), progressBarColor.getB(), progressBarColor.getA());
            SDL_RenderFillRect(render, &progressRect);
        }

        private:
        Color progressBarColor;
        int progress;
        int width, height;
    };
}