#pragma once

template <class T>
class IEnumerator {
public:
    virtual bool IsEnd() const = 0;

    virtual void MoveNext() = 0;

    virtual T& Dereference() = 0;

    virtual int Index() const = 0;
};

template <class T>
class IConstEnumerator {
public:
    virtual bool IsEnd() const = 0;

    virtual void MoveNext() = 0;

    virtual const T& ConstDereference() const = 0;

    virtual int Index() const = 0;
};

template <class T>
class IEnumerable {
public:
    virtual IEnumerator<T>* GetEnumerator() = 0;

    virtual IConstEnumerator<T>* GetConstEnumerator() const = 0;
};
