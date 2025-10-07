# Base64 Encoder/Decoder in C++23
A command-line tool for Base64 encoding and decoding, written in modern C++23 with a focus on security and performance.

## 🌟 Features
C++23 Standard: Utilizes the latest C++ features for robust and clean code.

Secure: Implements secure coding best practices to prevent common vulnerabilities.

No Dependencies: A self-contained executable with no external library requirements.

Cross-Platform: Designed to be compiled and run on various operating systems.

## 🚀 Getting Started
Prerequisites
A C++23 compliant compiler (e.g., GCC 13+, Clang 16+, MSVC v19.33+).

CMake (version 3.23 or later).

### Building the Project
Clone the repository and build the project:

```Bash

git clone https://github.com/mavirique/base64_encode_decode.git
cd base64_encode_decode
Configure and build with CMake:

mkdir build && cd build
cmake ..
cmake --build .
```

### Run the executable:
The executable will be located in the build directory.
```Bash
./base64_tool
```

### 💻 Usage
The tool can be used for both encoding and decoding from the command line.

### Encoding
To encode a string, use the -e or --encode flag:

```Bash
./build/base64_tool -e "Hello, C++23!"
```
### Decoding
To decode a Base64 string, use the -d or --decode flag:

```Bash
./build/base64_tool -d "SGVsbG8sIEMrKzIzIQ=="
```

## 🔒 Secure Coding Considerations
Input Validation: All input is rigorously validated to prevent buffer overflows and other parsing-related vulnerabilities.

Memory Management: Modern C++ smart pointers (std::unique_ptr, std::shared_ptr) and containers (std::vector, std::string_view) are used to ensure memory safety.

Constant-Time Operations: While not critical for Base64, for cryptographic operations, it's important to use constant-time algorithms to prevent timing attacks.

Secure C++ Core Guidelines: The project adheres to the C++ Core Guidelines, particularly the rules related to security.

## 🤝 Contributing
Contributions are welcome! Please feel free to submit a pull request or open an issue.

### 📜 License
This project is licensed under the MIT License - see the LICENSE file for details.