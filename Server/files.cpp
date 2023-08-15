#include <files.hpp>
#include <fstream>

bool files::Read(const std::string& filepath, std::vector<uint8_t>* buffer) {
    std::ifstream stream(filepath, std::ios::binary);
    if (!stream.is_open()) {
        return false;
    }

    buffer->assign((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    stream.close();
    return !buffer->empty();
}
