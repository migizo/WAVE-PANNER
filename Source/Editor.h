/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Knob.h"
#include "ExtendToggle.h"
#include "AnimatorByCount.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment KnobAttachment;

//[/Headers]

#include "WaveView.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Editor  : public juce::AudioProcessorEditor,
                private juce::Timer,
                public juce::Button::Listener
{
public:
    //==============================================================================
    Editor (WAVEPANNERAudioProcessor& p,  juce::AudioProcessorValueTreeState& vts);
    ~Editor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* bg_png;
    static const int bg_pngSize;
    static const char* extendToggle_png;
    static const int extendToggle_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void timerCallback() override;

    WAVEPANNERAudioProcessor& processor;
    Knob knob;

    juce::AudioProcessorValueTreeState& valueTreeState;
    std::unique_ptr<KnobAttachment> defaultAttachment;
    std::unique_ptr<KnobAttachment> speedAttachment;
    std::unique_ptr<KnobAttachment> mixAttachment;
    std::unique_ptr<KnobAttachment> curveAttachment;
    std::unique_ptr<KnobAttachment> offsetAttachment;

    // extendToggle
    ExtendToggle extendToggleLookAndFeel;
    double defaultHeight = 537.0;
    double extendHeight = 660.0;
    AnimatorByCount animator;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<WaveView> waveView;
    std::unique_ptr<juce::Slider> defaultKnob;
    std::unique_ptr<juce::Slider> speedKnob;
    std::unique_ptr<juce::Slider> mixKnob;
    std::unique_ptr<juce::Slider> curveKnob;
    std::unique_ptr<juce::Slider> offsetKnob;
    std::unique_ptr<juce::ImageButton> extendToggle;
    juce::Image cachedImage_bg_png_1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

