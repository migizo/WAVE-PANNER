/*
  ==============================================================================

    WavePanner.h
    Created: 12 Mar 2021 8:54:02pm
    Author:  migizo    
    
    countMaxは1小節分のsample

  ==============================================================================
*/

#pragma once
#include <cmath>
class WavePanner {
    
public:
    void update() {
        
        // reset
        if (isInitialized) {
            isInitialized = false;
            count = 0.0;
        }
        
        const double duration = countMax / 5.0;
        if (abs(destOffset - offset) > 1E-4) offset += (destOffset - offset) / duration;
        double n = offset + ((double)count / countMax);
        
        double sinValue = sin(n * juce::MathConstants<double>::twoPi);
        // tri or sin
        if (curve <= 1.0) {
            double fract = fmod(n + 0.25, 1.0);
            double triValue = (fract < 0.5) ? (fract * 2.0) : (1.0 - (fract - 0.5) * 2.0);
            triValue = juce::jmap(triValue, 0.0, 1.0, -1.0, 1.0);
            pan = juce::jmap(curve, 0.0, 1.0, triValue, sinValue);
        }
        // sin or rect
        else {
            double rectValue = pow(abs(sinValue), 1.0/(curve*8.0)) * (sinValue < 0.0 ? -1.0 : 1.0);
            pan = juce::jmap(curve, 1.0, 2.0, sinValue, rectValue);
        }
        pan = juce::jmap(mix, 0.0, 1.0, defaultPan, pan);
        
        // update panList
        const int PATH_RES = 130 * 2; // GUI height: 130
        const double ADJ_STEP = 32.0;
        const int SKIP_STEP = (int)(countMax / (4.0 * ADJ_STEP)); // 処理軽減で調整値*1拍分ごとにpathバッファ保存

        if (countForPushPanList == 0) panList.push_front(pan);
        countForPushPanList = (countForPushPanList + 1) % SKIP_STEP;

        if (panList.size() > PATH_RES)panList.resize((int)PATH_RES);
        if (panList.size() < PATH_RES) {
            const int startSize = (int)panList.size();
            for (int i = startSize; i < PATH_RES; i++) {
                panList.push_front(pan);
            }
        }
        
        count = fmod(count + 1.0 * speed, countMax);
    }
    
    void clear() {
        isInitialized = true;
    }

    // setter
    void setDefaultPan(double _defaultPan){this->defaultPan = _defaultPan;}
    void setMix(double _mix){this->mix = _mix / 100.0;}
    void setSpeed(double _speed){this->speed = _speed;}
    void setCurve(double _curve){this->curve = _curve;}
    void setOffset(double _offset){
        this->destOffset = _offset;
    }
    void setCountMax(double _countMax){this->countMax = _countMax;}
    void setCount(int _count) {
        count = fmodf(_count, countMax);
    }
    // getter
    double getLeftVolume() {return juce::jmap(pan, -1.0, 1.0, 1.0, 0.0); }
    double getRightVolume() {return juce::jmap(pan, -1.0, 1.0, 0.0, 1.0); }
    double getPan(){return pan;}
    const std::deque<double>& getPanList() {return panList;}
private:
    bool isInitialized = false;
    double pan = 0.0; // -1.0: Left, 0: Center, 1: Right
    double speed = 1.0;
    double curve = 1.0;
    double mix = 1.0;
    double defaultPan = 0.0; // -1: Left, 0: Center, 1: Right
    double countMax = 44100.0; // 1000は仮
    std::deque<double> panList;
    
    double offset = 0.0;
    double destOffset = 0.0;
    double count = 0.0;
    int countForPushPanList = 0;
};
