/**
 * @file Pair.h
 * @author Aleksy Walczak (aleksyww@gmail.com)
 * @date 2021-04-09
 * 
 */


#ifndef PAIR_H
#define PAIR_H


template <class T1, class T2>
struct Pair
{
    T1 first;
    T2 second;

    Pair()
    {
    }

    Pair(const T1& _first, const T2& _second)
        : first(_first),
          second(_second)
    {
    }
};


template <class T1, class T2>
bool operator==(const Pair<T1, T2>& lhs, const Pair<T1, T2>& rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}


template <class T1, class T2>
bool operator!=(const Pair<T1, T2>& lhs, const Pair<T1, T2>& rhs)
{
    return lhs.first != rhs.first || lhs.second != rhs.second;
}


#endif
