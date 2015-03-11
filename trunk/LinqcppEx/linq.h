#ifndef LINQ_H
#define LINQ_H
#include <functional>
#include <stdlib.h>
#include <iterator>
#include <vector>
/* Writen By Smy20011
 * Another library of linq on cpp
 * Date 2013-4-25
 */
template <class Elem> class Enumerable
{
public:
    virtual Elem* next()
    {
        return NULL;
    }
    virtual bool hasNext()
    {
        return false;
    }
    typedef Elem ElemType;
};

template <class ElemType> class ArrayEnum : public Enumerable<ElemType>
{
private:
    ElemType   *ptr;
    std::size_t size;
    std::size_t index;
public:
    ArrayEnum(ElemType *ptr, std::size_t size) : ptr(ptr), size(size), index(0) {}
    virtual ElemType* next()
    {
        if (hasNext())
        {
            return &ptr[index++];
        }
        return NULL;
    }
    virtual bool hasNext()
    {
        return index != size;
    }
};

template <class T> class LinqMachine : public Enumerable<T>
{
private:
    int searchIndex;
    Enumerable<T>           *dataSource;
    std::function<bool (T&)> filter;
public:
    class Iter
    {
    private:
        LinqMachine<T> machine;
        T *data;
    public:
        Iter(LinqMachine<T> machine) : machine(machine) , data(NULL)
        {
            data = machine.next();
        }
        Iter() : machine(NULL) {};
        Iter operator++()
        {
            data = machine.next();
            return *this;
        }
        T operator*()
        {
            return *data;
        }
        bool operator!=(Iter iter)
        {
            return data;
        }
    };
    Iter begin() const
    {
        return Iter(*this);
    }
    Iter end() const
    {
        return Iter();
    }
    LinqMachine(Enumerable<T> *source) : dataSource(source) {}
    LinqMachine<T> Where(std::function<bool (T&)> filter)
    {
        LinqMachine<T> var = *this;
        var.filter = filter;
        var.searchIndex = 0;
        return var;
    }
    T* next()
    {
        while (dataSource->hasNext())
        {
            T *element = dataSource->next();
            if (filter(*element))
            {
                return element;
            }
        }
        return NULL;
    }
    std::vector<T> ToVector()
    {
        std::vector<T> vec;
        for (T element : *this)
        {
            vec.push_back(element);
        }
        return vec;
    }
};

template <class T, int N> LinqMachine<T> From(T (&arr)[N])
{
    auto source = new ArrayEnum<T>(arr , N);
    LinqMachine<T> linq(source);
    return linq;
}

#endif