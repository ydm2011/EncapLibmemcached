/*
 * =====================================================================================
 *
 *       Filename:  encaplibmemcache.cpp
 *
 *    Description: this is the defination of the  EncapLibMemcached cla                   class; 
 *
 *        Version:  1.0
 *        Created:  2014年09月04日 10时11分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yu daoming (), ydm2011@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "encaplibmemcache.h"
using namespace std;
/* 
 * this is util class free the stack memory that not allocated by new operator
 */
template<class FreeType>
void FreeNotByNew::operator =(FreeType* instance)
{
    if(length ==0 )
        free(instance);
    else delete[] instance;
}


/*****************************************************************************/
/*
 * this constructor function is construct by the --SERVER=<servername>:<optional port>/<option weight>
 */
EncapLibMemcached::EncapLibMemcached(const string& configParameters,\
        const MemManipulateParam& mmparams):manipuParam(mmparams){
    //initial the memcached_st smart pointer
    memcached_st* memc = memcached(configParameters.c_str(),configParameters.size()); 
    shared_ptr<memcached_st> tempMemc(memc,memcached_free);
    smartMemcPointer = tempMemc;
    //initail the memcached_result_st smartPointer;
    memcached_result_st* tempResult;
    memcached_result_create(smartMemcPointer.get(),tempMemcResult);
    shared_ptr<memcached_result_st> tempMemcResult(tempResult,memcached_result_free);
    smartResultPointer = tempMemcResult; 
}
/*
 * this constructor using the outside memcached struct consturt the object;
 */
EncapLibMemcached::EncapLibMemcached(shared_ptr<memcached_st> initializedServer,MemManipulateParam& mmParam):manipuParam(mmParam){
    smartMemcPointer = initializedServer;
    //initail the memcached_result_st smartPointer;
    memcached_result_st* tempResult;
    memcached_result_create(smartMemcPointer.get(),tempMemcResult);
    shared_ptr<memcached_result_st> tempMemcResult(tempResult,memcached_result_free);
    smartResultPointer = tempMemcResult; 
}
//this fucntion store the value to the memcached
const char* EncapLibMemcached::set(const string& key,const string& value){
    return_info = memcached_set(smartMemcPointer.get(),key.c_str(),\
        key.size(),value.c_str(), manipuParam.expirationTime,manipuParam.flags);
    return memcached_strerror(smartMemcPointer.get(),&return_info);
}
//this function add the nonexist value to the memcache server
const char* EncapLibMemcached::add(const string& key,const string& value){
    return_info = memcached_add(smartMemcPointer.get(),key.c_str(),\
            value.c_str(),value.size(),manipuParam.expirationTime,manipuParam.flags);
    return memcached_strerror(smartMemcPointer.get(),return_info);
}
/*
 * this function replace the value of the given key;
 * if the key is not found ,it will return error string;
 */
const char* EncapLibMemcached::replace(const string& key,const string& value)
{
    return_info = memcahched_replace(smartMemcPointer.get(),key.c_str(),\
           key.size(),value.c_str(),value.size(),manipuParam.expirationTime,\
          manipuParam.flags);
    return memcached_strerror(smartMemcPointer.get(),return_info);
}
/*
 * this function places a segment of data at the end of piece of the data stored;
 */
const char* EncapLibMemcached::append(const string& key,const string& value)
{
    return_info = memcahced_append(smartMemcPointer.get(),key.c_str(),\
            key.size(),value.c_str(),manipuParam.expirationTime,manipuParam.flags);
    return memcached_strerror(smartMemcPointer.get(),return_info);
}
/*
 * this function places a segment of data before the last piece of the data stored;
 */
const char* EncapLibMemcached::prepend(const string& key, const string& value)
{
    return_info = memcached_prepend(smartMemcPointer.get(),key.c_str(),\
            key.size(),value.c_str(),value.size(),manipuParam.expirationTime,\
            MemManipulateParam.flags);
    return memcached_strerror(smartMemcPointer.get(),return_info);
}
/* this function incrememnt the numberic value of the key,if the value is not t
 * numeberic value ,this function will return the error;
 */

const char* EncapLibMemcached::increment(const string& key, const uint32_t \
        offset,uint64_t& result)
{
    return_info = memcached_increment(smartMemcPointer.get(),key.c_str(),key.size(),offset,&result);
    return memcached_strerror(smartMemcPointer.get(),return_info);
} 
/*
 * this function is decrement ,similiar as the increment value
 */

const char* EncapLibMemcached::decrement(const string& key, const uint32_t \
        offset, uint64_t& result)
{
    return_info = memcached_decrement(smartMemcPointer.get(),key.c_str(),\
            key.size(),offset,&result);
    return memcached_strerror(smartMemcPointer.get(),return_info);
}
/*
 * this function delete the data of the given key
 */
const char* EncapLibMemcached::delete_data(const string& key)
{
    return_info = memcached_delete(smartMemcPointer.get(),key.c_str(),\
            key.size(),manipuParam.expirationTime);
    return memcached_strerror(smartMemcPointer.get(),return_info);
}
/*
 * this function is use the unique flag to keep the data consistent
 */
const char* EncapLibMemcached::cas(const string& key,const string& value,uint64_t casUnique)
{
    return_info = memcached_cas(smartMemcPointer.get(),key.c_str,key.size(),\
            value.c_str(),value.size(),manipuParam.expirationTime,manipuParam.flags,casUnique);
    return memcached_strerror(smartMemcPointer.get(),return_info);
}
/*
 * to check whether the key exist or not 
 */
const char* EncapLibMemcached::key_exits(const string& key);
{
    return_info = memcached_exist(smartMemcPointer.get(),key.c_str(),key.size());
    return memcached_strerror(smartMemcPointer.get(),return_info);
}
/*
 * this function will flush all buffers by sending the bufferd commands to the server for processing. 
 */
const char* EncapLibMemcached::flush_client_buffers()
{
    return_info = memcached_flush_buffers(smartMemcPointer.get());
    return memcached_strerror(smartMemcPointer.get(),return_info);
} 
/* 
 * this function wii set will get the memcahced_st smart pointer 
 */
shared_ptr<memcached_st> EncapLibMemcached::getMemcached()
{
    return smartMemcPointer;
}
/* 
 * this function get the get_configurtaion of the memcached_st 
 *  *
 */
const MemManipulateParam& EncapLibMemcached::get_configurtaion()
{
   return manipuParam; 
} 
/*
 * this function will fecch one result from the server
 */
const char* get(const string& key, shared_ptr<char> retrunResult)
{
    size_t value_length;
    uint32_t flags;
    char *result;
    result = memcached_get(smartMemcPointer.get(),key.c_str(),key.size(),\
           &value_length,&flags,&return_info);
    shared_ptr<char>tmp_shared(result,FreeNotByNew());
    return memcached_strerror(smartMemcPointer.get(),return_info);
}
/* 
 *call fetch_result method ,you must first call the muti_get method 
 */
const char* EncapLibMemcached::muti_get(const vector<string>& keys)
{
    const char** tmp_keys = new const char*[keys.size()];
    size_t keysNo = keys.size();
    size_t *eachKeyLength = new size_t[keys.size()];
    vector<string>::iterator iter = keys.begin();
    for(int i=0;i<keysNo;i++,iter++)
    {
        eachKeyLength[i] = iter->size();
    }
    return_info = memcached_mget(smartMemcPointer.get(),tmp_keys,eachKeyLength\
            ,keysNo);
    return memcached_strerror()
}