#ifndef BALIN_CURL_H
#define BALIN_CURL_H


#include<fstream>
#include<iostream>
#include<curl/curl.h>
#include<json/json.h>


const int BALIN_CURL_ERROR = -1;


const std::string url = "http://127.0.0.1:5000";

/** 
 * Functionalilty: Callback function to create an object .tar.gz file 
 * @Param: void* contents
 * @Param: size_t size
 * @Param: size_t nmemb 
 * @Param: void* userp 
 */
static size_t WriteDataCallback(void* contents, size_t size, size_t nmemb, void* userp);

/** 
 * Functionalilty: Callback function to write out a string response from a query 
 * @Param: void* contents
 * @Param: size_t size
 * @Param: size_t nmemb 
 * @Param: void* userp 
 */
static size_t WriteResponseCallback(void* contents, size_t size, size_t nmemb, void* userp); 


/** 
 * Functionalilty: get the id of a dependency in the database with a given name
 * @Param: (string) name -> name of target dependency
 */
unsigned int id_getWithName(std::string name);

/** 
 * Functionalilty: function to return a boolean value if the target was found with a given response
 * @Param: target -> name of dependency
 * @Param: response -> JSON string response typically like: 
 *  "[[id_key, "dep_name","archive.tar.gz", "version"] [2, "balinAdd", "balinAdd.tar.gz", "1.0"]]"
 * @Param: *id -> pointer to an unsigned integer value. Passed by reference
 * @Param: *id -> poitner to an string target filename, passed by reference 
 */
bool balinParseJson(std::string target, std::string reponse, unsigned int* id, std::string* name);

void downloadArchiveFromDB(unsigned int id, std::string packageName);

void moveToBuildDirectory(std::string archiveName);

#endif
