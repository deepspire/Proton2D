//
// Created by Monsler on 06.07.2025.
//

#ifndef HANDLEFRAME_H
#define HANDLEFRAME_H
#include <string>

namespace Bionic {

class HandleFrame {
public:
    explicit HandleFrame(std::string title, int width, int height);
private:
    int windowWidth, windowHeight;
    std::string windowTitle;
};

} // Bionic

#endif //HANDLEFRAME_H
