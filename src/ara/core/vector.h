#include <vector>
namespace ara {
    namespace core {
        //[SWS_CORE_01301], [SWS_CORE_11300]
        template<typename T, typename Allocator = std::allocator<T>>
        using Vector = std::vector<T, Allocator>;
    }
}