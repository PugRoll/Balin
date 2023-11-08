#ifndef BALIN_CURL_H
#define BALIN_CURL_H

#include<fstream>
#include<iostream>
#include<curl/curl.h>
#include<json/json.h>


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


unsigned int id_getWithName(std::string name);
bool balinParseJson(std::string target, std::string reponse, unsigned int* id, std::string* name);
#endif
