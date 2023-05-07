#include "gpio.hpp"

#include <fstream>

GPIO_OUT::GPIO_OUT(const std::vector<int> &ios): iofiles{} {
    for (auto io: ios) {
        std::ofstream export_io("/sys/class/gpio/export");
        export_io << io;
        export_io.close();

        std::string io_path{"/sys/class/gpio/gpio" + std::to_string(io)};
        std::ofstream io_direction(io_path + "/direction");
        io_direction << "out";
        io_direction.close();

        iofiles[io].open(io_path + "/value");
    }
}

GPIO_OUT::~GPIO_OUT() {
    release_ios();
}

IO& GPIO_OUT::operator[](int io) {
    return iofiles[io];
}

void GPIO_OUT::release_ios() {
    for (auto &pair: iofiles) {
        pair.second.close();

        std::ofstream unexport_io("/sys/class/gpio/unexport");
        unexport_io << pair.first;
        unexport_io.close();
    }
    iofiles.clear();
}