#pragma once
#include "../logman.hh"
#include "shape.hh"

namespace Proton
{
    class ProgressBar : public Shape
    {
        public:
        explicit ProgressBar(const float x=0, const float y=0, const int width=100, const int height=20, const int defaultProgress=100)
        {
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
            this->progress = defaultProgress;
        }

        void setPosition(const float x, const float y) override
        {
            this->x = x;
            this->y = y;
        }

        void setFillColor(const Color newColor) override
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
                Log("Progress value can't be lower than 0 or greater than 100! You're trying to set value: ", progress);
            }
        }
        
        [[nodiscard]] int getProgress() const {
            return this->progress;
        }

        void paint(SDL_Renderer* render, const int pX, const int pY) override {
            const float progressWidth = (static_cast<float>(progress)/100.0f)*this->width-4;
            const SDL_FRect bgRect = {(this->x+pX), (this->y+pY), static_cast<float>(this->width), static_cast<float>(this->height)};
            const SDL_FRect progressRect = {(this->x+pX+2), (this->y+pY+2), static_cast<float>(progressWidth), static_cast<float>(this->height-4)};
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