/*
  ==============================================================================

    Ball.h
    Created: 22 Nov 2023 8:18:54pm
    Author:  paul
    Simple struct for storing a moving circular ball.
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct Ball
{
    float x;
    float y;
    float speed;
    float radius;
    juce::Colour color;
};