#include "../include/balin-curl.hpp"
#include "../include/balin_common.hpp"

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
        balinError("Error initializing curl");
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
        balinInfo(response.c_str());
    }

    curl_easy_cleanup(curl);

    struct wantedData jsonresponse;

    if (balinParseJson(name, response, &jsonresponse.id, &jsonresponse.filename)) {
        balinInfo(("Found: " + name).c_str());
//        std::cout << "ID: " <<  jsonresponse.id << "\r\n";
//        std::cout << "Filename: " <<  jsonresponse.filename << "\r\n";
        return jsonresponse.id;
    }
    else {
        balinInfo((name + " could not be found").c_str());
        return -1;
    }
    
}


void downloadArchiveFromDB(unsigned int id, std::string packageName) {
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if(!curl) {
        balinError("Error initializing curl");
    }

    std::string filename = packageName + ".tar.gz";
    std::ofstream out_file(filename, std::ios::binary);

    std::string query = url + "/get_file/" + std::to_string(id);

    curl_easy_setopt(curl, CURLOPT_URL,query.c_str()); 

    //Use the write data callback function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteDataCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA , &out_file); 

    res = curl_easy_perform(curl);
    if(res == CURLE_OK) {
        balinInfo((filename + "downloaded succesfully").c_str());
        moveToBuildDirectory(filename);
    }
    else {
        std::ostringstream str;
        str << "\t[ERROR]: " << filename << " download unsuccesfull";
        balinError(str.str().c_str());
        str.clear();
        str << "curl_easy_perform() failed: " << curl_easy_strerror(res);
        balinError(str.str().c_str());
    }




    out_file.close(); 

}


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

                int compareResult = BALIN_CURL_ERROR; //assume we won't find it
                std::ostringstream str;
    
                for(int i = 0; i < curr.size(); i++) {                 
                    const Json::Value& currItem = curr[i];
                    str << "Package: " << currItem[0].asInt();
                    balinInfo(str.str().c_str());
                    str.clear();
                    str << "Package name: " << currItem[1].asString();
                    balinInfo(str.str().c_str());
                    compareResult = currItem[1].compare(target);
                    if(compareResult == 0) { 
                        *id = currItem[0].asInt();
                        return true;
                    }
                }
            }
            else {
                balinError("Json response is empty or not an array");
                return false;
            }
        }
        else {
            balinError("Json parsing failed");
            return false;
        }
        return false;
}


void moveToBuildDirectory(std::string archiveName) {
    std::ostringstream cmd;
    cmd << "mv " << archiveName << " ./build/" << archiveName;
    std::system(cmd.str().c_str());
}


