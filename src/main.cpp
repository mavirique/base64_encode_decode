#include "base64.hpp"
#include <iostream>
#include <string_view>
#include <span>

void print_usage() {
    std::cerr << "Usage: b64tool [-e | -d] \"text\"\n";
    std::cerr << "   -e: encode string to Base64\n";
    std::cerr << "   -d: decode string from Base64\n";
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        print_usage();
        return 1;
    }

    std::string_view mode = argv[1];
    std::string_view data = argv[2];

    if (mode == "-e") {
        auto bytes = std::as_bytes(std::span{data});
        std::cout << base64_encode(bytes) << std::endl;
    } else if (mode == "-d") {
        auto decoded_bytes_opt = base64_decode(data);
        if (!decoded_bytes_opt) {
            std::cerr << "Error: Invalid Base64 input string\n";
            return 1;
        }

        // Convert the vector of bytes back to string to print it
        for (const auto& byte : *decoded_bytes_opt) {
            std::cout << static_cast<char>(byte);
        }

        std::cout << std::endl;
    } else {
        std::cerr << "Error: Unknown mode '" << mode << "'\n";
        print_usage();
        return 1;
    }

    return 0;
}