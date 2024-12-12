#include "runtime/core/meta/reflection/reflection.h"
#include "runtime/core/meta/reflection/reflection_register.h"


namespace VE
{
    namespace Reflection
    {
        void TypeMetaRegister::Unregister() { TypeMetaRegisterinterface::unregisterAll(); }
    } // namespace Reflection
    
} // namespace VE
