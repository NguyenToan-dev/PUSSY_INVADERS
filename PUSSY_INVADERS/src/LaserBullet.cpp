#include "LaserBullet.h"

// The constructor now initializes the non-const members in the initializer list.
LaserBullet::LaserBullet(Vector2 pos, Texture2D* laserFrames)
    : Bullet(pos, nullptr), frameDelay(0.05f), maxLoops(2) 
{ 
    for(int i=0; i<4; i++)
        frames[i] = &laserFrames[i];
}

void LaserBullet::Update() {
    if (active) {
        // Increment the frame timer based on the time elapsed since the last frame
        frameTimer += GetFrameTime();

        // Check if it's time to switch to the next animation frame
        if (frameTimer >= frameDelay) {
            currentFrame++;
            frameTimer = 0.0f; // Reset the timer

            // Check if we've reached the end of the animation frames
            if (currentFrame >= 4) {
                currentFrame = 0; // Loop back to the first frame
                loopCount++;      // Increment the loop counter

                // Check if the animation has played the required number of times
                if (loopCount >= maxLoops) {
                    active = false; // Deactivate the laser after the loops are complete
                }
            }
        }
    }
}

Rectangle LaserBullet::getRect() const
{
    float scale = 0.6f;
    return {
        position.x - (frames[0]->width * scale) / 2,
        position.y - (frames[0]->height * scale) + 10,
        frames[0]->width * scale,
        frames[0]->height * scale
    };
}

void LaserBullet::Draw() {
    if (active && currentFrame < 4) {
        float scale = 0.6f;
        Vector2 drawPos = {
            position.x - (frames[currentFrame]->width * scale) / 2.0,
            position.y - (frames[currentFrame]->height * scale) + 10
        };
        // Draw the current frame of the animation at the bullet's position
        DrawTextureEx(*(frames[currentFrame]), drawPos, 0.0f, scale, WHITE);
    }
}

