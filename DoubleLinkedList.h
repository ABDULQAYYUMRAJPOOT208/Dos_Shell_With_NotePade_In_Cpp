#pragma once
#include <iostream>
using namespace std;
template <typename T>
// Node Class
class Node
{
public:
    Node *next;
    Node *prev;
    T data;
    Node(T val)
    {
        next = nullptr;
        prev = nullptr;
        data = val;
    }
};
// Linked List Class
template <typename T>
class dLinkList
{
public:
    int size;
    Node<T> *head;
    Node<T> *tail;

    dLinkList() : head(nullptr), tail(nullptr), size(0) {}
    //******Iterators*******
    // Forward Iterator Class
    // class fIterator
    // {
    //     Node<T> *iter;

    // public:
    //     fIterator(Node<T> *val) : iter(val) {}
    //     Node<T> *operator++(int)
    //     {
    //         return iter = iter->next;
    //     }
    //     bool operator==(fIterator i)
    //     {
    //         return this->iter == i;
    //     }
    //     bool operator!=(fIterator i)
    //     {
    //         return this->iter != i.iter;
    //     }
    //     T &operator*()
    //     {
    //         return this->iter->data;
    //     }
    // };

    // fIterator fbegin()
    // {
    //     fIterator i(head);
    //     return i;
    // }

    // fIterator fend()
    // {
    //     fIterator i(tail);
    //     return i;
    // }

    // Reverse Iterator
    // class rIterator
    // {
    //     Node<T> *iter;

    // public:
    //     rIterator(Node<T> *val)
    //     {
    //         iter = val;
    //     }
    //     Node<T> *operator++(int)
    //     {
    //         return iter = iter->prev;
    //     }
    //     bool operator==(rIterator i)
    //     {
    //         return this->iter == i;
    //     }
    //     bool operator!=(rIterator i)
    //     {
    //         return this->iter != i.iter;
    //     }
    //     T &operator*()
    //     {
    //         return this->iter->data;
    //     }
    // };

    // rIterator rbegin()
    // {
    //     rIterator i(tail);
    //     return i;
    // }

    // rIterator rend()
    // {
    //     rIterator i(head);
    //     return i;
    // }

    // General Iterator
    class iterator
    {
    public:
        Node<T> *iter;
        iterator(Node<T> *val) : iter(val) {}

        Node<T> *operator++(int)
        {
            return iter = iter->next;
        }
        Node<T> *operator--(int)
        {
            return iter = iter->prev;
        }
        bool operator==(iterator i)
        {
            return this->iter == i;
        }
        bool operator!=(iterator i)
        {
            return this->iter != i.iter;
        }
        T &operator*()
        {
            return this->iter->data;
        }
    };

    iterator begin()
    {
        iterator i(head);
        return i;
    }

    iterator end()
    {
        iterator i(tail);
        return i;
    }

    //*******General Class Functions*******
    bool isEmpty()
    {
        return head == nullptr;
    }

    bool deleteDataWithIndex(int idx)
    {
        if (idx < 0 || idx >= size)
        {
            return false; // Index out of bounds
        }

        Node<T>* temp = head;

        if (idx == 0)
        {
            head = head->next;
            if (head != nullptr)
            {
                head->prev = nullptr;
            }
            else
            {
                // The list is now empty
                tail = nullptr;
            }
        }
        else
        {
            for (int i = 0; i < idx; i++)
            {
                temp = temp->next;
            }

            temp->prev->next = temp->next;

            if (temp->next != nullptr)
            {
                temp->next->prev = temp->prev;
            }
            else
            {
                // Removing the last element
                tail = temp->prev;
            }
        }

        delete temp;
        size--;

        return true;
    }

    void insertAtTail(T val)
    {
        Node<T> *insert = new Node<T>(val);
        if (head == nullptr)
        {
            head = insert;
            tail = insert;
        }
        else
        {
            tail->next = insert;
            insert->prev = tail;
            tail = insert;
        }
        size++;
    }
    void insertAtHead(T val)
    {
        Node<T> *insert = new Node<T>(val);
        if (head == nullptr)
        {
            head = insert;
            tail = insert;
        }
        else
        {
            head->prev = insert;
            insert->next = head;
            head = insert;
        }
        size++;
    }
    void showList()
    {
        Node<T> *temp = head;
        while (temp != nullptr)
        {
            cout << temp->data << "--->";
            temp = temp->next;
        }
        cout << "Null\n";
    }

    void insertAfter(iterator i, T val)
    {
        Node<T> *insert = new Node<T>(val);
        if (i.iter == nullptr)
        {
            head = insert;
            tail = insert;
        }
        else if (i.iter == tail)
        {
            insert->prev = i.iter;
            i.iter->next = insert;
            tail = insert;
        }
        else
        {
            insert->prev = i.iter;
            insert->next = i.iter->next;
            i.iter->next->prev = insert;
            i.iter->next = insert;
        }
        size++;
    }
    void insertBefore(iterator i, T val)
    {
        Node<T> *insert = new Node<T>(val);

        if (i.iter == nullptr)
        {
            head = insert;
            tail = insert;
        }
        else if (i.iter == head)
        {
            insert->next = i.iter;
            i.iter->prev = insert;
            head = insert;
        }
        else
        {
            insert->next = i.iter;
            insert->prev = i.iter->prev;
            i.iter->prev->next = insert;
            i.iter->prev = insert;
        }
        size++;
    }

    void splice(iterator i, dLinkList<T> insert)
    {
        if (i.iter == nullptr)
        {
            return;
        }
        if (head == nullptr)
        {
            head = insert.begin();
            tail = insert.end();
        }
        else
        {
            insert.end()->next = i.iter->next;
            if (i.iter->next != nullptr)
            {
                i.iter->next->prev = insert.end();
            }
            else
            {
                tail = insert.end();
            }

            i.iter->next = insert.begin();
            insert.begin()->prev = i.iter;
        }
        insert.head = nullptr;
        insert.tail = nullptr;
    }

    T& operator[](int idx)
    {
        if (idx < 0 or idx > this->size)
        {
            throw out_of_range("index is out of range");
        }
        else if (idx == 0 )
        {
            return this->head->data;
        }
        else
        {
            Node<T> *result = this->head;
            for (int i = 0; i < idx; i++)
            {
                result = result->next;
            }
            if (result == nullptr)
            {
                throw out_of_range("this is null");
                
            }
            return result->data;
        }
    }
};