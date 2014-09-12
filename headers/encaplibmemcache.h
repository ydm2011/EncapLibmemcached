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
#include <vector>
#include  <memory>
#include <map>

enum NETPROTOCOL{TCP,UDP,UNIX};

/*
 * this struct is the MemManipulateParam 
 */
struct MemManipulateParam{
    MemManipulateParam(time_t extime=0,uint32_t _flags=0):\
        expirationTime(extime),flags(_flags){
    }
    time_t expirationTime;
    uint32_t flags;
};
/*
 * this template class have the ability that free the stack memory that
 * not allocted by new;
 */ 
template<class FreeType>
class FreeNotByNew{
public:
    FreeNotByNew(size_t length=0):freeBytes(length)
    {
        ;
    }
    void operator()(FreeType*);
private:
    FreeNotByNew operator =(const FreeNotByNew<FreeType>&);
    size_t freeBytes;
};
/*
 * this was the result fetched from the memcached server
 */
struct ResultFromMemcache{
public:
    ResultFromMemcache();
    ResultFromMemcache(char* _key,size_t _keyLength,char* _value,\
                       size_t _valueLength,uint32_t flags=0);
    std::string key;
    std::shared_ptr<char> value;
    size_t keyLength;
    size_t valueLength;
    uint32_t flags;
};
class EncapLibMemcached{
public:
    EncapLibMemcached(const std::string& configParameters,const MemManipulateParam& mmparams);
    EncapLibMemcached(std::shared_ptr<memcached_st> initializedServer,const MemManipulateParam& mmParam);
    //modify the key and value of the memcached;
    const char* set(const std::string& key,const std::string& value);
    const char* add(const std::string& key, const std::string& value);
    const char* replace(const std::string& key,const std::string& value);
    const char* append(const std::string& key,const std::string& value);
    const char* prepend(const std::string& key,const std::string& value);
    const char* increment(const std::string& key,std::uint32_t offset,std::uint64_t& result);
    const char* decrement(const std::string& key,std::uint32_t offset,std::uint64_t& result);
    const char* delete_data(const std::string& key);
    const char* cas(const std::string& key,const std::string& value,std::uint64_t casUnique);
    const char* key_exist(const std::string& key);
    const char* flush_client_buffers();
    //get the memcached instance;
    std::shared_ptr<memcached_st> getMemcached();
    //get infomation from the server
    const MemManipulateParam& get_configurtaion();
    //retrieving data from the server
    const char* get(const std::string& key,std::shared_ptr<char> returnResult);
    const char* muti_get(const std::vector<std::string>& keys);

    const char* fetch_result(std::map<std::string,std::shared_ptr<ResultFromMemcache> >&\
                             results);
    const char* add_server(const std::string& host,in_port_t port,\
                           NETPROTOCOL ptotocol=TCP);
    ~EncapLibMemcached();
protected:
    //EncapLibMemcached(const EncapLibMemcached& memc);
    //EncapLibMemcached operator=(EncapLibMemcached& memc); 
    const char* flush_client();
    void mem_quit();
private:
    memcached_return_t return_info;
    std::shared_ptr<memcached_result_st> smartResultPointer;
    std::shared_ptr<memcached_st> smartMemcPointer;
    MemManipulateParam manipuParam;
};

#endif
