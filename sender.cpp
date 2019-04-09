#include<iostream>
#include<bits/stdc++.h>
#include<windows.h>
#include<time.h>
#include<string>
#include<conio.h>
using namespace std;

int frameSize = 64;
int transmissionInterval = 2500;
int currentFrame = 0;

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

int checkACK() {
    ifstream transmission("transmission.txt");
    string ACKData, currentACK;
    getline(transmission, ACKData);
    transmission.close();
    currentACK = decodeSignal(ACKData.substr(0, 8));
    if(currentACK.compare("0") != -1 || currentACK.compare("1") != -1) {
        int currentACKNo = atoi(currentACK.c_str());
        cout << "Received ACK " << currentACKNo << "." << endl;
        return currentACKNo;
    } else return -1;
}

void sendFrame(string bitsetData) {
    cout << "\nSending frame " << currentFrame << " (Press any key to stop.)" << endl;
    Sleep(transmissionInterval);
    if(getch_noblock() != -1) {
        cout << "(Stopped sending frame " << currentFrame << ")" << endl;
    } else {
        string frameData = encodeSignal((currentFrame == 0) ? "0" : "1") + bitsetData;
        ofstream transmission("transmission.txt");
        transmission << frameData;
        transmission.close();
    }
    Sleep(transmissionInterval);
    int ACKNo = checkACK();
    if(ACKNo == -1 || ACKNo == currentFrame) sendFrame(bitsetData);
    else currentFrame = (currentFrame == 0) ? 1 : 0;
}

void EOT() {
    ofstream transmission("transmission.txt");
    transmission << encodeSignal("*");
    transmission.close();
    cout << "\n(EOT) End of transmission.\n" << endl;
}

void transmitData(string data) {
    string encodedData;
    encodedData = encodeSignal(data);

    cout << "\nEncoded data size: " << encodedData.length() << endl;
    cout << "Total frames calculated: " << encodedData.length() / frameSize + 1 << endl;

    currentFrame = 0;
    for(int i = 0; i < encodedData.length(); i = i + frameSize) {
        sendFrame(encodedData.substr(i, frameSize));
    }
    EOT();
}

int main() {
    while(TRUE) {
        string data, encodedData;
        cout << "Enter data to send: ";
        getline(cin, data);
        transmitData(data);
    }
}
