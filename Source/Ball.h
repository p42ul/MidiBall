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

const int defaultWidth = 400;
const int defaultHeight = 300;

struct Ball
{
    float x;
    float y;
    float dx;
    float dy;
    float radius;
    int note;
    juce::Colour color;
};