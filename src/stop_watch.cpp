#include "stop_watch.hpp"
#include <chrono>
#include <iostream>

using namespace std;

bool Stopwatch::running = false;
thread Stopwatch::counter;
int Stopwatch::seconds = 0;
int Stopwatch::minutes = 0;
int Stopwatch::hours = 0;

Stopwatch::Stopwatch(){
}

void Stopwatch::start() {
    if (running) {
        return;
    }
    running = true;
    counter = thread(&count);
}

void Stopwatch::stop() {
    running = false;
    counter.join();
}

void Stopwatch::count() {
    while (running) {
        this_thread::sleep_for(chrono::milliseconds(1000));
        if (seconds == 60) {
            seconds = 0;
            minutes++;
        }
        else if (minutes == 60) {
            minutes = 0;
            hours++;
        }
        else {
            seconds++;
        }
    }
}

string Stopwatch::timeTaken() {
    ostringstream timeStr;

    timeStr << "Time taken: ";
    if (hours > 1) {
        timeStr << hours << " Hours ";
    }
    else if (hours == 1) {
        timeStr << hours << " Hour ";
    }
    if (minutes > 1) {
        timeStr << minutes << " Minutes ";
    }
    else if (minutes == 1) {
        timeStr << minutes << " Minute ";
    }
    if (seconds > 1 || seconds == 0) {
        timeStr << seconds << " Seconds";
    }
    else if (seconds == 1) {
        timeStr << seconds << " Second";
    }
    return timeStr.str();
}