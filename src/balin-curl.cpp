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
    struct wantedData {
        unsigned int id; 
        std::string filename;
    } wantedData;
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

    struct wantedData jsonresponse;

    balinParseJson(name, response, &jsonresponse.id, &jsonresponse.filename);

    std::cout << "ID: " <<  jsonresponse.id << "\r\n";
    std::cout << "Filename: " <<  jsonresponse.filename << "\r\n";
    
    return jsonresponse.id;
}
//TODO: fix functionality to get with the name
bool balinParseJson(std::string target, std::string response, unsigned int* id, std::string* name) { 
    Json::Value root;
    Json::CharReaderBuilder reader;
    std::istringstream responseStream(response);
    std::string errs;
    if(Json::parseFromStream(reader, responseStream, &root, &errs)) {
            //Ensure we have a valid reponse:
            if(root.isArray() && root.size() > 0) {
                const Json::Value& firstItem = root[0];

                const Json::Value& curr = root;
    
                for(int i = 0; i < curr.size(); i++) {                 
                    const Json::Value& currItem = curr[i];
                    std::cout << "GAMER: " << currItem[0].asInt() << "\r\n";
                    std::cout << "GAMER NAME: " << currItem[1].asString() << "\r\n";
                }

                if(firstItem.isArray() && firstItem.size() >=2) {
                    *id = firstItem[0].asInt();
                    *name = firstItem[1].asString();
                } 
                else {
                    std::cerr << "Json structure does not match expected form\r\n";
                    return false;
                }
            }
            else {
                std::cerr << "Json response is empty or not an array\r\n";
                return false;
            }
        }
        else {
            std::cerr << "Json parsing failed\r\n";
            return false;
        }
        return true;
}




int main() {
    id_getWithName("balinAdd.tar.gz");
    return 0;
}
