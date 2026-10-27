#pragma once
#include <iostream>
#include <mutex>
#include <condition_variable>
using namespace std;

template <typename T>
class sequential_queue4
{
    struct node
    {
        T* data;
        node* next;

        node() : data(nullptr), next(nullptr) {}
    };

    node* head;
    node* tail;

    mutex head_mutex;
    mutex tail_mutex;
    condition_variable cv;

    node* get_tail()
    {
        lock_guard<mutex> lock(tail_mutex);
        return tail;
    }

    T* pop_head()
    {
        lock_guard<mutex> lock(head_mutex);

        if (head == get_tail())
            return nullptr;

        node* old_head = head;
        head = head->next;

        T* result = old_head->data;
        delete old_head;

        return result;
    }

    T* wait_pop_head()
    {
        unique_lock<mutex> lock(head_mutex);

        cv.wait(lock, [&]()
        {
            return head != get_tail();
        });

        node* old_head = head;
        head = head->next;

        T* result = old_head->data;
        delete old_head;

        return result;
    }

public:
    sequential_queue4()
    {
        head = new node();
        tail = head;
    }

    sequential_queue4(const sequential_queue4&) = delete;
    sequential_queue4& operator=(const sequential_queue4&) = delete;

    ~sequential_queue4()
    {
        while (head != nullptr)
        {
            node* temp = head;
            head = head->next;
            delete temp->data;
            delete temp;
        }
    }

    void push(T value)
    {
        T* new_data = new T(value);
        node* ptr = new node();

        lock_guard<mutex> lock(tail_mutex);

        tail->data = new_data;
        tail->next = ptr;
        tail = ptr;

        cv.notify_one();
    }

    T* pop()
    {
        return pop_head();
    }

    T* wait_pop()
    {
        return wait_pop_head();
    }
};