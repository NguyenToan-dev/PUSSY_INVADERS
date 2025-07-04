#include "Background.h"
#include<algorithm>
#include<cmath>

Background::Background()
{
    backgroundRotation = .0;
    rotationSpeed = .1;
    backgroundScale = 1.0f;
    backgroundTexture = LoadTexture("image/Background-1.jpg");
    CalculateBackgroundScale();
    backgroundCenter = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

}

Background::~Background() {}

void Background::CalculateBackgroundScale() 
{
    if (backgroundTexture.id == 0) return;
    //float scaleX = (float)GetScreenWidth() / backgroundTexture.width;
    //float scaleY = (float)GetScreenHeight() / backgroundTexture.height;
    //float baseScale = std::max(scaleX, scaleY);
    float diagonal = sqrtf(GetScreenWidth() * GetScreenWidth() + GetScreenHeight() * GetScreenHeight());
    float imageMinDim = std::min((float)backgroundTexture.width, (float)backgroundTexture.height);
    backgroundScale = diagonal / imageMinDim;
}

void Background::DrawRotatingBackground() 
{
    if (backgroundTexture.id == 0) 
    {
        for (int i = 0; i < 100; i++) 
            DrawPixel(GetRandomValue(0, GetScreenWidth()), GetRandomValue(0, GetScreenHeight()), WHITE);
        return;
    }
    Rectangle source = { 0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height };
    float scaledWidth = backgroundTexture.width * backgroundScale;
    float scaledHeight = backgroundTexture.height * backgroundScale;
    Rectangle dest = { backgroundCenter.x, backgroundCenter.y, scaledWidth, scaledHeight };
    Vector2 origin = { scaledWidth * 0.5f, scaledHeight * 0.5f };
    DrawTexturePro(backgroundTexture, source, dest, origin, backgroundRotation, WHITE);
}

void Background::SetRotation(float data) 
{
    backgroundRotation = data;
    return;
}

float Background::GetRotation() const   { return backgroundRotation; }
    
float Background::GetSpeed() const  { return rotationSpeed; }
   
float Background::GetScale() const  { return backgroundScale; }

Texture2D Background::GetTexture() const { return backgroundTexture; }

void Background::SetCenter(float x, float y)
{
    backgroundCenter.x = x;
    backgroundCenter.y = y;
}

Vector2 Background::GetCenter() const { return backgroundCenter; }

void Background::SetScale(float data)
{
    backgroundScale = data;
    return;
}
