#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <cstddef>
#include <span>
#include <array>

constexpr std::string_view base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::string base64_encode(std::span<const std::byte> data) {
    std::string result;
    // Pre-calculate and reserve momory to avoid reallocations.
    // Each 3-byte group becomes 4 chars. Ceiling division is (a + b - 1) / b.
    result.reserve(((data.size() + 2) / 3) * 4);

    size_t i = 0;
    // Process full 3-byte chucks
    while (i + 2 < data.size()) {
        // Combine 3 bytes (24 bits) into a single integer.
        uint32_t combined = (static_cast<uint32_t>(data[i])   << 16 ) |
                            (static_cast<uint32_t>(data[i+1]) << 8)   |
                            (static_cast<uint32_t>(data[i+2]));
        
        // Extract four 6-bit indices and append the corresponding chars.
        result += base64_chars[(combined >> 18) & 0x3F];
        result += base64_chars[(combined >> 12) & 0x3F];
        result += base64_chars[(combined >> 6)  & 0x3F];
        result += base64_chars[combined & 0x3F];

        i += 3;
    }

    // Handle padding for the last 1 or 2 bytes.
    if (i < data.size()) {
        uint32_t combined = (static_cast<uint32_t>(data[i]) << 16);

        if (i + 1 < data.size()) { // Two bytes remain
            combined |= (static_cast<uint32_t>(data[i+1]) << 8);
            result += base64_chars[(combined >> 18) & 0x3F];
            result += base64_chars[(combined >> 12) & 0x3F];
            result += base64_chars[(combined >> 6)  & 0x3F];
            result += '=';
        } else { // One byte remains
            result += base64_chars[(combined >> 18) & 0x3F];
            result += base64_chars[(combined >> 12) & 0x3F];
            result += "==";
        }
    }

    return result;
}

// A constexpr function to build our reverse lookup table at compile time
constexpr auto build_decoding_table() {
    std::array<int, 256> table{};

    //Initialize all entries to -1 (invalid)
    for (int i = 0; i < 256; ++i) {
        table[i] = -1;
    }

    // Map the Base64 characters to their 6-bit values
    for (int i = 0; i < 64; ++i) {
        table[static_cast<size_t>(base64_chars[i])] = i;
    }

    return table;
}

constexpr auto decoding_table = build_decoding_table();

std::vector<std::byte> base64_decode(std::string_view data) {
    // 1. Input Validation
    if (data.length() % 4 != 0) {
        return {}; // Invalid length, return empty vector to signal error
    }

    std::vector<std::byte> result;
    result.reserve((data.length() / 4) * 3);

    std::array<int, 4> char_values{};

    // 2. Main Loop (Process 4 characters at a time)
    for (size_t i = 0; i < data.length(); i += 4) {
        // Lookup the 6-bit value for each of the characters
        for (size_t j = 0; j < 4; ++j) {
            // Padding characters are handled later. For now, treat as 0
            if (data[i + j] == '=') {
                char_values[j] = 0;
            } else {
                char_values[j] = decoding_table[static_cast<unsigned char>(data[i + j])];
                // If any char is not a valid Base64 char, abort
                if (char_values[j] == -1) {
                    // Malformed input
                    return {};
                }
            }
        }

        // Combined the four 6-bit values into a 24-bit integer
        uint32_t combined = (char_values[0] << 18) | (char_values[1] << 12 ) |
                            (char_values[2] << 6)  | (char_values[3]);

        // 3. Extract Bytes & Handle Padding
        result.push_back(static_cast<std::byte>((combined >> 16) & 0xFF));

        // If the second-to-last char is not padding, extract the second byte
        if (data[i + 2] != '=') {
            result.push_back(static_cast<std::byte>((combined >> 8) & 0xFF));
        }

        // If the last char is not padding, extract the third byte.
        if (data[i + 3] != '=') {
            result.push_back(static_cast<std::byte>(combined & 0xFF));
        }
    }

    return result;
}

// Main Application Login
void print_usage() {
    std::cerr << "Usage: b64tool [-e | -d] \"text\"\n";
    std::cerr << "   -e: encode string to Base64\n";
    std::cerr << "   -d: decode string from Base64\n";
}

int main(int argc, char* argv[]) {
    // We expect exactly 3 arguments: ./b64tool, blag, data
    if (argc != 3) {
        print_usage();
        return 1; // Return an error code
    }

    std::string_view mode = argv[1];
    std::string_view data = argv[2];

    if (mode == "-e") {
        auto bytes = std::as_bytes(std::span{data});
        std::cout << base64_encode(bytes) << std::endl;
    } else if (mode == "-d") {
        auto decoded_bytes = base64_decode(data);
        if (decoded_bytes.empty()) {
            std::cerr << "Error: Invalid Base64 input string.\n";
            return 1;
        }

        // Convert the vector of bytes back to string to print it
        for (const auto& byte: decoded_bytes) {
            std::cout << static_cast<char>(byte);
        }
        std::cout << std::endl;
    } else {
        std::cerr << "Error: Unknown mode '" << mode << "'\n";
        print_usage();
        return 1;
    }

    return 0; // Success
}