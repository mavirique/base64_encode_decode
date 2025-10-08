#include "base64.hpp"

#include <array>
#include <cstdint>
#include <vector>

namespace base64 {
    
    // Anonymous namespace to keep implementation details private to this file.
    namespace {
        constexpr std::string_view base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";

        // This compile-time function builds a reverse lookup table for decoding.
        constexpr auto build_decoding_table() {
            std::array<int8_t, 256> table{};
            table.fill(-1); // -1 indicates an invalid Base64 character.
            for (int i = 0; i < 64; ++i) {
                table[static_cast<size_t>(base64_chars[i])] = i;
            }
            return table;
        }
    
        constexpr auto decoding_table = build_decoding_table();
    }
    
    std::string encode(std::span<const std::byte> data) {
        std::string encoded_string;
        // Pre-allocate memory for efficiency, avoiding reallocations.
        encoded_string.reserve(((data.size() + 2) / 3) * 4);
    
        size_t i = 0;
        // Process full 3-byte (24-bit) chunks.
        while (i + 2 < data.size()) {
            uint32_t byte_chunk = (static_cast<uint32_t>(data[i])     << 16) |
                                  (static_cast<uint32_t>(data[i + 1]) << 8)  |
                                  (static_cast<uint32_t>(data[i + 2]));
    
            encoded_string += base64_chars[(byte_chunk >> 18) & 0x3F];
            encoded_string += base64_chars[(byte_chunk >> 12) & 0x3F];
            encoded_string += base64_chars[(byte_chunk >> 6)  & 0x3F];
            encoded_string += base64_chars[byte_chunk         & 0x3F];
            i += 3;
        }
    
        // Handle the final 1 or 2 bytes and add padding.
        if (i < data.size()) {
            uint32_t final_chunk = static_cast<uint32_t>(data[i]) << 16;
            if (i + 1 < data.size()) { // Two bytes remaining
                final_chunk |= static_cast<uint32_t>(data[i + 1]) << 8;
                encoded_string += base64_chars[(final_chunk >> 18) & 0x3F];
                encoded_string += base64_chars[(final_chunk >> 12) & 0x3F];
                encoded_string += base64_chars[(final_chunk >> 6)  & 0x3F];
                encoded_string += '=';
            } else { // One byte remaining
                encoded_string += base64_chars[(final_chunk >> 18) & 0x3F];
                encoded_string += base64_chars[(final_chunk >> 12) & 0x3F];
                encoded_string += "==";
            }
        }
    
        return encoded_string;
    }

    std::optional<std::vector<std::byte>> decode(std::string_view data) {
        // 1. Handle the valid case of an empty string first.
        if (data.empty()) {
            return std::vector<std::byte>{};
        }
    
        // 2. Now, for non-empty strings, validate the length.
        if (data.length() % 4 != 0) {
            return std::nullopt;
        }
    
        // --- Find and validate padding ---
        size_t padding_count = 0;
        if (data.back() == '=') {
            padding_count++;
        }
        if (data.length() >= 2 && data[data.length() - 2] == '=') {
            padding_count++;
        }
    
        // A string of only padding ("==" or "====") is invalid.
        if (data.length() - padding_count == 0 && padding_count > 0) {
            return std::nullopt;
        }
    
        std::vector<std::byte> decoded_bytes;
        decoded_bytes.reserve(data.length() / 4 * 3);
    
        uint32_t bit_chunk = 0;
        int bits_collected = 0;
    
        // Process all characters up to the padding.
        for (size_t i = 0; i < data.length() - padding_count; ++i) {
            const char character = data[i];
            const int8_t value = decoding_table[static_cast<unsigned char>(character)];
    
            if (value == -1) { // Invalid character.
                return std::nullopt;
            }
    
            // --- Strict RFC 4648 padding check ---
            if (padding_count == 1 && i == data.length() - 2) {
                if ((value & 0x03) != 0) return std::nullopt;
            }
            if (padding_count == 2 && i == data.length() - 3) {
                if ((value & 0x0F) != 0) return std::nullopt;
            }
    
            bit_chunk = (bit_chunk << 6) | value;
            bits_collected += 6;
    
            if (bits_collected >= 8) {
                bits_collected -= 8;
                decoded_bytes.push_back(static_cast<std::byte>((bit_chunk >> bits_collected) & 0xFF));
            }
        }
    
        return decoded_bytes;
    }
} // namespace base64
