#include "proton/shapes/imagebutton.hh"

namespace Proton
{
ImageButton::ImageButton(SDL_Texture *texture, const float x, const float y, const float w, const float h)
    : ButtonArea(x, y, w, h)
{
    this->image = new Image(texture, x, y, w, h);
}
ImageButton::~ImageButton() { delete this->image; }

void ImageButton::setRotation(const float angle) { this->image->setRotation(angle); }

void ImageButton::setPosition(const float x, const float y)
{
    this->position.x = x;
    this->position.y = y;
    image->setPosition(x, y);
}

void ImageButton::resize(const float w, const float h)
{
    this->w = w;
    this->h = h;
    image->resize(w, h);
}

void ImageButton::paint(SDL_Renderer *render, const float rX, const float rY) { image->paint(render, rX, rY); }
} // namespace Proton