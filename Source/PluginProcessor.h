/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WavePanner.h"
//==============================================================================
/**
*/
class WAVEPANNERAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    WAVEPANNERAudioProcessor();
    ~WAVEPANNERAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    WavePanner wavePanner;
private:
    juce::AudioProcessorValueTreeState parameters;
    juce::AudioProcessorValueTreeState::Listener* listener;
    std::atomic<float>* defaultParameter = nullptr;
    std::atomic<float>* speedParameter = nullptr;
    std::atomic<float>* mixParameter = nullptr;
    std::atomic<float>* curveParameter = nullptr;
    std::atomic<float>* offsetParameter = nullptr;
    
    class ParameterListener : public juce::AudioProcessorValueTreeState::Listener {
        public:
        ParameterListener(WAVEPANNERAudioProcessor& _p) : p(_p){}
        void parameterChanged(const juce::String& parameterID, float newValue) override {
            if (parameterID == "DEFAULT") p.wavePanner.setDefaultPan(newValue);
            else if (parameterID == "SPEED") p.wavePanner.setSpeed(newValue);
            else if (parameterID == "MIX") p.wavePanner.setMix(newValue);
            else if (parameterID == "CURVE") p.wavePanner.setCurve(newValue);
            else if (parameterID == "OFFSET") p.wavePanner.setOffset(newValue);

        }
        private:
        WAVEPANNERAudioProcessor& p;
    };
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WAVEPANNERAudioProcessor)
};
