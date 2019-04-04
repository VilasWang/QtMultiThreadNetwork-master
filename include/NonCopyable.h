/*!
* @file CNonCopyable.h
* @brief 不可复制对象，用于禁止子类的拷贝构造与赋值。子类可从该类继承即可自动获得以上保证。
*/
#ifndef C_NON_COPYABLE_H
#define C_NON_COPYABLE_H

#ifdef _MSC_VER
#pragma warning(disable: 4275)
#endif

namespace CVC
{
class CNonCopyable
{
protected:
    CNonCopyable(void) {}
    ~CNonCopyable(void) {}

private:
    CNonCopyable(const CNonCopyable&);
    CNonCopyable& operator=(const CNonCopyable&);
};

}

#endif