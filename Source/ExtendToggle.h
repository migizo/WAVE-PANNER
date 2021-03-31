/*
  ==============================================================================

    ExtendToggle.h
    Created: 12 Mar 2021 9:57:54pm
    Author:  migizo

  ==============================================================================
*/

#pragma once
#include "AnimatorByCount.h"
class ExtendToggle : public juce::LookAndFeel_V4
{
    
public:
    
    void drawImageButton (juce::Graphics& g, juce::Image* img, int imageX, int imageY, int imageW, int imageH, 
                            const juce::Colour& overlayColour, float imageOpacity, juce::ImageButton& imgButton) override
    {
        g.setOpacity(imgButton.isOver() ? 0.8 : 1.0);

        bool state = imgButton.getToggleState();
        
        // 状態が変わったらアニメーション開始
        if (state != lastState) {
            const int requiredFrame = 5;
            const float defaultRadian = 0.0;
            const float extendRadian = juce::MathConstants<double>::halfPi;
            if (state) animator.start(requiredFrame, defaultRadian, extendRadian);
            else animator.start(requiredFrame, extendRadian, defaultRadian);
        }
        animator.update();
        imageW = g.getClipBounds().getWidth();
        imageH = g.getClipBounds().getHeight();

        auto triangleSize = 12;
        auto centreX = (float) imageW * 0.5f - triangleSize * 0.5f;
        auto centreY = (float) imageH * 0.5f - triangleSize * 0.5f;
        float angle = animator.getValue();
        if (! animator.getIsInitialized()) angle = 0.0;

        auto transform = juce::AffineTransform::translation(centreX, centreY).rotated(angle, imageW * 0.5f, imageH * 0.5f);
    
        img->rescaled(triangleSize, triangleSize);
        g.drawImageTransformed (*img, transform);
        
        
        lastState = state;
    }
    
private:

    bool lastState = false;
    AnimatorByCount animator;
};
