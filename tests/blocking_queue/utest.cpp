#include "mu_test.h"
#include "blocking_queue.hpp"

#include <vector> // second test
#include <thread> // thread

BEGIN_TEST(no_threads)
    thread::BlockingQueue<int> queue{5};
    ASSERT_THAT(queue.empty());
    for (int i = 0; i < 5; ++i) {
        queue.enqueue(i);
    }
    ASSERT_THAT(queue.full());

END_TEST

BEGIN_TEST(one_prod_one_cons)

    thread::BlockingQueue<int> queue{5};
    std::vector<int> v1;
    for (int i = 0; i < 50; ++i) {
        v1.push_back(i);
    }
    std::vector<int> v2{};
    std::vector<std::thread> threads{};
    auto enqueue = [&v1,&queue](){
        for (size_t i = 0; i < 50; ++i) {
            int a = v1.back();
            v1.pop_back();
            queue.enqueue(a);
        }
    };

    auto dequeue = [&v2,&queue](){
        for (size_t i = 0; i < 50; ++i) {
            int a;
            queue.dequeue(a);
            v2.push_back(a);
        }
    };

    threads.emplace_back(enqueue);
    threads.emplace_back(dequeue);
    for (auto& thread : threads) {
        thread.join();
    }
    for (int i = 0; i < 50; ++i) {
        ASSERT_EQUAL(v2.at(i),49 - i);
    }
    

END_TEST

// BEGIN_TEST(yes_threads)
//     thread::BlockingQueue<double> queue{10};
//     std::vector<double> vector_one{1,1000000};
//     std::vector<double> vector_two{2,1000000};
//     std::vector<double> vector_three{3,1000000};

//     auto v1_remover = [&vector_one,&queue](){
//         while (not vector_one.empty()) {
//             auto d = vector_one.back();
//             vector_one.pop_back();
//             queue.enqueue(d);
//         }
//     };

//     auto v2_remover = [&vector_two,&queue](){
//         while (not vector_two.empty()) {
//             auto d = vector_two.back();
//             vector_two.pop_back();
//             queue.enqueue(d);
//         }
//     };

//     auto v3_remover = [&vector_three,&queue](){
//         while (not vector_three.empty()) {
//             auto d = vector_three.back();
//             vector_three.pop_back();
//             queue.enqueue(d);
//         }
//     };

//     auto dequeuer_one = [&queue](){
//         for (size_t i = 0; i < 3000000; ++i) {
//             queue.dequeue();
//         }
//     };

//     std::vector<std::thread> threads;
//     threads.emplace_back(v1_remover);
//     threads.emplace_back(v2_remover);
//     threads.emplace_back(v3_remover);

//     threads.emplace_back(dequeuer_one);

//     for (auto& thread : threads) {
//         thread.join();
//     }
//     ASSERT_THAT(vector_one.empty());
//     ASSERT_THAT(vector_two.empty());
//     ASSERT_THAT(vector_three.empty());
//     ASSERT_THAT(queue.empty());


// END_TEST

BEGIN_SUITE([Block this])
    TEST(no_threads)
    TEST(one_prod_one_cons)
    //TEST(yes_threads) 

END_SUITE