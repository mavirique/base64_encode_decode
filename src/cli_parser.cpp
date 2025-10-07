#include <iostream>

#include "cli_parser.hpp"

// Main parsing function definition
std::expected<CommandVariant, std::string> parse_cli(const std::vector<std::string_view>& args) {
    if (args.empty()) {
        return std::unexpected("No arguments provided. Use --help for usage.");
    }

    // A simple, direct parsing loop
    for (size_t i = 0; i < args.size(); ++i) {
        const std::string_view arg = args[i];

        if (arg == "-h" || arg == "--help") {
            return OperationMode::Help;
        }

        if (arg == "-e" || arg == "--encode") {
            if (i + 1 >= args.size()) {
                return std::unexpected("Error: --encode flag requires an input string.");
            }

            // Using designated initializers for clarity
            return EncodeCommand { .input = std::string(args[i + 1]) };
        }

        if (arg == "-d"|| arg == "--decode") {
            if (i + 1 >= args.size()) {
                return std::unexpected("Error: --decode flag requires an input string.");
            }

            return DecodeCommand { .input = std::string(args[i + 1]) };
        }
    }

    return std::unexpected("No valid operation specified or invalid argument provided.");
}

// Help function
void print_usage(std::ostream& outputstream) {
    outputstream << "Usage:\n"
                 << " base64_tool --encode <text_to_encode>\n"
                 << " base64_tool --decode <text_to_decode>\n\n"
                 << "Options:\n"
                 << " -e, --encode  Encode the provided string to Base64.\n"
                 << " -d, --decode  Decode the provided Base64 string\n"
                 << " -h, --help    Show this help message.\n";
}