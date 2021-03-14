/*
  ==============================================================================

    WaveView.h
    Created: 12 Mar 2021 8:04:22pm
    Author:  migizo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <cmath>

class WaveView : public juce::Component, private juce::Timer{
  
    public:
    
    WaveView() {
        startTimerHz(60);
    }
    
    void paint (juce::Graphics& g) override {
        
        double startY = 31.0;
        double endY = getHeight();
        double minX = 0.0;
        double maxX = getWidth();
        
        // rect
        g.setColour(juce::Colours::white);
        double rectX;
        if (! panBuffer.empty()) rectX = juce::jmap(panBuffer[0], -1.0, 1.0, minX, maxX);
        else rectX = (minX + maxX) / 2.0;
        g.drawRect(rectX, 0, 2, 31);
            

        // path
        juce::Path path;

        for (int i = 0; i < panBuffer.size(); i++) {
            double x = juce::jmap(panBuffer[i], -1.0, 1.0, minX, maxX);
            if (i == 0) {
                path.startNewSubPath (juce::Point<float> (x, startY));
            }
            else {
                double y = juce::jmap((double)i, 0.0, (double)(panBuffer.size()-1), startY, endY);
                path.lineTo(juce::Point<float>(x, y));
            }
        }
            
        g.setColour(juce::Colour(70, 115, 170));
        g.strokePath(path, juce::PathStrokeType(1.4));
    }
    
    void setPanList(const std::deque<double> & panList) {
        panBuffer = panList;
    }
    
    private:
    void timerCallback() override
    {
        repaint();
    }
    
    std::deque<double> panBuffer;
};
