#pragma once

#if _MSC_VER >= 1700
#define DECL_EQ_DELETE = delete
#else
#define DECL_EQ_DELETE
#endif


#define CLASS_DISABLE_COPY(Class) \
	Class(const Class &) DECL_EQ_DELETE;\
    Class &operator=(const Class &) DECL_EQ_DELETE;


#define REIMPL_CLASSNAME(Class) \
    public: \
        virtual const char* class_name() { return #Class; }


#if _MSC_VER >= 1700
#define FOR_EACH(ITEM, VECTOR) \
    for(auto& ITEM : VECTOR)
#else
#error FOR_EACH not implemented
#endif


#define WSTRING_OF(NAME) L#NAME
