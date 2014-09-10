/*
 * =====================================================================================
 *
 *       Filename:  encaplibmemcache.h
 *
 *    Description:  this file just encap the libmemcached by c++
 *
 *        Version:  1.0
 *        Created:  2014年09月03日 14时49分41秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yu daoming (), ydm2011@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef ENCAPLIBMEMCACHE_H
#define ENCAPLIBMEMCACHE_H
#include <iostream>
#include "memcached.h"
#include  <memory>
struct MemManipulateParam{
    MemManipulateParam(time_t extime=0,uint32_t _flags=0):\
        expirationTime(extime),flags(_flags){
    }
    time_t expirationTime;
    uint32_t flags;
};
/*
 * this template class have the ability that free the stack memory that not allocted by new;
 *
 */ 
template<class FreeType>
class FreeNotByNew{
public:
    FreeNotByNew(const size_t length=0):freeBytes(length)
    {
        ;
    }
    void operator()(FreeType*);
private:
    FreeNotByNew(const FreeNotByNew<FreeType>&);
    FreeNotByNew operator =(const FreeNotByNew<FreeType>&);
    size_t freeBytes;
}

class EncapLibMemcached{
public:
    EncapLibMemcached(vector<string> &configParameters,manipulate); 
    EncapLibMemcached(memcached_st* initialedServer);
    //modify the key and value of the memcached;
    const char* set(const std::string& key,const std::string& value);
    const char* add(const string& key, const string& value);
    const char* replace(const string& key,const string& value);
    const char* append(const string& key,const string& value);
    const char* prepend(const string&key,const string& value);
    const char* increment(const string& key,const uint32_t offset);
    const char* decrement(const string& key,const uint32_t offset);
    const char* delete_data_of_server(const string& key);
    const char* cas(const string& key,const string& value,uint64_t casUnique);
    const char* key_exits(const string& key);
    const char* flush_client_buffers();
    //get the memcached instance;
    shared_ptr<memcached_ist> getMemcached();
    //get infomation from the server
    const MemManipulateParam& get_configurtaion();
    //retrieving data from the server
    const char* get(const string& key,shared_ptr<char> returnResult);
    const char* muti_get(const vector<string>& keys);

    const char* fetch_result(char* result);
    void result_key_value();
    void result_value();
    void add_server();
protected:
    //EncapLibMemcached(const EncapLibMemcached& memc);
    //EncapLibMemcached operator=(EncapLibMemcached& memc); 
    private:
        size_t freeBytes;
    };
    const char* flush_client();
    void mem_quit();
    void parse_return_info();
private:
    memcached_return_t return_info;
    shared_ptr<memcached_result_st> smartResultPointer; 
    shared_ptr<memcached_st> smartMemcPointer;
    MemManipulateParam manipuParam;
};

#endif
