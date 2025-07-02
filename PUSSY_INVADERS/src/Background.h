#pragma once
#include<raylib.h>

class Background
{
private:
    Texture2D backgroundTexture;
    float backgroundRotation;
    Vector2 backgroundCenter;
    float rotationSpeed;
    float backgroundScale;

public:
    Background();
    ~Background();
    void CalculateBackgroundScale();
    void DrawRotatingBackground();

    void SetRotation(float);
    float GetRotation() const;
    float GetSpeed() const;
    Texture2D GetTexture() const;
    void SetCenter(float, float);
    Vector2 GetCenter() const;
    void SetScale(float);
    float GetScale() const;

};