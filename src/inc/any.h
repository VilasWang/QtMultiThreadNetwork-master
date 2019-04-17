// Owner: wengkd-a
// Co-Owner:

#pragma once
/////////////////
// Any
//
// A utility container to store any type of data

/* Usage
any a = 42;
cout << a.cast<int>() << endl;
a = 13;
cout << a.cast<int>() << endl;
a = "hello";
cout << a.cast<const char*>() << endl;
a = std::string("1234567890");
cout << a.cast<std::string>() << endl;
int n = 42;
a = &n;
cout << *a.cast<int*>() << endl;
any b = true;
cout << b.cast<bool>() << endl;
swap(a, b);
cout << a.cast<bool>() << endl;
a.cast<bool>() = false;
cout << a.cast<bool>() << endl;
*/

#include <utility>
#include <typeinfo>

namespace CVC
{
//------------------------------------------------------------------------------
namespace anyimpl
{
    struct bad_any_cast {};
    struct empty_any    {};

    struct IPolicy
    {
        virtual size_t get_size()                                    = 0;
        virtual void   static_delete(void** x)                       = 0;
        virtual void   copy_from_value(void const* src, void** dest) = 0;
        virtual void   clone(void* const* src, void** dest)          = 0;
        virtual void   move(void* const* src, void** dest)           = 0;
        virtual void*  get_value(void** src)                         = 0;
        virtual size_t get_typeid() const                            = 0;
    };

    template<typename T>
    struct SmallValuePolicy_T : IPolicy
    {
        virtual size_t get_size() override                          { return sizeof(T); }
        virtual void static_delete(void**) override                 {}
        virtual void copy_from_value(void const* src, void** dest) override { new(dest) T(*reinterpret_cast<T const*>(src)); }
        virtual void clone(void* const* src, void** dest) override  { *dest = *src; }
        virtual void move (void* const* src, void** dest) override  { *dest = *src; }
        virtual void* get_value(void** src) override                { return reinterpret_cast<void*>(src); }
        virtual size_t get_typeid() const override                  { return typeid(T).hash_code(); }
    };

    template<typename T>
    struct BigValuePolicy_T : IPolicy
    {
        virtual size_t get_size() override                          { return sizeof(T); }
        virtual void static_delete(void** x) override               { if (*x) delete(*reinterpret_cast<T**>(x)); *x = NULL; }
        virtual void copy_from_value(void const* src, void** dest) override  { *dest = new T(*reinterpret_cast<T const*>(src)); }
        virtual void clone(void* const* src, void** dest) override  { *dest = new T(**reinterpret_cast<T* const*>(src)); }
        virtual void move(void* const* src, void** dest) override   { (*reinterpret_cast<T**>(dest))->~T(); **reinterpret_cast<T**>(dest) = **reinterpret_cast<T* const*>(src); }
        virtual void* get_value(void** src) override                { return *src; }
        virtual size_t get_typeid() const override                  { return typeid(T).hash_code(); }
    };

    template<typename T>
    struct choose_policy
    {
        typedef BigValuePolicy_T<T> type;
    };

    template<typename T>
    struct choose_policy<T*>
    {
        typedef SmallValuePolicy_T<T*> type;
    };


    // 为Any的类型选择Policy是非法的， 不应该出现这种情况。 如果出现了， 就会有一个编译错误
    struct any;
    template<>
    struct choose_policy<any>
    {
        typedef void type;
    };

    // 一些内建类型的特化
    #define SMALL_POLICY(TYPE) template<> struct choose_policy<TYPE> { typedef SmallValuePolicy_T<TYPE> type; };
    SMALL_POLICY(signed char);
    SMALL_POLICY(unsigned char);
    SMALL_POLICY(signed short);
    SMALL_POLICY(unsigned short);
    SMALL_POLICY(signed int);
    SMALL_POLICY(unsigned int);
    SMALL_POLICY(signed long);
    SMALL_POLICY(unsigned long);
    SMALL_POLICY(float);
    SMALL_POLICY(bool);
    SMALL_POLICY(double);
    #undef SMALL_POLICY

    // 返回不同类型的Policy
    template<typename T>
    IPolicy* get_policy()
    {
        static typename choose_policy<T>::type policy;
        return &policy;
    };
}  // namespace anyimpl
//------------------------------------------------------------------------------
struct any
{
public:
    template <typename T>
    any(const T& x)
        : m_Policy(anyimpl::get_policy<anyimpl::empty_any>())
        , m_Value(NULL)
    {
        assign(x);
    }

    any()
        : m_Policy(anyimpl::get_policy<anyimpl::empty_any>()), m_Value(NULL)
    { }

    any(const char* x)
        : m_Policy(anyimpl::get_policy<anyimpl::empty_any>()), m_Value(NULL)
    {
        assign(x);
    }

    any(const any& x)
        : m_Policy(anyimpl::get_policy<anyimpl::empty_any>()), m_Value(NULL)
    {
        assign(x);
    }

    any(any && x)
        : m_Policy(anyimpl::get_policy<anyimpl::empty_any>()), m_Value(NULL)
    {
        reset();
        m_Policy = x.m_Policy;
        m_Policy->move(&x.m_Value, &m_Value);
        x.m_Policy = anyimpl::get_policy<anyimpl::empty_any>();
        x.m_Value = NULL;
    }

    ~any() {
        m_Policy->static_delete(&m_Value);
    }

    any& assign(const any& x) {
        reset();
        m_Policy = x.m_Policy;
        m_Policy->clone(&x.m_Value, &m_Value);
        return *this;
    }

    template <typename T>
    any& assign(const T& x) {
        reset();
        m_Policy = anyimpl::get_policy<T>();
        m_Policy->copy_from_value(&x, &m_Value);
        return *this;
    }

    template<typename T>
    any& operator=(const T& x) {
        return assign(x);
    }

    any& operator=(const any & x) {
        return assign(x);
    }

    any& operator=(any && x) {
        reset();
        m_Policy = x.m_Policy;
        m_Policy->move(&x.m_Value, &m_Value);
        x.m_Policy = anyimpl::get_policy<anyimpl::empty_any>();
        x.m_Value = NULL;
        return *this;
    }

    any& operator=(const char* x) {
        return assign(x);
    }

    any& swap(any& x) {
        std::swap(m_Policy, x.m_Policy);
        std::swap(m_Value, x.m_Value);
        return *this;
    }

    // Cast operator. 转换成原来的类型
    template<typename T>
    T& cast() {
        if (!policy_compatible(anyimpl::get_policy<T>()))
            throw anyimpl::bad_any_cast();
        T* r = reinterpret_cast<T*>(m_Policy->get_value(&m_Value));
        return *r;
    }

    // Cast operator. 转换成原来的类型
    template<typename T>
    const T& cast() const {
        if (!policy_compatible(anyimpl::get_policy<T>()))
            throw anyimpl::bad_any_cast();
        const T* r = reinterpret_cast<const T*>(m_Policy->get_value(const_cast<void **>(&m_Value)));
        return *r;
    }

    bool empty() const {
        return policy_compatible(anyimpl::get_policy<anyimpl::empty_any>());
    }

    // 释放内存
    void reset() {
        m_Policy->static_delete(&m_Value);
        m_Policy = anyimpl::get_policy<anyimpl::empty_any>();
    }

    // 如果类型一样， 返回 'true'
    bool compatible(const any& x) const {
        return policy_compatible(x.m_Policy);
    }

private:
    bool policy_compatible(const anyimpl::IPolicy *pPolicy) const
    {
        return m_Policy == pPolicy ||
               m_Policy->get_typeid() == pPolicy->get_typeid();
    }
    anyimpl::IPolicy* m_Policy;
    void*             m_Value;
};  // struct any
} // namespace gbmp