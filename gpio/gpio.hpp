#ifndef GPIO_HPP_
#define GPIO_HPP_

#include <vector>
#include <map>
#include <fstream>

typedef std::ofstream IO;

class GPIO_OUT {
private:
    std::map<int, IO> iofiles;
public:
    GPIO_OUT(const std::vector<int> &ios);
    ~GPIO_OUT();
    IO& operator[](int io);
    void release_ios();
};

#endif //GPIO_HPP_
