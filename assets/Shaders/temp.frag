#version 330 core

out vec4 FragColor;

uniform vec2 screenSize;


//--------------------------------------------------- random value

float RandomValue(inout uint state)
{
    state *= (state + 195439u)
           * (state + 124395u)
           * (state + 845921u);

    return float(state) / 4294967295.0;
}


//------------------------------------------------------------ main

void main()
{
    uvec2 pixelCoords = uvec2(gl_FragCoord.xy);
    uvec2 numPixels   = uvec2(screenSize);

    uint pixelIndex = pixelCoords.y * numPixels.x
                    + pixelCoords.x;

    uint rngState = pixelIndex;

    float randomValue = RandomValue(rngState);

    FragColor = vec4(randomValue);
}
