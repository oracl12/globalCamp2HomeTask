#include "../headers/loadability.h"

std::mutex Loadability::protectResources;
std::string Loadability::resources;

std::vector<std::wstring> Loadability::parseString(const std::wstring& paramsL)
{
    std::vector<std::wstring> values;

    size_t startPos = 0;
    size_t commaPos = paramsL.find(',');

    while (commaPos != std::wstring::npos) {
        std::wstring value = paramsL.substr(startPos, commaPos - startPos);
        values.push_back(value);
        startPos = commaPos + 1;
        commaPos = paramsL.find(',', startPos);
    }

    std::wstring lastValue = paramsL.substr(startPos);
    values.push_back(lastValue);

    return values;
}

// std::string Loadability::getResult(const std::wstring& paramsL)
// {
//     std::wclog << "LOADABILITY: GETTING RESULT BaseD ON PARAMS: " << paramsL << std::endl;

//     auto params = parseString(paramsL);
//     PDH_HQUERY query;
//     PdhOpenQuery(NULL, 0, &query);

//     PDH_HCOUNTER cpuCounter = NULL, memoryCounter = NULL, networkCounter = NULL;

//     for (std::wstring param: params) {
//         if (param == L"cpu")
//         {
//             std::wcout << param << std::endl;
//             PdhAddCounterW(query, L"\\Processor(_Total)\\% Processor Time", 0, &cpuCounter);
//         }
//         if (param == L"mem")
//         {
//             std::wcout << param << std::endl;
//             PdhAddCounterW(query, L"\\Memory\\Available Bytes", 0, &memoryCounter);
//         }
//         if (param == L"net"){
//             std::wcout << param << std::endl;
//             PdhAddCounterW(query, L"\\Network Interface(*)\\Bytes Total/sec", 0, &networkCounter);
//         }
//     }

//     PdhCollectQueryData(query);
//     Sleep(1000); // Wait for some time to collect data

//     std::string result;
//     std::cout << "HHHHERERERE" << std::endl;
//     std::cout << "HHHHERERERE" << std::endl;
//     std::cout << "HHHHERERERE" << std::endl;
//     std::cout << "HHHHERERERE" << std::endl;
//     if (cpuCounter) {
//         PDH_FMT_COUNTERVALUE cpuValue;
//     std::cout << "WE  are in cpu" << std::endl;

//         PdhGetFormattedCounterValue(cpuCounter, PDH_FMT_DOUBLE, NULL, &cpuValue);
//         result += "CPU Usage: " + std::to_string(cpuValue.doubleValue) + "% | ";
//     }

//     if (memoryCounter) {
//         PDH_FMT_COUNTERVALUE memoryValue;
//         std::cout << "WE  are in memory" << std::endl;

//         PdhGetFormattedCounterValue(memoryCounter, PDH_FMT_LARGE, NULL, &memoryValue);
//         result += "Memory Available: " + std::to_string(memoryValue.largeValue) + " bytes | ";
//     }

//     if (networkCounter) {
//         PDH_FMT_COUNTERVALUE networkValue;
//         std::cout << "WE  are in network" << std::endl;

//         PdhGetFormattedCounterValue(networkCounter, PDH_FMT_LARGE, NULL, &networkValue);
//         result += "Network Usage: " + std::to_string(networkValue.largeValue) + " bytes/sec";
//     }

//     PdhCloseQuery(query);

//     return result;
// }
std::string Loadability::getResult(const std::wstring& paramsL)
{
    // Open a query to collect performance data
    PDH_HQUERY query;
    PdhOpenQuery(NULL, 0, &query);

    // Add counters for CPU, memory, and network
    PDH_HCOUNTER cpuCounter, memoryCounter, networkCounter;
    DWORD pid = GetCurrentProcessId();
    std::wstring counterCpuPath = L"\\Processor(" + std::to_wstring(pid) + L")\\% Processor Time";
    std::wstring counterMemPath = L"\\Memory()" + std::to_wstring(pid) + L"\\% Processor Time";
    std::wstring counterNetPath = L"\\Network Interface(*)" + std::to_wstring(pid) + L"\\% Processor Time";
    PdhAddCounterW(query, counterCpuPath.c_str(), 0, &cpuCounter);
    PdhAddCounterW(query, counterMemPath.c_str(), 0, &memoryCounter);
    PdhAddCounterW(query, counterNetPath.c_str(), 0, &networkCounter);
    // PdhAddCounter(query, _T("\\Memory\\Committed Bytes"), 0, &memoryCounter);
    // PdhAddCounter(query, _T("\\Network Interface(*)\\Bytes Total/sec"), 0, &networkCounter);

    // Collect the performance data
    PdhCollectQueryData(query);

    // Sleep for a while to collect data
    Sleep(2000); // Collect data for 1 second

    // Get and display CPU usage
    PDH_FMT_COUNTERVALUE cpuValue;
    PdhGetFormattedCounterValue(cpuCounter, PDH_FMT_DOUBLE, NULL, &cpuValue);
    std::cout << "CPU Usage: " << cpuValue.doubleValue << "%" << std::endl;

    // Get and display memory available
    PDH_FMT_COUNTERVALUE memoryValue;
    PdhGetFormattedCounterValue(memoryCounter, PDH_FMT_LARGE, NULL, &memoryValue);
    double memoryMB = static_cast<double>(memoryValue.largeValue) / (1024 * 1024);
    std::cout << "Memory Available: " << memoryMB << " MB" << std::endl;

    // Get and display network usage
    PDH_FMT_COUNTERVALUE networkValue;
    PdhGetFormattedCounterValue(networkCounter, PDH_FMT_LARGE, NULL, &networkValue);
    std::cout << "Network Usage: " << networkValue.largeValue << " bytes/sec" << std::endl;

    // Close the query
    PdhCloseQuery(query);
    return "";
}

void Loadability::writeInTextFile(int period)
{
    std::ofstream outputFile(TXTLOG, std::ios::app);

    while (true)
    {
        std::string textToAppend;
        {
            protectResourceGetter.lock();
            textToAppend = getResult(L"mem,cpu,net");
            std::cout << textToAppend << std::endl;
        }

        if (outputFile.is_open()) {
        
            outputFile << textToAppend << std::endl;

            std::cout << "LOADABILITY: ADDED STRING TO FILE" << std::endl;
        } else {
            std::cerr << "LOADABILITY: FILE iS NOT OPEN error" << std::endl;
            break;
        }
            
        {
            std::lock_guard<std::mutex> lock(stopCondition);
            if (stop)
                break;
        }

        Sleep(period * 60 * 100); // in miliseconds
    }
    outputFile.close();
}

void Loadability::updateVisualRecources(std::wstring paramsL){
    while (true) {
        {
            std::lock_guard<std::mutex> lock(protectResources);
            {
                std::lock_guard<std::mutex> lock(protectResourceGetter);
                resources = getResult(paramsL);
            }
        }

        {
            std::lock_guard<std::mutex> lock(stopCondition);
            if (stop)
                break;
        }
    }
}

// mem cpu net
Loadability::Loadability(std::wstring paramsL, int period)
{
    if (paramsL.empty())
        paramsL = L"mem,cpu,net";

    std::clog << "LOADABILITY: CREAtiNG THREADS" << std::endl;

    writerThread = std::thread([this, period]() {
        writeInTextFile(period);
    });

    resourceUpdater = std::thread([this, paramsL]() {
        updateVisualRecources(paramsL);
    });

    std::clog << "LOADABILITY: THREADS CREATED" << std::endl;
}

Loadability::~Loadability()
{
    {
        std::lock_guard<std::mutex> lock(stopCondition);
        stop = true;
    }

    if (writerThread.joinable())
        writerThread.join();
    if (resourceUpdater.joinable())
        resourceUpdater.join();

    std::clog << "LOADABILITY: THREADS STOPPED" << std::endl;
}