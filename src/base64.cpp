#include <array>
#include <cstdint>
#include <span>
#include <vector>

#include "base64.hpp"


namespace base64 {
    
    // Anonymous namespace to keep these details private to this file
    namespace {
        constexpr std::string_view base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
        // A constantexpr function to build our revers lookup table at compile time
        constexpr auto build_decoding_table() {
            std::array<int, 256> table{};
            // Initialize all entries to -1 (invalid)
            table.fill(-1);
        
            // Map the Base64 characters to their 6-bit values
            for (int i = 0; i < 64; ++i) {
                table[static_cast<size_t>(base64_chars[i])] = i;
            }
        
            return table;
        }
    
        constexpr auto decoding_table = build_decoding_table();
    }
    
    std::string encode(std::string_view str) {
        std::span<const std::byte> data(reinterpret_cast<const std::byte*>(str.data()), str.size());
    
        std::string result;
        result.reserve(((data.size() + 2) / 3) *4);
    
        size_t i = 0;
        while (i + 2 < data.size()) {
            uint32_t combined = (static_cast<uint32_t>(data[i])    << 16) |
                                (static_cast<uint32_t>(data[i + 1]) << 8) |
                                (static_cast<uint32_t>(data[i + 2]));
    
            result += base64_chars[(combined >> 18) & 0x3F];
            result += base64_chars[(combined >> 12) & 0x3F];
            result += base64_chars[(combined >> 6)  & 0x3F];
            result += base64_chars[combined         & 0x3F];
    
            i += 3;
        }
    
        if (i < data.size()) {
            uint32_t combined = (static_cast<uint32_t>(data[i]) << 16);
            // Two bytes remain
            if (i + 1 < data.size()) {
                combined |= (static_cast<uint32_t>(data[i + 1]) << 8);
                result += base64_chars[(combined >> 18) & 0x3F];
                result += base64_chars[(combined >> 12) & 0x3F];
                result += base64_chars[(combined >> 6)  & 0x3F];
                result += '=';
            // One byte remains
            } else {
                result += base64_chars[(combined >> 18) & 0x3F];
                result += base64_chars[(combined >> 12) & 0x3F];
                result += "==";
            }
        }
    
        return result;
    }
    
    std::optional<std::string> decode(std::string_view data) {
        if (data.length() % 4 != 0) {
            // Invalid length
            return std::nullopt;
        }
    
        std::vector<std::byte> byte_result;
        byte_result.reserve((data.length() / 4) * 3);
    
        std::array<int, 4> char_values{};
    
        for (size_t i = 0; i < data.length(); i += 4) {
            for (size_t j = 0; j < 4; ++j) {
                if (data[i + j] == '=') {
                    // Padding is handled later, treat as 0 for now.
                    char_values[j] = 0;
                } else {
                    char_values[j] = decoding_table[static_cast<unsigned char>(data[i + j])];
                    if (char_values[j] == -1) {
                        // Invalid character
                        return std::nullopt;
                    }
                }
            }
    
            uint32_t combined = (char_values[0] << 18) | (char_values[1] << 12) |
                                (char_values[2] << 6)  | (char_values[3]);
    
            byte_result.push_back(static_cast<std::byte>((combined >> 16) & 0xFF));
            if (data[i + 2] != '=') {
                byte_result.push_back(static_cast<std::byte>((combined >> 8) & 0xFF));
            }
    
            if (data[i + 3] != '=') {
                byte_result.push_back(static_cast<std::byte>(combined & 0xFF));
            }
        }
    
        return std::string(reinterpret_cast<const char*>(byte_result.data()), byte_result.size());
    }
}
