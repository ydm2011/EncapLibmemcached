#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include "libmemcached/memcached.h"
#include "headers/encaplibmemcache.h"
using namespace std;
shared_ptr<memcached_st> test_function(const string& config)
{
    memcached_st* memc = memcached(config.c_str(),config.size());
    shared_ptr<memcached_st> tmp(memc,memcached_free);
    return tmp;
}

int main()
{
    /*
     * test the FreeNotBYNew template
     */
    char* test_free_template = new char[10];
    memset(test_free_template,'a',10);
    shared_ptr<char> share_test(test_free_template,FreeNotByNew<char>(ARRAY));
    char* test_copy = new char('b');
    shared_ptr<char>  shared_test_copy(test_copy);
    share_test = shared_test_copy;

    /*
     * test the constuctor of the EncapLibmemcached
     */
    string hostAddress = "--SERVER=localhost:11211";
    MemManipulateParam testPara;
    EncapLibMemcached testMemcached(hostAddress,testPara);
    //memcached_st* test_memc = memcached("--SERVER=localhost:11211",hostAddress.size());
    //memcached_free(test_memc);
    //shared_ptr<memcached_st> test_st;
    //test_st = test_function(hostAddress);
    int status;
    status = testMemcached.set("daoming","1000");
    if(status == MEM_SUCCESS)
         std::cout<<status<<endl;
    else std::cout<<"error!"<<endl;
    status = testMemcached.replace("xiaodi","3000");
    testMemcached.delete_data("daoming");
    if(status != MEM_SUCCESS)
        std::cout<<"error!"<<endl;
    return 0;
}


