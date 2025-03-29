#include "fetch.h"
#include <iostream>
#include <curl/curl.h>
#include <json/json.h>
#include <sstream>

using namespace std;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

string fetch(const string& apiUrl) {

    CURLcode res;
    string response;

    CURL *hnd = curl_easy_init();


    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "x-rapidapi-key: cffc4eae9bmsh412a8af618e4a49p19ced7jsn1220c328bc9e");
    headers = curl_slist_append(headers, "x-rapidapi-host: meteostat.p.rapidapi.com");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(hnd, CURLOPT_URL, apiUrl.c_str());
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);
    res = curl_easy_perform(hnd);
    curl_easy_cleanup(hnd);
    curl_slist_free_all(headers);


    return(response);
}