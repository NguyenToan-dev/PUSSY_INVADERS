#include "IntroTypingAudio.h"
#include <math.h>
#include <raylib.h>
#include <stdlib.h>
#include <vector> 

void GenerateKeyClickSound(short* buffer, int samples, float baseFrequency, int sampleRate)
{
    float randomPitch = (rand() % 20 - 10) / 100.0f;
    float frequency = baseFrequency * (1.0f + randomPitch);

    for (int i = 0; i < samples; i++)
    {
        float t = (float)i / sampleRate;
        float value = (fmod(t * frequency, 1.0) < 0.5) ? 1.0f : -1.0f;
        float decay = 1.0f - (float)i / samples;
        buffer[i] = (short)(value * 32000 * decay);
    }
}

Sound generateSimpleSound(int sampleRate, int samples)
{
    // Cấp phát động để buffer sống sau khi hàm kết thúc
    short* buffer = new short[samples];
    GenerateKeyClickSound(buffer, samples, 100.0f, sampleRate);

    Wave wave;
    wave.frameCount = samples;
    wave.sampleRate = sampleRate;
    wave.sampleSize = 16;
    wave.channels = 1;
    wave.data = buffer;

    Sound sound = LoadSoundFromWave(wave);

    delete[] buffer;  // Xóa sau khi LoadSoundFromWave đã copy
    return sound;
}
