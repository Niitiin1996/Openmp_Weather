#include <iostream>
#include <sstream>
#include <vector>
#include <omp.h>
#include <mutex>
#include <numeric>
#include <json/json.h>
#include "fetch.h"

using namespace std;
std::mutex coutMutex;
vector<string> Station(const string& jsonResponse) {
    vector<string> stations;
    Json::CharReaderBuilder reader;
    Json::Value root;
    string errors;
    istringstream stream(jsonResponse);

    if (!Json::parseFromStream(reader, stream, &root, &errors)) {
        cerr << "JSON Parsing Error: " << errors << endl;
        return stations;
    }

    // Check if "meta" -> "stations" exists and is an array
    if (root.isMember("meta") && root["meta"].isMember("stations") && root["meta"]["stations"].isArray()) {
        for (const auto& station : root["meta"]["stations"]) {
            stations.push_back(station.asString());
        }
    }
    return stations;
}

void fetchWeatherForStations(const vector<string>& stations, const string& fetchedData) {
    Json::CharReaderBuilder reader;
    Json::Value root;
    string errors;
    istringstream stream(fetchedData);

    if (!Json::parseFromStream(reader, stream, &root, &errors)) {
        cerr << "JSON Parsing Error: " << errors << endl;
        return;
    }

    #pragma omp parallel for
    for (int i = 0; i < stations.size(); i++) {
        string stationID = stations[i];

        {
            std::lock_guard<std::mutex> guard(coutMutex);
            cout << "Processing Station " << stationID << " in thread " << omp_get_thread_num() << endl;
        }

        vector<double> tavgValues, tminValues, presValues;

        for (const auto& entry : root["data"])
        {
            tavgValues.push_back(entry["tavg"].asDouble());
            tminValues.push_back(entry["tmin"].asDouble());
            presValues.push_back(entry["pres"].asDouble());
        }

        double tavgMean = 0, tminMean = 0, presMean = 0;
        if (!tavgValues.empty()) tavgMean = accumulate(tavgValues.begin(), tavgValues.end(), 0.0) / tavgValues.size();
        if (!tminValues.empty()) tminMean = accumulate(tminValues.begin(), tminValues.end(), 0.0) / tminValues.size();
        if (!presValues.empty()) presMean = accumulate(presValues.begin(), presValues.end(), 0.0) / presValues.size();

        {
            std::lock_guard<std::mutex> guard(coutMutex);
            cout << "Station: " << stationID << endl;
            cout << "Average tavg: " << tavgMean << endl;
            cout << "Average tmin: " << tminMean << endl;
            cout << "Average pres: " << presMean << endl;
        }
    }
}

int main() {
    string apiUrl = "https://meteostat.p.rapidapi.com/point/monthly?lat=52.5244&lon=13.4105&alt=43&start=2020-01-01&end=2020-12-31";
    string Data= fetch(apiUrl);
    vector<string> stations;
    //cout<<Data;

    stations = Station(Data);
    cout<<"\nStations are";
    for (const auto& station : stations) {
        cout << station <<"\n";
    }
    fetchWeatherForStations(stations, Data);

    return 0;
}

