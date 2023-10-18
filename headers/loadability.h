#pragma once

#define TXTLOG "log.txt"

#include <tchar.h>
#include <windows.h>
#include <thread>
#include <mutex>
#include <string>
#include <vector>
#include <pdh.h>
#include <fstream>
#include <iostream>
#include <cmath>

class Loadability
{
public:
    Loadability(std::wstring paramsL, int period);// create a thread which

	Loadability(const Loadability &) = delete;

	Loadability &operator=(const Loadability &) = delete;

    ~Loadability(); // close file 

    static std::string getResources() {
        std::lock_guard<std::mutex> lock(protectResources);
        return resources;
    }
private:
    void writeInTextFile(int period);
    void updateVisualRecources(std::wstring paramsL);

    std::vector<std::wstring> parseString(const std::wstring& paramsL);

    std::string getResult(const std::wstring& paramsL);
    std::mutex protectResourceGetter;

    static std::mutex protectResources;
    static std::string resources;

    bool stop = false;
    std::mutex stopCondition;

    // threads

    std::thread writerThread;
    std::thread resourceUpdater;
};