#include <iostream>
#include "shape.hh"
#include <vector>

namespace Proton
{
    class Group : public Shape
    {
        public:
        Group(int x, int y)
        {
            this->x = x;
            this->y = y;
            this->isVisible = true;
        }

        void paint(SDL_Renderer* render, int gX, int gY) override
        {
            for (Shape* shape: this->shapes)
            {
                shape->paint(render, this->x + gX, this->y + gY);
            }
        }

        void setFillColor(Color /*unused*/) override
        {

        }

        void update(float dt) override
        {
            for (Shape *shape : this->shapes)
            {
                shape->update(dt);
            }
        }

        void setPosition(int x, int y) override
        {
            this->x = x;
            this->y = y;
        }

        void addObject(Shape* shape)
        {
            shapes.push_back(shape);
        }

        void clear()
        {
            for(Proton::Shape* shape: this->shapes)
            {
                delete shape;
            }
        }

        ~Group() {
            clear();
        }

        protected:

        std::vector<Proton::Shape*> shapes;  
    };
}