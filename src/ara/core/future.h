#include "error_code.h"
#include <future>
namespace ara {
    namespace core {
        //forward declaration so Promise can use the type
        template<typename T, typename E = ErrorCode>
        class Future;

        template<typename T, typename E = ErrorCode>
        class Promise: public std::promise<T> {
            public:
            Future<T, E> get_future() {
                return Future(std::promise<T>::get_future());
            }
        };

        template<typename T, typename E = ErrorCode>
        class Future {
            friend Promise<T, E>;
            
            private:
            std::future<T> future;
            Future(std::future<T> result_future): future{result_future} { }
        };
    }
}