#include <iostream>
#include <memory>
#include <vector>
#include "libmemcached/memcached.h"
using namespace std;

void myfree(char* p)
{
    delete[] p;
}
template<class FreeClass>
class FreeGet{
public:
    FreeGet(const size_t length=0);
    void operator ()(FreeClass* instance){
        std::cout<<"test!"<<endl;
    }
private:
    size_t freeBytes;
};

int main()
{
    memcached_server_st *servers =NULL;
    //memcached_server_list_st is defined by memcached_server_st*
    memcached_st *memc;// = new memcached_st;
    memcached_return_t rc;
    char *key="keystring";
    char *value = "aaaaaaa";

    memc= memcached_create(NULL);

    servers = memcached_server_list_append(servers,"localhost",11211,&rc);

    rc = memcached_server_push(memc,servers);
    if(rc == MEMCACHED_SUCCESS)
        std::cout<<"append success!"<<std::endl;
    else std::cout<<"append failed!"<<memcached_strerror(memc,rc)<<std::endl;

    /*
     * test set
     */
    rc = memcached_set(memc,key,9,value,7,(time_t)0,(u_int32_t)0);

    if(rc == MEMCACHED_SUCCESS)
        std::cout<<"set success!"<<std::endl;
    else std::cout<<"setfailed!"<<memcached_strerror(memc,rc)<<std::endl;
    key="daoming";
    value = "bbbbbbb";
    rc = memcached_set(memc,key,7,value,7,(time_t)0,(u_int32_t)0);

    if(rc == MEMCACHED_SUCCESS)
        std::cout<<"set success!"<<std::endl;
    else std::cout<<"setfailed!"<<memcached_strerror(memc,rc)<<std::endl;


    /*
     * test get
     */
    size_t value_length;
    memcached_return_t rerror;
    char *result;
    u_int32_t flags=0;
    result= memcached_get(memc,key,9,&value_length,&flags,&rerror);
    if(memcached_success(rerror)){
        std::cout<<"get success!"<<std::endl;
        std::cout<<result<<std::endl;
    }
    else std::cout<<"get failed!"<<memcached_strerror(memc,rc)<<std::endl;
    free(result);
    /*
     * test delete
     */
    rerror = memcached_delete(memc,key,7,(time_t)0);
    if(memcached_success(rerror)){
        std::cout<<"delete success!"<<std::endl;
        std::cout<<result<<std::endl;
    }
    else std::cout<<"delete failed!"<<memcached_strerror(memc,rc)<<std::endl;

    /*
     * test again
     */
    rc = memcached_set(memc,key,7,value,7,(time_t)0,(u_int32_t)0);

    if(rc == MEMCACHED_SUCCESS)
        std::cout<<"set success!"<<std::endl;
    else std::cout<<"setfailed!"<<memcached_strerror(memc,rc)<<std::endl;

    rerror = memcached_exist(memc,key,7);
    std::cout<<memcached_strerror(memc,rerror)<<std::endl;

    rerror = memcached_flush_buffers(memc);
    std::cout<<memcached_strerror(memc,rerror)<<std::endl;

    /*
     * test the mutiget
     */
    char *keys[] = {"daoming","keystring"};
    size_t key_length[]={7,9};
    rerror = memcached_mget(memc,keys,key_length,2);
    std::cout<<memcached_strerror(memc,rerror)<<std::endl;
    /*
     * test the fetch_result
     */
    memcached_result_st fetch_result;
    memcached_fetch_result(memc,&fetch_result,&rerror);

    //get the keyvalue from the result;the key value will be overlad
    const char* test_result;
    test_result = memcached_result_key_value(&fetch_result);
    std::cout<<test_result<<"  "
            <<sizeof(test_result)
            <<std::endl;

    //test the
    const char* test_result2;
    memcached_fetch_result(memc,&fetch_result,&rerror);
    test_result2 = memcached_result_key_value(&fetch_result);
    //memcached_fetch()

    std::cout<<test_result<<"  "<<test_result2<<"  "
            <<sizeof(test_result)
            <<std::endl;

    memcached_mget(memc,keys,key_length,2);

    //test memcached_fetch memory leak;
    char test_fetch1_key[MEMCACHED_MAX_KEY];
    char test_fetch2_key[MEMCACHED_MAX_KEY];
    char *test_value1;
    char *test_value2;

    size_t test_fetch1_key_length;
    size_t test_fetch1_returnValue_length;
    std::uint32_t test_flags;
    test_value1 = memcached_fetch(memc,test_fetch1_key,&test_fetch1_key_length,&test_fetch1_returnValue_length,&test_flags,&rerror);
    test_value2 = memcached_fetch(memc,test_fetch2_key,&test_fetch1_key_length,&test_fetch1_returnValue_length,&test_flags,&rerror);

    std::cout<<test_fetch1_key<<":"<<test_value1
            <<"  "<<test_fetch2_key<<":"<<test_value2<<std::endl;

    rerror = memcached_flush(memc,0);
    std::cout<<memcached_strerror(memc,rerror)<<std::endl;

    shared_ptr<char> test_ptr(new char[10],myfree);
    shared_ptr<char> test_ptr2 = test_ptr;

    char* test_ptr_con = test_ptr.get();
    for(int i=0;i<10;i++)
    {
        test_ptr_con[i]='a';
    }

    std::cout<<*test_ptr<<std::endl;
    //memcached_result_free
   // memcached_fetch();
    const string string_test("test");
    //shared_ptr<char> test_const_smart(string_test.c_str());
    const char* str = string_test.c_str();

    char* i_love = "i love";
    char i_love1[] = {'i',' ','l','o','v','e'};
    std::cout<<sizeof(i_love)<<std::endl;
    std::cout<<strlen(i_love)<<std::endl;
    std::cout<<sizeof(i_love1)<<" "<<strlen(i_love1)<<std::endl;

    const char* tmp_array;
    const char** tmp_two = new const char* [10];

    string test_array("love");
    tmp_array = test_array.c_str();
       tmp_two[0] = tmp_array;
    return 0;
}


