#pragma once

#include "Bullet.h"
#include<raylib.h>
#include<vector>
using namespace std;

class LaserBullet : public Bullet {
private:
    Texture2D* frames[4];  // Holds the animation textures
    int currentFrame = 0;           // Current frame index
    float frameTimer = 0.0f;        // Timer to control frame rate
    float frameDelay;         // Time in seconds for each frame
    int loopCount = 0;              // Counter for animation loops
    int maxLoops;             // The animation will play 3 times

public:
    // Constructor takes position and the vector of animation textures
    LaserBullet(Vector2 pos, Texture2D* laserFrames);
    
    // Override base class methods to implement custom animation logic
    void Update() override;
    void Draw() override;
    Rectangle getRect() const override;
};
