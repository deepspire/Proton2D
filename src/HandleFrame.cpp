//
// Created by Monsler on 06.07.2025.
//

#include "HandleFrame.h"

#include <utility>

namespace Bionic {
    HandleFrame::HandleFrame(std::string title, const int width, const int height): windowWidth(width), windowHeight(height), windowTitle(std::move(title)) {
    }
} // Bionic