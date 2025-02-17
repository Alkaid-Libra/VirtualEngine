#pragma once

#include "runtime/core/meta/json.h"

#include <functional>
#include <string>
#include <vector>
#include <tuple>

namespace VE
{
#if defined(__REFLECTION_PARSER__)
#define META(...) __attribute__((annotate(#__VA_ARGS__)))
#define CLASS(class_name, ...) class __attribute__((annotate(#__VA_ARGS__))) class_name
#define STRUCT(struct_name, ...) struct __attribute__((annotate(#__VA_ARGS__))) struct_name
//#define CLASS(class_name,...) class __attribute__((annotate(#__VA_ARGS__))) class_name:public Reflection::object
#else
#define META(...)
#define CLASS(class_name, ...) class class_name
#define STRUCT(struct_name, ...) struct struct_name
#endif // __REFLECTION_PARSER__

#define REFLECTION_BODY(class_name) \
    friend class Reflection::TypeFieldReflectionOperator::Type##class_name##Operator; \
    friend class Serializer; // todo in serializer.h

#define REFLECTION_TYPE(class_name) \
    namespace Reflection \
    { \
        namespace TypeFieldReflectionOperator \
        { \
            class Type##class_name##Operator; \
        } \
    };

#define REGISTER_FIELD_TO_MAP(name, value) TypeMetaRegisterinterface::registerToFieldMap(name, value); //Maybe location is error
#define REGISTER_BASE_CLASS_TO_MAP(name, value) TypeMetaRegisterinterface::registerToClassMap(name, value); //Maybe location is error
#define REGISTER_ARRAY_TO_MAP(name, value) TypeMetaRegisterinterface::registerToArrayMap(name, value); //Maybe location is error
#define UNREGISTER_ALL(name, value) TypeMetaRegisterinterface::unregisterAll(name, value); //Maybe location is error

#define VIRTUAL_REFLECTION_NEW(name, ...) Reflection::ReflectionPtr(#name, new name(__VA_ARGS__));
#define VIRTUAL_REFLECTION_DELETE(value) \
    if (value) \
    { \
        delete value.operator->(); \
        value.getPtrReference() = nullptr; \
    }

#define TypeMetaDef(class_name, ptr) \
    VE::Reflection::ReflectionInstance(VE::Reflection::TypeMeta::newMetaFromName(#class_name), (class_name*)ptr);
    
#define TypeMetaDefPtr(class_name, ptr) \
    new VE::Reflection::ReflectionInstance(VE::Reflection::TypeMeta::newMetaFromName(#class_name), (class_name*)ptr);

    namespace Reflection
    {
        class TypeMeta;
        class FieldAccessor;
        class ArrayAccessor;
        class ReflectionInstance;
    } // namespace Reflection
    
    template<typename T, typename U, typename = void>
    struct is_safely_castable : std::false_type {};

    template<typename T, typename U>
    struct is_safely_castable<T, U, std::void_t<decltype(static_cast<U>(std::declval<T>()))>> : std::true_type {};

    typedef std::function<int(Reflection::ReflectionInstance*&, void*)> getBaseClassReflectionInstanceListFunc;
    typedef std::function<void*(const Json&)> constructorWithJson;
    typedef std::function<Json(void*)> writeJsonByName;

    typedef std::function<void(void*, void*)> setFuncion;
    typedef std::function<void*(void*)> getFuncion;
    typedef std::function<const char*()> getNameFuncion;
    typedef std::function<bool()> getBoolFunc;
    typedef std::function<void(int, void*, void*)> setArrayFunc;
    typedef std::function<void*(int, void*)> getArrayFunc;
    typedef std::function<int(void*)> getSizeFunc;

    typedef std::tuple<getBaseClassReflectionInstanceListFunc, constructorWithJson, writeJsonByName>
                                                                                                ClassFunctionTuple;
    typedef std::tuple<setFuncion, getFuncion, getNameFuncion, getNameFuncion, getNameFuncion, getBoolFunc>
                                                                                                FieldFunctionTuple;
    typedef std::tuple<setArrayFunc, getArrayFunc, getSizeFunc, getNameFuncion, getNameFuncion> ArrayFunctionTuple;

    namespace Reflection
    {
        class TypeMetaRegisterinterface
        {
        public:
            static void registerToClassMap(const char* name, ClassFunctionTuple* value);
            static void registerToFieldMap(const char* name, FieldFunctionTuple* value);
            static void registerToArrayMap(const char* name, ArrayFunctionTuple* value);

            static void unregisterAll();
        };

        class TypeMeta
        {
            friend class FieldAccessor;
            friend class ArrayAccessor;

            friend class TypeMetaRegisterinterface;

        public:
            TypeMeta();

            static TypeMeta newMetaFromName(std::string type_name);

            static bool newArrayAccessorFromName(std::string array_type_name, ArrayAccessor& accessor);
            static ReflectionInstance newFromNameAndJson(std::string type_name, const Json& json_context);
            static Json writeByName(std::string type_name, void* instance);

            std::string getTypeName();

            int getFieldsList(FieldAccessor*& out_list);

            int getBaseClassReflectionInstanceList(ReflectionInstance*& out_list, void* instance);

            FieldAccessor getFieldByName(const char* name);

            bool isValid() { return m_is_valid; }

            TypeMeta& operator=(const TypeMeta& dest);

        private:
            TypeMeta(std::string type_name);

        private:
            std::vector<FieldAccessor, std::allocator<FieldAccessor>> m_fields;

            std::string m_type_name;

            bool m_is_valid;
        };

        class FieldAccessor
        {
            friend class TypeMeta;

        public:
            FieldAccessor();

            void* get(void* instance);
            void set(void* instance, void* value);

            TypeMeta getOwnerTypeMeta();

            /* 
             * param: TypeMeta out_type
             *        a reference of TypeMeta
             *
             * return: bool value
             *        true: it's a reflection type
             *        false: it's not a reflection type
             */
            bool getTypeMeta(TypeMeta& field_type);
            const char* getFieldName() const;
            const char* getFieldTypeName();
            bool isArrayType();

            FieldAccessor& operator=(const FieldAccessor& dest);

        private:
            FieldAccessor(FieldFunctionTuple* functions);

        private:
            FieldFunctionTuple* m_functions;
            const char* m_field_name;
            const char* m_field_type_name;
        };

        // Function reflection is not implemented, so use this as an std::vector accessor
        class ArrayAccessor
        {
            friend class TypeMeta;

        public:
            ArrayAccessor();
            const char* getArrayTypeName();
            const char* getElementTypeName();
            void set(int index, void* instance, void* element_value);

            void* get(int index, void* instance);
            int getSize(void* instance);

            ArrayAccessor& operator=(ArrayAccessor& dest);

        private:
            ArrayAccessor(ArrayFunctionTuple* array_func);

        private:
            ArrayFunctionTuple* m_func;
            const char* m_array_type_name;
            const char* m_element_type_name;
        };

        class ReflectionInstance
        {
        public:
            ReflectionInstance(TypeMeta meta, void* instance) : m_meta(meta), m_instance(instance) {}
            ReflectionInstance() : m_meta(), m_instance(nullptr) {}

            ReflectionInstance& operator=(ReflectionInstance& dest);

            ReflectionInstance& operator=(ReflectionInstance&& dest);

        public:
            TypeMeta m_meta;
            void* m_instance;
        };

        template<typename T>
        class ReflectionPtr
        {
            template<typename U>
            friend class ReflectionPtr;

        public:
            ReflectionPtr(std::string type_name, T* instance) : m_type_name(type_name), m_instance(instance) {}
            ReflectionPtr() : m_type_name(), m_instance(nullptr) {}

            // copy constructor
            ReflectionPtr(const ReflectionPtr& dest) : m_type_name(dest.m_type_name), m_instance(dest.m_instance) {}
            
            template<typename U /*, typename = typename std::enable_if<std::is_safely_castable<T*, U*>::value>::type*/>
            ReflectionPtr<T>& operator=(const ReflectionPtr<U>& dest)
            {
                if (this == static_cast<void*>(&dest))
                {
                    return *this;
                }
                m_type_name = dest.m_type_name;
                m_instance = static_cast<T*>(dest.m_instance);
                return *this;
            }

            template<typename U /*, typename = typename std::enable_if<std::is_safely_castable<T*, U*>::value>::type*/>
            ReflectionPtr<T>& operator=(const ReflectionPtr<U>&& dest)
            {
                if (this == static_cast<void*>(&dest))
                {
                    return *this;
                }
                m_type_name = dest.m_type_name;
                m_instance = static_cast<T*>(dest.m_instance);
                return *this;
            }

            ReflectionPtr<T>& operator=(const ReflectionPtr<T>& dest)
            {
                if (this == &dest)
                {
                    return *this;
                }
                m_type_name = dest.m_type_name;
                m_instance  = dest.m_instance;
                return *this;
            }

            ReflectionPtr<T>& operator=(ReflectionPtr<T>&& dest)
            {
                if (this == &dest)
                {
                    return *this;
                }
                m_type_name = dest.m_type_name;
                m_instance  = dest.m_instance;
                return *this;
            }

            std::string getTypeName() const { return m_type_name; }

            void setTypeName(std::string name) { m_type_name = name; }

            bool operator==(const T* ptr) const { return (m_instance == ptr); }
            bool operator!=(const T* ptr) const { return (m_instance != ptr); }

            bool operator==(const ReflectionPtr<T>& rhs_ptr) const { return (m_instance == rhs_ptr.m_instance); }
            bool operator!=(const ReflectionPtr<T>& rhs_ptr) const { return (m_instance != rhs_ptr.m_instance); }

            template<typename T1 /* , typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type */>
            explicit operator T1*()
            {
                return static_cast<T1*>(m_instance);
            }

            template<typename T1 /* , typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type */>
            operator ReflectionPtr<T1>()
            {
                return ReflectionPtr<T1>(m_type_name, (T1*)(m_instance));
            }

            template<typename T1 /* , typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type */>
            explicit operator const T1*() const
            {
                return static_cast<T1*>(m_instance);
            }

            template<typename T1 /* , typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type */>
            operator const ReflectionPtr<T1>() const
            {
                return ReflectionPtr<T1>(m_type_name, (T1*)(m_instance));
            }

            T* operator->() { return m_instance; }
            T* operator->() const { return m_instance; }

            T& operator*() { return *(m_instance); }
            const T& operator*() const { return *(static_cast<const T*>(m_instance)); }
            
            T*& getPtrReference() { return m_instance; }

            operator bool() const { return (m_instance != nullptr); }

        private:
            std::string m_type_name {""};
            typedef T m_type;
            T* m_instance {nullptr};
        };
    } // namespace Reflection

} // namespace VE
