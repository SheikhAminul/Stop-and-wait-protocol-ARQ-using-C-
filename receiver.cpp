#include<iostream>
#include<bits/stdc++.h>
#include<windows.h>
#include<time.h>
#include<string>
#include<conio.h>
using namespace std;

int frameSize = 64;
int transmissionInterval = 2500;
int lastFrame = 1;

int getch_noblock() {
    if (_kbhit()) return _getch();
    else return -1;
}

string encodeSignal(string stringData) {
    string bitsetData = "";
    for(int i = 0; i < stringData.length(); i++) {
        bitsetData = bitsetData + bitset<8>(stringData[i]).to_string();
    }
    return bitsetData;
}

string decodeSignal(string bitsetData) {
    string stringData = "";
    for(int i = 0; i < bitsetData.length(); i = i + 8) {
        stringData = stringData + char(bitset<8>(bitsetData.substr(i, 8)).to_ulong());
    }
    return stringData;
}

void sendACK(string currentFrame) {
    cout << "Sending ACK " << lastFrame << " (Press any key to stop.)" << endl;
    Sleep(250);
    if(getch_noblock() != -1) {
        cout << "(Stopped sending ACK " << currentFrame << ")" << endl;
    } else {
        string frameData = encodeSignal((currentFrame.compare("1") != -1) ? "0" : "1");
        ofstream transmission("transmission.txt");
        transmission << frameData;
        transmission.close();
        cout << "ACK " << lastFrame << " sent!\n" << endl;
    }
}

string fullBitsetData = "";
void getFrame() {
    Sleep(250);
    ifstream transmission("transmission.txt");
    string frameData, bitsetData, currentFrame;
    getline(transmission, frameData);
    transmission.close();
    if(frameData.compare("") == 0) return;
    currentFrame = decodeSignal(frameData.substr(0, 8));
    bitsetData = frameData.substr(8, frameSize);
    if((currentFrame.compare("0") != -1 || currentFrame.compare("1") != -1) && currentFrame.length() != 0) {
        if(bitsetData.compare("") == 0) return;
        int currentFrameNo = atoi(currentFrame.c_str());
        if(currentFrameNo == lastFrame) {
            cout << "Frame " << currentFrameNo << " discarded!\n" << endl;
            sendACK(currentFrame);
            lastFrame = currentFrameNo;
        } else {
            cout << "Frame " << currentFrameNo << " accepted!" << endl;
            fullBitsetData = fullBitsetData + bitsetData;
            sendACK(currentFrame);
            lastFrame = currentFrameNo;
        }
    } else if(currentFrame.compare("*") != -1) {
        cout << "(EOT) End of transmission." << endl;
        cout << "Received (decoded) data: " << decodeSignal(fullBitsetData) << endl;
        cout << "Received bitset data: " << fullBitsetData << endl << endl;
        ofstream transmission("transmission.txt");
        transmission << "";
        transmission.close();
        fullBitsetData = "";
        lastFrame = 1;
    }
}

int main() {
    while(TRUE) {
        getFrame();
    }
}
