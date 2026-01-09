//
// Created by Blokki on 2025/12/17.
//

#include "Game/Gameplay.h"
#include "Tools/Timer.h"

int main() {
    // Initialize timer
    timer::initialize();
    
    // Run the gameplay function
    Game::Gameplay();
    
    // Cleanup timer
    timer::shutdown();
    
    return 0;
}
