#include "runtime/core/meta/reflection/reflection.h"
#include "runtime/core/meta/reflection/reflection_register.h"

#include "_generated/reflection/all_reflection.h"
#include "_generated/serializer/all_serializer.ipp"

namespace VE
{
    namespace Reflection
    {
        void TypeMetaRegister::Unregister() { TypeMetaRegisterinterface::unregisterAll(); }
    } // namespace Reflection
    
} // namespace VE
