#include "mu_test.h"
#include "thread_pool.hpp"
#include "task_printer.hpp"

class Add : public concurrency::Task {
public:
    explicit Add(std::mutex& a_mutex,std::atomic_int& a_number,int a_addendum,int a_times);

    void operator()() override;

private:
    std::mutex& m_mtx;
    std::atomic_int& m_number;
    int m_addendum;
    int m_times;
};

Add::Add(std::mutex& a_mutex,std::atomic_int& a_number,int a_addendum,int a_times)
: m_mtx{a_mutex}
, m_number{a_number}
, m_addendum{a_addendum}
, m_times{a_times}
{
}

void Add::operator()()
{
    for (int i = 0; i < m_times; ++i) {
        m_number += m_addendum;
    }
}


class Sysiphus : public concurrency::Task {
public:
    explicit Sysiphus(size_t& a_number,int a_add,size_t a_times);

    void operator()() override;

private:
    size_t& m_number;
    int m_addendum;
    size_t m_times;
};

Sysiphus::Sysiphus(size_t& a_number,int a_add,size_t a_times)
: m_number{a_number}
, m_addendum{a_add}
, m_times{a_times}
{
}

void Sysiphus::operator()()
{
    for (size_t i = 0; i < m_times; ++i) {
        m_number += m_addendum;
    }
}


BEGIN_TEST(ctor_dtor)
    concurrency::ThreadPoolExecuter defaulted;
    concurrency::ThreadPoolExecuter with_thread_size{2};
    concurrency::ThreadPoolExecuter with_queue_size{10,50};
    ASSERT_PASS();

END_TEST

BEGIN_TEST(test_with_tasks)
    size_t threads_num = 20;
    concurrency::ThreadPoolExecuter pool{threads_num};
    std::mutex mutex_one;
    std::mutex mutex_two;
    std::atomic_int number{};
    for (size_t i = 0; i < 50; ++i) {
        pool.submit(std::make_unique<task::PrinterTask>(task::PrinterTask{mutex_one,5}));
    }
    for (int i = 1; i < 50; ++i) {
        pool.submit(std::make_unique<Add>(Add{mutex_two,number,1,i}));
    }
    ASSERT_EQUAL(threads_num,pool.threads());
    ::sleep(2);
    ASSERT_EQUAL(number,1225);
   

END_TEST

BEGIN_TEST(shutdown_test)
    concurrency::ThreadPoolExecuter pool{2,1000};
    std::mutex mutex_one;
    std::atomic_int number{};
    for (int i = 1; i < 1001; ++i) {
        pool.submit(std::make_unique<Add>(Add{mutex_one,number,1,i}));
    }
    pool.shutdown();
    ASSERT_EQUAL(0,pool.threads());
    ASSERT_EQUAL(number,500500);
    pool.submit(std::make_unique<Add>(Add{mutex_one,number,100,100}));
    ASSERT_EQUAL(number,500500);
   

END_TEST

BEGIN_TEST(add_test)
    size_t vector_size = 100'000;
    size_t number_of_adds = 100'000;
    int number_to_add = 2;
    std::vector<size_t> vector(vector_size,0);
    ASSERT_EQUAL(vector.size(),vector_size);
    concurrency::ThreadPoolExecuter pool{0,vector_size};
    for (size_t i = 0; i < vector_size; ++i) {
        pool.submit(std::make_unique<Sysiphus>(Sysiphus{vector.at(i),number_to_add,number_of_adds}));
    }
    pool.add(1000);
    pool.shutdown();
    auto begin = vector.begin();
    auto end = vector.end();
    while (begin != end) {
        ASSERT_EQUAL(*begin,number_of_adds*number_to_add);
        ++begin;
    }

END_TEST




BEGIN_TEST(remove_test)
    size_t vector_size = 10'000;
    size_t number_of_adds = 100'000;
    int number_to_add = 2;
    std::vector<size_t> vector(vector_size,0);
    ASSERT_EQUAL(vector.size(),vector_size);
    concurrency::ThreadPoolExecuter pool{50,vector_size};
    ASSERT_EQUAL(pool.threads(),50);
    for (size_t i = 0; i < vector_size; ++i) {
        pool.submit(std::make_unique<Sysiphus>(Sysiphus{vector.at(i),number_to_add,number_of_adds}));
    }
    pool.remove(40);
    ASSERT_EQUAL(10,pool.threads());
    pool.shutdown();
    auto begin = vector.begin();
    auto end = vector.end();
    while (begin != end) {
        ASSERT_EQUAL(*begin,number_of_adds*number_to_add);
        ++begin;
    }


END_TEST

BEGIN_TEST(second_remove_test)
    size_t number_of_adds = 100'000;
    int number_to_add = 2;
    size_t number{};
    concurrency::ThreadPoolExecuter pool{5,5};
    ASSERT_EQUAL(pool.threads(),5);
    for (size_t i = 0; i < 5; ++i) {
        pool.submit(std::make_unique<Sysiphus>(Sysiphus{number,number_to_add,number_of_adds}));
    }
    pool.remove(5);
    ASSERT_EQUAL(0,pool.threads());
    pool.shutdown();
    ASSERT_THAT(number < 5*number_of_adds*number_to_add);

END_TEST

BEGIN_TEST(immediate_shutdown_test)
    size_t number_of_adds = 10'000;
    int number_to_add = 2;
    size_t number{};
    concurrency::ThreadPoolExecuter pool{5,20};
    ASSERT_EQUAL(pool.threads(),5);
    for (size_t i = 0; i < 20; ++i) {
        pool.submit(std::make_unique<Sysiphus>(Sysiphus{number,number_to_add,number_of_adds}));
    }
    pool.immediate_shutdown();
    ASSERT_THAT(number < 20*number_of_adds*number_to_add);
    size_t new_number = number;
    ASSERT_EQUAL(pool.threads(),0);
    pool.add(10);
    ::sleep(2);
    ASSERT_EQUAL(new_number,number);

END_TEST


BEGIN_SUITE([Life and death are one thread])
    TEST(ctor_dtor)
    TEST(test_with_tasks)
    TEST(shutdown_test)
    TEST(add_test)
    TEST(remove_test)
    TEST(second_remove_test)  
    TEST(immediate_shutdown_test)
    
END_SUITE