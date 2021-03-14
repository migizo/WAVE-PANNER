/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "Editor.h"

//==============================================================================
WAVEPANNERAudioProcessor::WAVEPANNERAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
}

WAVEPANNERAudioProcessor::~WAVEPANNERAudioProcessor()
{
}

//==============================================================================
const juce::String WAVEPANNERAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WAVEPANNERAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WAVEPANNERAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WAVEPANNERAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WAVEPANNERAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WAVEPANNERAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WAVEPANNERAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WAVEPANNERAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WAVEPANNERAudioProcessor::getProgramName (int index)
{
    return {};
}

void WAVEPANNERAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WAVEPANNERAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void WAVEPANNERAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WAVEPANNERAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

/*
#ifndef JucePlugin_PreferredChannelConfigurations
bool WAVEPANNERAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    const juce::AudioChannelSet& mainInput  = layouts.getMainInputChannelSet();
    const juce::AudioChannelSet& mainOutput = layouts.getMainOutputChannelSet();

    return (mainInput.size() == 1 && mainOutput.size() == 2);
#endif
}
#endif
*/
void WAVEPANNERAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::AudioPlayHead::CurrentPositionInfo info;
    getPlayHead()->getCurrentPosition(info);
    
    double secPer1Bar = (60.0 / info.bpm) * 4.0;
    double countMax = secPer1Bar * getSampleRate(); // 1小節で1周期(sec to sample)
    wavePanner.setCountMax(countMax);
    
    static bool lastIsPlaying = false;
    bool isPlaying = info.isPlaying;
    if (!lastIsPlaying && isPlaying) {
        wavePanner.setCount(info.timeInSamples);
    }
    lastIsPlaying = isPlaying;

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const int bufferSize = buffer.getNumSamples();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // left buffer -> copy -> right buffer
    buffer.copyFrom(1, 0, buffer, 0, 0, bufferSize); // destCh, dstStartSample, buf, srcCh, srcStartSample, bufSize
    for (int i = 0; i < bufferSize; i++) {
        wavePanner.update();

        float* lBuf = buffer.getWritePointer(0);
        float* rBuf = buffer.getWritePointer(1);

        lBuf[i] *= wavePanner.getLeftVolume();
        rBuf[i] *= wavePanner.getRightVolume();            
    }
}

//==============================================================================
bool WAVEPANNERAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WAVEPANNERAudioProcessor::createEditor()
{
    return new Editor (*this);
}

//==============================================================================
void WAVEPANNERAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WAVEPANNERAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WAVEPANNERAudioProcessor();
}
