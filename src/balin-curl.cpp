#include "../include/balin-curl.hpp"

static size_t WriteDataCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    //Create a file object for our binary data
    std::ofstream* outfile = static_cast<std::ofstream*>(userp);
    outfile->write(static_cast<char*>(contents), total_size);
    return total_size;
}

static size_t WriteResponseCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), total_size);
    return total_size;
}

unsigned int id_getWithName(std::string name){
    unsigned int id;
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if(!curl) {
        std::cerr << "\t[ERROR]Error initializing curl\r\n";
        return -1;
    }
    std::string response;
    std::string query = url + "/query";
    curl_easy_setopt(curl, CURLOPT_URL, query.c_str());

    //Set the callback function to handle response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteResponseCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA , &response); 

    res = curl_easy_perform(curl);
    if(res == CURLE_OK) {
        std::cout << response << "\r\n";
    }

    curl_easy_cleanup(curl);

    id = 9999999;
    return id;
}


int main() {
    id_getWithName("balinAdd.tar.gz");
    return 0;
}
