#if __has_include("srell.hpp")
    #include "srell.hpp"
    #define USE_POWERFUL_REGEX 1
    #pragma message("SUCCESS: Compiling with powerful SRELL regex engine. Lookbehinds will work.")
#else
    #include <regex>
    #define USE_POWERFUL_REGEX 0
    #pragma message("WARNING: srell.hpp not found. Falling back to limited std::regex. Lookbehinds will NOT work.")
#endif

#include <algorithm>
#include <any>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

// Function to escape special characters for regex
std::string escapeRegex(const std::string& str) {
    static const std::regex specialChars{R"([-[\]{}()*+?.,\^$|#\s])"};
    return std::regex_replace(str, specialChars, R"(\$&)");
}
// Function to split a string based on delimiters
std::vector<std::string> LoopParseFunc(const std::string& var, const std::string& delimiter1 = "", const std::string& delimiter2 = "") {
    std::vector<std::string> items;
    if (delimiter1.empty() && delimiter2.empty()) {
        // If no delimiters are provided, return a list of characters
        for (char c : var) {
            items.push_back(std::string(1, c));
        }
    } else {
        // Escape delimiters for regex
        std::string escapedDelimiters = escapeRegex(delimiter1 + delimiter2);
        // Construct the regular expression pattern for splitting the string
        std::string pattern = "[" + escapedDelimiters + "]+";
        std::regex regexPattern(pattern);
        std::sregex_token_iterator iter(var.begin(), var.end(), regexPattern, -1);
        std::sregex_token_iterator end;
        while (iter != end) {
            items.push_back(*iter++);
        }
    }
    return items;
}

// Print function for const char*
void print(const char* value) {
    std::cout << std::string(value) << std::endl;
}
// Handle signed 8-bit integers
void print(int8_t value) {
    std::cout << static_cast<int>(value) << std::endl;
}
// Handle unsigned 8-bit integers
void print(uint8_t value) {
    std::cout << static_cast<unsigned int>(value) << std::endl;
}
// Generic print function fallback
template <typename T>
void print(const T& value) {
    std::cout << value << std::endl;
}

// Convert std::string to int
int INT(const std::string& str) {
    std::istringstream iss(str);
    int value;
    iss >> value;
    return value;
}

// Convert various types to std::string
std::string STR(int value) {
    return std::to_string(value);
}
// Convert various types to std::string
std::string STR(long long value) {
    return std::to_string(value);
}
std::string STR(float value) {
    return std::to_string(value);
}
std::string STR(double value) {
    return std::to_string(value);
}
std::string STR(size_t value) {
    return std::to_string(value);
}
std::string STR(bool value) {
    return value ? "1" : "0";
}
std::string STR(const char* value) {
    return std::string(value);
}
std::string STR(const std::string& value) {
    return value;
}

// Function to find the position of needle in haystack (std::string overload)
int InStr(const std::string& haystack, const std::string& needle) {
    size_t pos = haystack.find(needle);
    return (pos != std::string::npos) ? static_cast<int>(pos) + 1 : 0;
}

std::string FileRead(const std::string& path) {
    // This function relies on <fstream>, which is already in your global includes.
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open the file: " + path);
    }
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + '\n';
    }
    file.close();
    return content;
}

bool FileAppend(const std::string& content, const std::string& path) {
    std::ofstream file;
    // Open the file in append mode
    file.open(path, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file for appending." << std::endl;
        return false;
    }
    // Append the content to the file
    file << content;
    // Close the file
    file.close();
    return true;
}

bool FileDelete(const std::string& path) {
    return std::remove(path.c_str()) == 0;
}

size_t StrLen(const std::string& str) {
    return str.length();
}

int Asc(const std::string& str) {
    if (!str.empty()) {
        return static_cast<int>(str[0]);
    }
    return -1; // Return -1 if the string is empty
}

std::string SubStr(const std::string& str, int startPos, int length = -1) {
    std::string result;
    size_t strLen = str.size();
    // Handle negative starting positions (counting from the end)
    if (startPos < 0) {
        startPos = strLen + startPos;
        if (startPos < 0) startPos = 0;  // Ensure it doesn't go beyond the start of the string
    }
    else {
        startPos -= 1; // Convert to 0-based index for internal operations
    }
    // Handle length
    if (length < 0) {
        length = strLen - startPos; // Length to the end of the string
    } else if (startPos + length > static_cast<int>(strLen)) {
        length = strLen - startPos; // Adjust length to fit within the string
    }
    // Extract the substring
    result = str.substr(startPos, length);
    return result;
}

std::string Trim(const std::string &inputString) {
    if (inputString.empty()) return "";
    size_t start = inputString.find_first_not_of(" \t\n\r\f\v");
    size_t end = inputString.find_last_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : inputString.substr(start, end - start + 1);
}

std::string StrReplace(const std::string &originalString, const std::string &find, const std::string &replaceWith) {
    std::string result = originalString;
    size_t pos = 0;
    while ((pos = result.find(find, pos)) != std::string::npos) {
        result.replace(pos, find.length(), replaceWith);
        pos += replaceWith.length();
    }
    return result;
}

std::string StringTrimLeft(const std::string &input, int numChars) {
    return (numChars <= input.length()) ? input.substr(numChars) : input;
}

std::string StringTrimRight(const std::string &input, int numChars) {
    return (numChars <= input.length()) ? input.substr(0, input.length() - numChars) : input;
}

std::string StrLower(const std::string &string) {
    std::string result = string;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string StrSplit(const std::string &inputStr, const std::string &delimiter, int num) {
    size_t start = 0, end = 0, count = 0;
    while ((end = inputStr.find(delimiter, start)) != std::string::npos) {
        if (++count == num) {
            return inputStr.substr(start, end - start);
        }
        start = end + delimiter.length();
    }
    if (count + 1 == num) {
        return inputStr.substr(start);
    }
    return "";
}

std::string Chr(int number) {
    return (number >= 0 && number <= 0x10FFFF) ? std::string(1, static_cast<char>(number)) : "";
}

int Mod(int dividend, int divisor) {
    return dividend % divisor;
}

// Function to check if the operating system is Windows
bool isWindows() {
    #ifdef _WIN32
        return true;
    #else
        return false;
    #endif
}

#ifdef _WIN32
    #define ARGC __argc
    #define ARGV __argv
#else
    extern char **environ;
    int ARGC;
    char** ARGV;
    __attribute__((constructor)) void init_args(int argc, char* argv[], char* envp[]) {
        ARGC = argc;
        ARGV = argv;
    }
#endif
std::string GetParams() {
    // [FIX] This function is now safe as it does not use std::filesystem.
    std::vector<std::string> params;
    for (int i = 1; i < ARGC; ++i) {
        params.push_back(ARGV[i]);
    }
    std::string result;
    for (const auto& param : params) {
        result += param + "\n";
    }
    return result;
}

std::string RegExReplace(std::string_view inputStr, std::string_view regexPattern, std::string_view replacement) {
#if USE_POWERFUL_REGEX
    // --- SRELL PATH ---
    try {
        const srell::regex re = srell::regex(regexPattern.data(), regexPattern.size());
        return srell::regex_replace(std::string(inputStr), re, std::string(replacement));
    } catch (const srell::regex_error& e) {
        // ERROR IS CAUGHT, BUT WE DO NOTHING. NO MORE MESSAGE.
        return std::string(inputStr); // Return original string on failure
    }
#else
    // --- FALLBACK PATH ---
    try {
        const std::regex re{std::string(regexPattern)};
        return std::regex_replace(std::string(inputStr), re, std::string(replacement));
    } catch (const std::regex_error& e) {
        // ERROR IS CAUGHT, BUT WE DO NOTHING. NO MORE MESSAGE.
        return std::string(inputStr); // Return original string on failure
    }
#endif
}

int RegExMatch(std::string_view haystack, std::string_view needle) {
#if USE_POWERFUL_REGEX
    // --- SRELL PATH ---
    try {
        const srell::regex re = srell::regex(needle.data(), needle.size());
        srell::cmatch match;
        if (srell::regex_search(haystack.data(), haystack.data() + haystack.size(), match, re)) {
            return match.position(0) + 1;
        }
    } catch (const srell::regex_error& e) {
        // ERROR IS CAUGHT, BUT WE DO NOTHING. NO MORE MESSAGE.
    }
#else
    // --- FALLBACK PATH ---
    try {
        const std::regex re{std::string(needle)};
        std::match_results<std::string_view::const_iterator> match;
        if (std::regex_search(haystack.begin(), haystack.end(), match, re)) {
            return match.position(0) + 1;
        }
    } catch (const std::regex_error& e) {
        // ERROR IS CAUGHT, BUT WE DO NOTHING. NO MORE MESSAGE.
    }
#endif
    return 0; // Return 0 on failure
}

void HTVM_Append(std::vector<std::string>& arr, const std::string& value) {
    arr.push_back(value);
}
void HTVM_Append(std::vector<std::string>& arr, const char* value) {
    arr.push_back(std::string(value));
}
void HTVM_Append(std::vector<int>& arr, int value) {
    arr.push_back(value);
}
void HTVM_Append(std::vector<float>& arr, float value) {
    arr.push_back(value);
}
void HTVM_Append(std::vector<bool>& arr, bool value) {
    arr.push_back(value);
}

void HTVM_Pop(std::vector<std::string>& arr) {
    if (!arr.empty()) arr.pop_back();
}
void HTVM_Pop(std::vector<int>& arr) {
    if (!arr.empty()) arr.pop_back();
}
void HTVM_Pop(std::vector<float>& arr) {
    if (!arr.empty()) arr.pop_back();
}
void HTVM_Pop(std::vector<bool>& arr) {
    if (!arr.empty()) arr.pop_back();
}

size_t HTVM_Size(const std::vector<std::string>& arr) {
    return arr.size();
}
size_t HTVM_Size(const std::vector<int>& arr) {
    return arr.size();
}
size_t HTVM_Size(const std::vector<float>& arr) {
    return arr.size();
}
size_t HTVM_Size(const std::vector<bool>& arr) {
    return arr.size();
}


#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstdint>
// POSIX permissions support
#ifdef __unix__
#include <sys/stat.h>
#endif
/**
 * @brief Writes raw bytes to a file and optionally makes it executable.
 *
 * Each element of @p hex_bytes is trimmed, lowercased, and converted from a
 * two‑character hex string (e.g., "FF", "03") into its raw byte value.
 * The file is created/truncated and written in binary mode.
 *
 * If @p make_executable is true (default) and the operating system is POSIX,
 * the file’s permissions are set to 0755 (rwxr-xr-x) after writing.
 *
 * @param filename        Path to the output file.
 * @param hex_bytes       Vector of hex strings, one byte each.
 * @param make_executable Whether to set executable permissions on POSIX systems.
 * @throws std::invalid_argument if a hex string is invalid.
 * @throws std::ios_base::failure if the file cannot be opened.
 * @throws std::runtime_error if chmod fails (POSIX only).
 */
void final_assemble(const std::string& filename,
                    const std::vector<std::string>& hex_bytes,
                    bool make_executable = true) {
    // Open the file in binary mode, overwriting any existing content
    std::ofstream out(filename, std::ios::binary | std::ios::out | std::ios::trunc);
    if (!out) {
        throw std::ios_base::failure("Cannot open file: " + filename);
    }
    // Write each byte
    for (const auto& raw : hex_bytes) {
        // 1. Trim whitespace
        auto start = raw.find_first_not_of(" \t\n\r\f\v");
        if (start == std::string::npos) {
            throw std::invalid_argument("Hex string is empty or only whitespace.");
        }
        auto end = raw.find_last_not_of(" \t\n\r\f\v");
        std::string trimmed = raw.substr(start, end - start + 1);
        // 2. Lowercase (optional, stoul handles both cases)
        std::transform(trimmed.begin(), trimmed.end(), trimmed.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        // 3. Validate length (exactly 2 hex digits)
        if (trimmed.length() != 2) {
            throw std::invalid_argument(
                "Hex string must be exactly 2 characters: \"" + trimmed + "\"");
        }
        // 4. Convert to unsigned long (0x00–0xFF)
        unsigned long byte_val;
        try {
            byte_val = std::stoul(trimmed, nullptr, 16);
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Invalid hex characters in: \"" + trimmed + "\"");
        } catch (const std::out_of_range&) {
            throw std::invalid_argument("Hex value out of range (00..FF): \"" + trimmed + "\"");
        }
        if (byte_val > 0xFF) {
            throw std::invalid_argument("Byte value exceeds 0xFF: \"" + trimmed + "\"");
        }
        // 5. Write raw byte
        out.put(static_cast<unsigned char>(byte_val));
    }
    // Close the file (important before changing permissions)
    out.close();
    if (!out) {
        throw std::ios_base::failure("Failed to close file after writing.");
    }
    // Make executable on POSIX systems if requested
#ifdef __unix__
    if (make_executable) {
        // 0755 = rwxr-xr-x
        if (chmod(filename.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) != 0) {
            throw std::runtime_error("chmod failed on file: " + filename);
        }
    }
#else
    // On non‑POSIX systems this flag is silently ignored (no error)
    (void)make_executable;
#endif
}
std::vector<std::string> size_to_hex32(uint32_t size_in_bytes) {
    std::vector<std::string> pairs;
    for (int i = 0; i < 4; i++) {
        uint8_t byte = (size_in_bytes >> (i * 8)) & 0xFF;
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
        pairs.push_back(ss.str());
    }
    return pairs;
}
// This function takes a 32-bit input but pads it out to an 8-byte (64-bit) little-endian format.
// It loops 8 times to generate 8 hex pairs, filling the upper 4 bytes with "00".
std::string size_to_hex64(uint64_t size_in_bytes) {
    std::string result;
    for (int i = 0; i < 8; i++) {
        uint8_t byte = (size_in_bytes >> (i * 8)) & 0xFF;
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
        result += ss.str();
        if (i < 7) result += " ";
    }
    return result;
}
std::string address_to_hexstr(uint64_t addr, int min_digits = 0) {
    std::stringstream ss;
    ss << "0x" << std::hex << std::setfill('0');
    if (min_digits > 0) {
        ss << std::setw(min_digits);
    }
    ss << addr;
    return ss.str();
}
std::string char_to_hex(std::string c) {
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c[0];
    return ss.str();
}
///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
int64_t parse_number(const std::string& input) {
    if (input.empty()) {
        throw std::invalid_argument("empty string");
    }
    size_t pos = 0;
    bool negative = false;
    std::string s = input;
    // Check for leading minus sign
    if (s[0] == '-') {
        negative = true;
        s = s.substr(1);
        if (s.empty()) {
            throw std::invalid_argument("invalid number: just '-'");
        }
    }
    // Determine base
    int base = 10;
    if (s.size() >= 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        base = 16;
        s = s.substr(2);
        if (s.empty()) {
            throw std::invalid_argument("invalid hex number: missing digits");
        }
    }
    // Convert the remaining string
    uint64_t value = 0;
    for (char c : s) {
        int digit;
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (base == 16 && c >= 'a' && c <= 'f') {
            digit = c - 'a' + 10;
        } else if (base == 16 && c >= 'A' && c <= 'F') {
            digit = c - 'A' + 10;
        } else {
            throw std::invalid_argument("invalid character '" + std::string(1, c) + "' for base " + std::to_string(base));
        }
        if (value > (UINT64_MAX - digit) / base) { // overflow check
            throw std::out_of_range("number too large for 64-bit");
        }
        value = value * base + digit;
    }
    // Apply sign and cast to int64_t (two's complement)
    if (negative) {
        if (value > static_cast<uint64_t>(INT64_MAX) + 1) {
            throw std::out_of_range("negative number too small (underflow)");
        }
        return -static_cast<int64_t>(value);
    } else {
        if (value > static_cast<uint64_t>(INT64_MAX)) {
            throw std::out_of_range("positive number too large for int64_t");
        }
        return static_cast<int64_t>(value);
    }
}
void print_hex(uint64_t decimal) {
    std::cout << "0x" << std::hex << std::setw(16) << std::setfill('0') << decimal << std::endl;
}
std::string dec_to_hex_pair(uint8_t decimal) {
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)decimal;
    return ss.str();
}
// This function converts a 32-bit decimal number to a 4-byte little-endian hexadecimal string.
// Little Endian means the least significant byte (lowest value) is stored and printed first.
std::string dec_to_hex_4byte_le(uint32_t decimal) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    
    // Extract and format each of the 4 bytes, starting with the lowest byte (Little Endian)
    // The output string will separate each byte with a space exactly as requested.
    ss << std::setw(2) << static_cast<int>((decimal >> 0) & 0xFF) << " "   // Byte 1: Lowest byte
       << std::setw(2) << static_cast<int>((decimal >> 8) & 0xFF) << " "   // Byte 2
       << std::setw(2) << static_cast<int>((decimal >> 16) & 0xFF) << " "  // Byte 3
       << std::setw(2) << static_cast<int>((decimal >> 24) & 0xFF);         // Byte 4: Highest byte
       
    return ss.str();
}
std::string calculate_jmp_string(const std::string& target_hex_str) {
    std::stringstream ss(target_hex_str);
    std::string byte_str;
    uint32_t address = 0;
    int shift = 0;
    
    // 1. Parse the spaced little-endian string into a raw integer
    while (ss >> byte_str) {
        uint32_t byte_val = std::stoul(byte_str, nullptr, 16);
        address |= (byte_val << shift);
        shift += 8;
    }
    
    // 2. Apply your numerical offset subtraction
    // Change '5' to whatever number your compilation pass requires
    uint32_t offset_address = address - 5; 
    
    // 3. Format it back to a spaced 4-byte little-endian hex string
    std::stringstream out_ss;
    out_ss << std::hex << std::setfill('0');
    out_ss << std::setw(2) << ((offset_address >> 0) & 0xFF) << " "
           << std::setw(2) << ((offset_address >> 8) & 0xFF) << " "
           << std::setw(2) << ((offset_address >> 16) & 0xFF) << " "
           << std::setw(2) << ((offset_address >> 24) & 0xFF);
           
    return out_ss.str();
}
void HTVM_Append(std::vector<int64_t>& arr, int64_t value) {
    arr.push_back(value);
}
// OVERLOAD: If the input is already a number (size_t), just return it directly
int INT(size_t num) {
    return static_cast<int>(num);
}
// start of HT-Lib.htvm
// HT-Lib - A Library for Creating Programming Languages
// Copyright (C) 2026  TheMaster1127
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
// global vars NEEDED
int HT_LIB_theIdNumOfThe34 = 0;
std::vector<std::string> HT_Lib_theIdNumOfThe34theVar;
//;;;;;;;;;;;;;;;;;;;;;;;;;
std::string preserveStrings(std::string code, std::string keyWordEscpaeChar = "\\") {
    std::vector<std::string> getAllCharForTheFurtureSoIcanAddEscapeChar;
    std::string ReplaceFixWhitOutFixDoubleQuotesInsideDoubleQuotes = "";
    std::string str21 = "";
    std::string htCodeOUT754754 = "";
    std::string OutFixDoubleQuotesInsideDoubleQuotes = "";
    int fixOutFixDoubleQuotesInsideDoubleQuotesFIXok = 0;
    int removeNexFixkeyWordEscpaeChar = 0;
    int areWEinSome34sNum = 0;
    std::vector<std::string> items1 = LoopParseFunc(code);
    for (size_t A_Index1 = 0; A_Index1 < items1.size(); A_Index1++) {
        std::string A_LoopField1 = items1[A_Index1 - 0];
        HTVM_Append(HT_Lib_theIdNumOfThe34theVar, "");
        HTVM_Append(HT_Lib_theIdNumOfThe34theVar, "");
    }
    std::vector<std::string> items2 = LoopParseFunc(code);
    for (size_t A_Index2 = 0; A_Index2 < items2.size(); A_Index2++) {
        std::string A_LoopField2 = items2[A_Index2 - 0];
        HT_Lib_theIdNumOfThe34theVar[A_Index2] = HT_Lib_theIdNumOfThe34theVar[A_Index2] + Chr(34);
        HTVM_Append(getAllCharForTheFurtureSoIcanAddEscapeChar, A_LoopField2);
    }
    HTVM_Append(getAllCharForTheFurtureSoIcanAddEscapeChar, " ");
    ReplaceFixWhitOutFixDoubleQuotesInsideDoubleQuotes = Chr(34) + "ihuiuuhuuhtheidFor" + str21 + "__" + str21 + "asds" + str21 + "as__" + str21 + "theuhtuwaesphoutr" + Chr(34);
    std::vector<std::string> items3 = LoopParseFunc(code);
    for (size_t A_Index3 = 0; A_Index3 < items3.size(); A_Index3++) {
        std::string A_LoopField3 = items3[A_Index3 - 0];
        if (A_LoopField3 == keyWordEscpaeChar && getAllCharForTheFurtureSoIcanAddEscapeChar[A_Index3 + 1] == Chr(34)) {
            fixOutFixDoubleQuotesInsideDoubleQuotesFIXok = 1;
            OutFixDoubleQuotesInsideDoubleQuotes += ReplaceFixWhitOutFixDoubleQuotesInsideDoubleQuotes;
        } else {
            if (fixOutFixDoubleQuotesInsideDoubleQuotesFIXok != 1) {
                OutFixDoubleQuotesInsideDoubleQuotes += A_LoopField3;
            } else {
                fixOutFixDoubleQuotesInsideDoubleQuotesFIXok = 0;
            }
        }
    }
    code = OutFixDoubleQuotesInsideDoubleQuotes;
    if (keyWordEscpaeChar != Chr(92)) {
        code = StrReplace(code, Chr(92), Chr(92) + Chr(92));
    }
    if (keyWordEscpaeChar == Chr(92)) {
        std::vector<std::string> items4 = LoopParseFunc(code);
        for (size_t A_Index4 = 0; A_Index4 < items4.size(); A_Index4++) {
            std::string A_LoopField4 = items4[A_Index4 - 0];
            if (A_LoopField4 == Chr(34)) {
                areWEinSome34sNum++;
            }
            if (areWEinSome34sNum == 1) {
                if (A_LoopField4 != Chr(34)) {
                    if (A_LoopField4 == keyWordEscpaeChar) {
                        HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] = HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] + Chr(92);
                    } else {
                        HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] = HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] + A_LoopField4;
                    }
                } else {
                    HT_LIB_theIdNumOfThe34++;
                    htCodeOUT754754 += "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(HT_LIB_theIdNumOfThe34) + Chr(65) + Chr(65);
                }
            }
            if (areWEinSome34sNum == 2 || areWEinSome34sNum == 0) {
                if (A_LoopField4 != Chr(34)) {
                    htCodeOUT754754 += A_LoopField4;
                }
                areWEinSome34sNum = 0;
            }
        }
    } else {
        std::vector<std::string> items5 = LoopParseFunc(code);
        for (size_t A_Index5 = 0; A_Index5 < items5.size(); A_Index5++) {
            std::string A_LoopField5 = items5[A_Index5 - 0];
            if (A_LoopField5 == Chr(34)) {
                areWEinSome34sNum++;
            }
            if (areWEinSome34sNum == 1) {
                if (A_LoopField5 != Chr(34)) {
                    if (A_LoopField5 == keyWordEscpaeChar && keyWordEscpaeChar == getAllCharForTheFurtureSoIcanAddEscapeChar[A_Index5 + 1]) {
                        HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] = HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] + keyWordEscpaeChar;
                        removeNexFixkeyWordEscpaeChar = 1;
                    }
                    else if (A_LoopField5 == keyWordEscpaeChar) {
                        if (removeNexFixkeyWordEscpaeChar != 1) {
                            HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] = HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] + Chr(92);
                        } else {
                            removeNexFixkeyWordEscpaeChar = 0;
                        }
                    } else {
                        HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] = HT_Lib_theIdNumOfThe34theVar[HT_LIB_theIdNumOfThe34] + A_LoopField5;
                    }
                } else {
                    HT_LIB_theIdNumOfThe34++;
                    htCodeOUT754754 += "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(HT_LIB_theIdNumOfThe34) + Chr(65) + Chr(65);
                }
            }
            if (areWEinSome34sNum == 2 || areWEinSome34sNum == 0) {
                if (A_LoopField5 != Chr(34)) {
                    htCodeOUT754754 += A_LoopField5;
                }
                areWEinSome34sNum = 0;
            }
        }
    }
    code = htCodeOUT754754;
    for (int A_Index6 = 0; A_Index6 < HT_LIB_theIdNumOfThe34; A_Index6++) {
        HT_Lib_theIdNumOfThe34theVar[A_Index6] = HT_Lib_theIdNumOfThe34theVar[A_Index6] + Chr(34);
    }
    HTVM_Append(HT_Lib_theIdNumOfThe34theVar, Chr(34));
    return code;
}
//;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;
std::string restoreStrings(std::string codeOUT, std::string keyWordEscpaeChar = Chr(92)) {
    codeOUT = StrReplace(codeOUT, Chr(34) + "ihuiuuhuuhtheidFor_" + Chr(95) + "asdsas__theuhtuwaesphoutr" , keyWordEscpaeChar);
    for (int A_Index7 = 0; A_Index7 < HT_LIB_theIdNumOfThe34; A_Index7++) {
        if (HT_LIB_theIdNumOfThe34 == A_Index7 + 1) {
            codeOUT = StrReplace(codeOUT, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(A_Index7 + 1) + Chr(65) + Chr(65), StrReplace(HT_Lib_theIdNumOfThe34theVar[A_Index7 + 1], keyWordEscpaeChar, "\\") + Chr(34));
        } else {
            codeOUT = StrReplace(codeOUT, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(A_Index7 + 1) + Chr(65) + Chr(65), StrReplace(HT_Lib_theIdNumOfThe34theVar[A_Index7 + 1], keyWordEscpaeChar, "\\"));
        }
    }
    return codeOUT;
}
std::string cleanUpFirst(std::string code) {
    code = StrReplace(code, Chr(13), "");
    std::string out = "";
    std::vector<std::string> items8 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index8 = 0; A_Index8 < items8.size(); A_Index8++) {
        std::string A_LoopField8 = items8[A_Index8 - 0];
        out += Trim(A_LoopField8) + Chr(10);
    }
    out = StringTrimRight(out, 1);
    return out;
}
std::string getLangParams(std::string binName, std::string langExtension, std::string extra = "") {
    std::string params = Trim(GetParams());
    std::string paramsTemp = "";
    if (params == "") {
        if (isWindows()) {
            print("Usage:" + Chr(10) + Trim(binName) + " your_file." + Trim(langExtension) + " " + Trim(extra));
        } else {
            print("Usage:" + Chr(10) + "./" + Trim(binName) + " your_file." + Trim(langExtension) + " " + Trim(extra));
        }
        return "";
    } else {
        return params;
    }
    return "MASSIVE ERROR";
}
void saveOutput(std::string outCode, std::string fileName) {
    FileDelete(Trim(fileName));
    FileAppend(Trim(outCode), Trim(fileName));
    print("Generation finished: " + Trim(fileName) + " generated.");
}
bool beginning(std::string line, std::string what) {
    if (SubStr(line, 1, StrLen(what)) == what) {
        return true;
    }
    return false;
}
std::string formatCurlyBracesForParsing(std::string code, std::string openBrace = "{", std::string closeBrace = "}") {
    code = StrReplace(code, openBrace, Chr(10) + "{" + Chr(10));
    code = StrReplace(code, closeBrace, Chr(10) + "}" + Chr(10));
    code = cleanUpFirst(code);
    return code;
}
std::string handleComments(std::string code, std::string commentKeyword = ";") {
    std::string str1 = "";
    std::vector<std::string> items9 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index9 = 0; A_Index9 < items9.size(); A_Index9++) {
        std::string A_LoopField9 = items9[A_Index9 - 0];
        str1 += StrSplit(A_LoopField9, commentKeyword, 1) + Chr(10);
    }
    code = StringTrimRight(str1, 1);
    return code;
}
// Define the function to check odd spaces at the beginning
std::string CheckOddLeadingSpaces(std::string string123) {
    // Initialize a variable to count the spaces
    int spaceCount = 0;
    // Loop through the string one character at a time
    std::vector<std::string> items10 = LoopParseFunc(string123);
    for (size_t A_Index10 = 0; A_Index10 < items10.size(); A_Index10++) {
        std::string A_LoopField10 = items10[A_Index10 - 0];
        // Check if the current character is a space
        if (A_LoopField10 == Chr(32)) {
            spaceCount++;
        } else {
            // When we hit a non-space character, break the loop
            break;
        }
    }
    // Return true if the number of spaces is odd, false otherwise
    std::string sdsfawasd = STR(Mod(spaceCount, 2) == 1);
    //MsgBox, % sdsfawasd
    return sdsfawasd;
}
std::string RepeatSpaces(int count) {
    std::string spaces = "";
    for (int A_Index11 = 0; A_Index11 < count; A_Index11++) {
        spaces += Chr(32);
    }
    return spaces;
}
// if you wanna convert to python, nim etc... indentation style we set modeCurlyBracesOn to 0
std::string indent_nested_curly_braces(std::string input_string, int modeCurlyBracesOn = 1) {
    int indent_size = 4;
    int current_indent = 0;
    std::string result = "";
    std::string trimmed_line = "";
    std::string resultOut = "";
    std::string culyOpenFix = "{";
    std::string culyCloseFix = "}";
    //MsgBox, % input_string
    std::vector<std::string> items12 = LoopParseFunc(input_string, "\n", "\r");
    for (size_t A_Index12 = 0; A_Index12 < items12.size(); A_Index12++) {
        std::string A_LoopField12 = items12[A_Index12 - 0];
        trimmed_line = Trim(A_LoopField12);
        if (trimmed_line == Chr(123)) {
            result += Chr(32) + RepeatSpaces(current_indent) + trimmed_line + Chr(10);
            current_indent = current_indent + indent_size;
        }
        else if (trimmed_line == Chr(125)) {
            current_indent = current_indent - indent_size;
            result += Chr(32) + RepeatSpaces(current_indent) + trimmed_line + Chr(10);
        } else {
            result += Chr(32) + RepeatSpaces(current_indent) + trimmed_line + Chr(10);
        }
    }
    if (modeCurlyBracesOn == 0) {
        std::string resultOut = "";
        std::vector<std::string> items13 = LoopParseFunc(result, "\n", "\r");
        for (size_t A_Index13 = 0; A_Index13 < items13.size(); A_Index13++) {
            std::string A_LoopField13 = items13[A_Index13 - 0];
            if (Trim(A_LoopField13) != "{" && Trim(A_LoopField13) != "}") {
                resultOut += A_LoopField13 + Chr(10);
            }
        }
        result = StringTrimRight(resultOut, 1);
    } else {
        // format curly braces in a K&R style
        std::vector<std::string> lookIntoFurture;
        std::vector<std::string> items14 = LoopParseFunc(result, "\n", "\r");
        for (size_t A_Index14 = 0; A_Index14 < items14.size(); A_Index14++) {
            std::string A_LoopField14 = items14[A_Index14 - 0];
            HTVM_Append(lookIntoFurture, Trim(A_LoopField14));
        }
        HTVM_Append(lookIntoFurture, " ");
        std::string resultOut = "";
        int skipNext = 0;
        std::vector<std::string> items15 = LoopParseFunc(result, "\n", "\r");
        for (size_t A_Index15 = 0; A_Index15 < items15.size(); A_Index15++) {
            std::string A_LoopField15 = items15[A_Index15 - 0];
            skipNext--;
            if (skipNext <= 0) {
                skipNext = 0;
            }
            if (Trim(lookIntoFurture[A_Index15 + 1]) == "{") {
                resultOut += A_LoopField15 + " " + culyOpenFix + Chr(10);
                skipNext = 2;
            }
            if (skipNext == 0) {
                resultOut += A_LoopField15 + Chr(10);
            }
        }
        result = StringTrimRight(resultOut, 1);
        std::vector<std::string> lookIntoFurture2;
        std::vector<std::string> items16 = LoopParseFunc(result, "\n", "\r");
        for (size_t A_Index16 = 0; A_Index16 < items16.size(); A_Index16++) {
            std::string A_LoopField16 = items16[A_Index16 - 0];
            HTVM_Append(lookIntoFurture2, Trim(A_LoopField16));
        }
        HTVM_Append(lookIntoFurture2, " ");
        resultOut = "";
        skipNext = 0;
        std::string addSpacesAtTheBegginig = "";
        std::vector<std::string> items17 = LoopParseFunc(result, "\n", "\r");
        for (size_t A_Index17 = 0; A_Index17 < items17.size(); A_Index17++) {
            std::string A_LoopField17 = items17[A_Index17 - 0];
            skipNext--;
            if (skipNext <= 0) {
                skipNext = 0;
            }
            if (Trim(A_LoopField17) == "}" && Trim(lookIntoFurture2[A_Index17 + 1]) == "else {") {
                skipNext = 2;
                addSpacesAtTheBegginig = "";
                std::vector<std::string> items18 = LoopParseFunc(A_LoopField17);
                for (size_t A_Index18 = 0; A_Index18 < items18.size(); A_Index18++) {
                    std::string A_LoopField18 = items18[A_Index18 - 0];
                    if (A_LoopField18 == " ") {
                        if (A_LoopField18 != " ") {
                            break;
                        }
                        addSpacesAtTheBegginig += StrReplace(A_LoopField18, "}", culyCloseFix);
                    }
                }
                resultOut += addSpacesAtTheBegginig + culyCloseFix + " else " + culyOpenFix + Chr(10);
            }
            if (skipNext == 0) {
                resultOut += A_LoopField17 + Chr(10);
            }
        }
        result = StringTrimRight(resultOut, 1);
    }
    resultOut = "";
    std::string ALoopField = "";
    std::vector<std::string> items19 = LoopParseFunc(result, "\n", "\r");
    for (size_t A_Index19 = 0; A_Index19 < items19.size(); A_Index19++) {
        std::string A_LoopField19 = items19[A_Index19 - 0];
        if (CheckOddLeadingSpaces(A_LoopField19) == "1") {
            ALoopField = StringTrimLeft(A_LoopField19, 1);
            resultOut += ALoopField + Chr(10);
        } else {
            resultOut += A_LoopField19 + Chr(10);
        }
    }
    result = StringTrimRight(resultOut, 1);
    // Return the result
    return result;
}
// end of HT-Lib.htvm
// =========================================================================
// GLOBAL ASSEMBLER STATE
// =========================================================================
int use_custom_assembler_x86_64_linux = 0;
std::vector<std::string> label_names;
std::vector<int64_t> label_values;
uint64_t current_address = 0ULL;
bool is_first_pass = false;
void add_label(std::string name, int64_t addr) {
    HTVM_Append(label_names, name);
    HTVM_Append(label_values, addr);
}
int64_t get_label_address(std::string name) {
    for (int A_Index20 = 0; A_Index20 < HTVM_Size(label_names); A_Index20++) {
        if (name == label_names[A_Index20]) {
            return label_values[A_Index20];
        }
    }
    return -1;
}
void set_current_address(uint64_t addr) {
    current_address = addr;
}
uint64_t get_current_address() {
    return current_address;
}
void set_first_pass(bool pass_state) {
    is_first_pass = pass_state;
}
int get_byte_count(std::string hex_str) {
    if (hex_str == "NULL" || hex_str == "") {
        return 0;
    }
    int count = 0;
    // Using Parse instead of StrSplit ensures we never go out of bounds
    std::vector<std::string> items21 = LoopParseFunc(hex_str, " ");
    for (size_t A_Index21 = 0; A_Index21 < items21.size(); A_Index21++) {
        std::string A_LoopField21 = items21[A_Index21 - 0];
        if (Trim(A_LoopField21) != "") {
            count++;
        }
    }
    return count;
}
// Helper: convert string to boolean (InStr returns position, so >0 means true)
bool is_mem(std::string op) {
    return (InStr(op, "[") > 0);
}
bool is_imm(std::string op) {
    op = Trim(op);
    if (RegExMatch(op, "^-?[0-9]+$") > 0) {
        return true;
    }
    if (RegExMatch(op, "^-?0x[0-9a-fA-F]+$") > 0) {
        return true;
    }
    if (RegExMatch(op, "^'.*'$") > 0) {
        return true;
    }
    return false;
}
// Returns "id|size" e.g. "0|64" for rax, "1|32" for ecx, "15|8" for r15b
std::string get_reg_info(std::string name) {
    std::vector<std::string> regs_64 = {"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
    std::vector<std::string> regs_32 = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d"};
    std::vector<std::string> regs_16 = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w"};
    std::vector<std::string> regs_8 = {"al", "cl", "dl", "bl", "spl", "bpl", "sil", "dil", "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b"};
    std::vector<std::string> regs_high = {"ah", "ch", "dh", "bh"};
    name = StrReplace(name, "qword ", "");
    name = StrReplace(name, "dword ", "");
    name = StrReplace(name, "word ", "");
    name = StrReplace(name, "byte ", "");
    name = StrReplace(name, "[", "");
    name = StrReplace(name, "]", "");
    name = Trim(name);
    for (int A_Index22 = 0; A_Index22 < HTVM_Size(regs_64); A_Index22++) {
        if (name == regs_64[A_Index22]) {
            return STR(A_Index22) + "|64";
        }
    }
    for (int A_Index23 = 0; A_Index23 < HTVM_Size(regs_32); A_Index23++) {
        if (name == regs_32[A_Index23]) {
            return STR(A_Index23) + "|32";
        }
    }
    for (int A_Index24 = 0; A_Index24 < HTVM_Size(regs_16); A_Index24++) {
        if (name == regs_16[A_Index24]) {
            return STR(A_Index24) + "|16";
        }
    }
    for (int A_Index25 = 0; A_Index25 < HTVM_Size(regs_8); A_Index25++) {
        if (name == regs_8[A_Index25]) {
            return STR(A_Index25) + "|8";
        }
    }
    for (int A_Index26 = 0; A_Index26 < HTVM_Size(regs_high); A_Index26++) {
        if (name == regs_high[A_Index26]) {
            return STR(A_Index26 + 4) + "|8";
        }
    }
    return "-1|0";
}
int get_reg_id(std::string name, std::vector<std::string> registers) {
    name = StrReplace(name, "qword ", "");
    name = StrReplace(name, "dword ", "");
    name = StrReplace(name, "word ", "");
    name = StrReplace(name, "byte ", "");
    name = StrReplace(name, "[", "");
    name = StrReplace(name, "]", "");
    name = Trim(name);
    for (int A_Index27 = 0; A_Index27 < HTVM_Size(registers); A_Index27++) {
        if (name == registers[A_Index27]) {
            return A_Index27;
        }
    }
    return -1;
}
// Helper to map mnemonics to opcodes
int get_mnemonic_info(std::string name, std::string info_type) {
    if (name == "add") {
        if (info_type == "base_op") {
            return 0x00;
        }
        if (info_type == "imm_ext") {
            return 0;
        }
    }
    else if (name == "or") {
        if (info_type == "base_op") {
            return 0x08;
        }
        if (info_type == "imm_ext") {
            return 1;
        }
    }
    else if (name == "adc") {
        if (info_type == "base_op") {
            return 0x10;
        }
        if (info_type == "imm_ext") {
            return 2;
        }
    }
    else if (name == "sbb") {
        if (info_type == "base_op") {
            return 0x18;
        }
        if (info_type == "imm_ext") {
            return 3;
        }
    }
    else if (name == "and") {
        if (info_type == "base_op") {
            return 0x20;
        }
        if (info_type == "imm_ext") {
            return 4;
        }
    }
    else if (name == "sub") {
        if (info_type == "base_op") {
            return 0x28;
        }
        if (info_type == "imm_ext") {
            return 5;
        }
    }
    else if (name == "xor") {
        if (info_type == "base_op") {
            return 0x30;
        }
        if (info_type == "imm_ext") {
            return 6;
        }
    }
    else if (name == "cmp") {
        if (info_type == "base_op") {
            return 0x38;
        }
        if (info_type == "imm_ext") {
            return 7;
        }
    }
    return -1;
}
int get_jcc_code(std::string name) {
    if (name == "jo") {
        return 0;
    }
    if (name == "jno") {
        return 1;
    }
    if (name == "jb" || name == "jc" || name == "jnae") {
        return 2;
    }
    if (name == "jnb" || name == "jnc" || name == "jae") {
        return 3;
    }
    if (name == "je" || name == "jz") {
        return 4;
    }
    if (name == "jne" || name == "jnz") {
        return 5;
    }
    if (name == "jbe" || name == "jna") {
        return 6;
    }
    if (name == "jnbe" || name == "ja") {
        return 7;
    }
    if (name == "js") {
        return 8;
    }
    if (name == "jns") {
        return 9;
    }
    if (name == "jp" || name == "jpe") {
        return 10;
    }
    if (name == "jnp" || name == "jpo") {
        return 11;
    }
    if (name == "jl" || name == "jnge") {
        return 12;
    }
    if (name == "jnl" || name == "jge") {
        return 13;
    }
    if (name == "jle" || name == "jng") {
        return 14;
    }
    if (name == "jg" || name == "jnle") {
        return 15;
    }
    return -1;
}
std::string encode(std::string instruction) {
    bool dest_is_mem = false;
    bool src_is_mem = false;
    bool src_is_imm = false;
    bool mem_is_imm = false;
    bool force_rex = false;
    int disp_size = 0;
    int64_t disp = 0LL;
    uint64_t REX = 0x40;
    uint64_t opcode = 0ULL;
    uint64_t modrm = 0ULL;
    uint64_t sib = 0ULL;
    int64_t imm_val = 0LL;
    std::string out_hex = "";
    std::string dest_op = "";
    std::string src_op = "";
    std::string str0 = "";
    bool has_0F = false;
    instruction = Trim(instruction);
    std::vector<std::string> registers = {"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
    if (instruction == "syscall") {
        return "0f 05";
    }
    if (instruction == "rep movsq") {
        return "f3 48 a5";
    }
    int space_pos = InStr(instruction, " ");
    std::string mnemonic = "mov";
    std::string ops_str = "";
    if (space_pos > 0) {
        mnemonic = Trim(SubStr(instruction, 1, space_pos));
        ops_str = Trim(SubStr(instruction, space_pos + 1));
    }
    int first_comma = InStr(ops_str, ",");
    if (first_comma > 0) {
        dest_op = Trim(SubStr(ops_str, 1, first_comma - 1));
        src_op = Trim(SubStr(ops_str, first_comma + 1));
    } else {
        dest_op = Trim(ops_str);
        src_op = "";
    }
    bool is_imul_3_op = false;
    std::string imul_imm_str = "";
    if (mnemonic == "imul") {
        std::string op3 = Trim(StrSplit(ops_str, ",", 3));
        if (op3 != "") {
            is_imul_3_op = true;
            dest_op = Trim(StrSplit(ops_str, ",", 1));
            src_op = Trim(StrSplit(ops_str, ",", 2));
            imul_imm_str = op3;
        }
    }
    dest_is_mem = InStr(dest_op, "[");
    src_is_mem = InStr(src_op, "[");
    src_is_imm = is_imm(src_op);
    bool dest_is_imm = is_imm(dest_op);
    if (instruction == "ret") {
        if (dest_op == "") {
            return "c3";
        }
        int64_t imm = parse_number(dest_op);
        return "c2 " + dec_to_hex_pair(imm & 0xFF) + " " + dec_to_hex_pair((imm >> 8) & 0xFF);
    }
    if (mnemonic == "push" && dest_is_imm) {
        int64_t imm = parse_number(dest_op);
        if (imm >= -128 && imm <= 127) {
            return "6a " + dec_to_hex_pair(imm & 0xFF);
        }
        else if (imm >= -2147483648 && imm <= 2147483647) {
            return "68 " + dec_to_hex_pair(imm & 0xFF) + " " + dec_to_hex_pair((imm >> 8) & 0xFF) + " " + dec_to_hex_pair((imm >> 16) & 0xFF) + " " + dec_to_hex_pair((imm >> 24) & 0xFF);
        }
        return "NULL";
    }
    if (mnemonic == "int" && dest_is_imm) {
        int64_t imm = parse_number(dest_op);
        return (imm == 3) ? "cc" : ("cd " + dec_to_hex_pair(imm & 0xFF));
    }
    if (dest_is_mem && src_is_mem) {
        return "NULL";
    }
    bool dest_has_high = (dest_op == "ah" || dest_op == "ch" || dest_op == "dh" || dest_op == "bh");
    bool src_has_high = (src_op == "ah" || src_op == "ch" || src_op == "dh" || src_op == "bh");
    bool has_high_byte = (dest_has_high || src_has_high);
    bool is_direct_jump_or_call = false;
    if (mnemonic == "call" || SubStr(mnemonic, 1, 1) == "j") {
        if (RegExMatch(dest_op, "^-?(0x)?[0-9a-fA-F]+$")) {
            is_direct_jump_or_call = true;
        } else {
            int64_t label_addr = get_label_address(dest_op);
            if (label_addr != -1 || is_first_pass) {
                is_direct_jump_or_call = true;
            }
        }
    }
    if (SubStr(mnemonic, 1, 1) == "j" && mnemonic != "jmp") {
        if (is_direct_jump_or_call == false) {
            return "NULL";
        }
    }
    if (is_direct_jump_or_call) {
        int64_t target = 0LL;
        bool is_label = (RegExMatch(dest_op, "^-?(0x)?[0-9a-fA-F]+$") == 0);
        int64_t label_addr = get_label_address(dest_op);
        if (label_addr != -1) {
            target = label_addr;
        } else {
            if (is_label == false) {
                target = parse_number(dest_op);
            } else {
                if (is_first_pass) {
                    if (mnemonic == "call") {
                        return "e8 00 00 00 00";
                    }
                    else if (mnemonic == "jmp") {
                        return "e9 00 00 00 00";
                    } else {
                        int cc = get_jcc_code(mnemonic);
                        return "0f " + dec_to_hex_pair(0x80 + cc) + " 00 00 00 00";
                    }
                } else {
                    return "NULL";
                }
            }
        }
        if (mnemonic == "call") {
            int64_t offset = target - (4194304 + current_address + 5);
            out_hex = "e8";
            out_hex += " " + dec_to_hex_pair(offset & 0xFF) + " " + dec_to_hex_pair((offset >> 8) & 0xFF) + " " + dec_to_hex_pair((offset >> 16) & 0xFF) + " " + dec_to_hex_pair((offset >> 24) & 0xFF);
            return out_hex;
        }
        else if (mnemonic == "jmp") {
            if (is_label) {
                int64_t offset32 = target - (4194304 + current_address + 5);
                out_hex = "e9";
                out_hex += " " + dec_to_hex_pair(offset32 & 0xFF) + " " + dec_to_hex_pair((offset32 >> 8) & 0xFF) + " " + dec_to_hex_pair((offset32 >> 16) & 0xFF) + " " + dec_to_hex_pair((offset32 >> 24) & 0xFF);
                return out_hex;
            } else {
                int64_t offset8 = target - (4194304 + current_address + 2);
                if (offset8 >= -128 && offset8 <= 127) {
                    out_hex = "eb " + dec_to_hex_pair(offset8 & 0xFF);
                } else {
                    int64_t offset32 = target - (4194304 + current_address + 5);
                    out_hex = "e9";
                    out_hex += " " + dec_to_hex_pair(offset32 & 0xFF) + " " + dec_to_hex_pair((offset32 >> 8) & 0xFF) + " " + dec_to_hex_pair((offset32 >> 16) & 0xFF) + " " + dec_to_hex_pair((offset32 >> 24) & 0xFF);
                }
                return out_hex;
            }
        } else {
            int cc = get_jcc_code(mnemonic);
            if (is_label) {
                int64_t offset32 = target - (4194304 + current_address + 6);
                out_hex = "0f " + dec_to_hex_pair(0x80 + cc);
                out_hex += " " + dec_to_hex_pair(offset32 & 0xFF) + " " + dec_to_hex_pair((offset32 >> 8) & 0xFF) + " " + dec_to_hex_pair((offset32 >> 16) & 0xFF) + " " + dec_to_hex_pair((offset32 >> 24) & 0xFF);
                return out_hex;
            } else {
                int64_t offset8 = target - (4194304 + current_address + 2);
                if (offset8 >= -128 && offset8 <= 127) {
                    out_hex = dec_to_hex_pair(0x70 + cc) + " " + dec_to_hex_pair(offset8 & 0xFF);
                } else {
                    int64_t offset32 = target - (4194304 + current_address + 6);
                    out_hex = "0f " + dec_to_hex_pair(0x80 + cc);
                    out_hex += " " + dec_to_hex_pair(offset32 & 0xFF) + " " + dec_to_hex_pair((offset32 >> 8) & 0xFF) + " " + dec_to_hex_pair((offset32 >> 16) & 0xFF) + " " + dec_to_hex_pair((offset32 >> 24) & 0xFF);
                }
                return out_hex;
            }
        }
    }
    int inst_len = 6;
    std::string dest_info = get_reg_info(dest_op);
    int dest_reg_id = INT(StrSplit(dest_info, "|", 1));
    int dest_reg_sz = INT(StrSplit(dest_info, "|", 2));
    std::string src_info = get_reg_info(src_op);
    int src_reg_id = INT(StrSplit(src_info, "|", 1));
    int src_reg_sz = INT(StrSplit(src_info, "|", 2));
    int d_sz = 0;
    int s_sz = 0;
    if (dest_is_mem == false) {
        d_sz = dest_reg_sz;
    }
    if (src_is_mem == false) {
        s_sz = src_reg_sz;
    }
    int op_size = 64;
    if (d_sz > 0) {
        op_size = d_sz;
    }
    else if (s_sz > 0) {
        op_size = s_sz;
    }
    else if (InStr(dest_op, "qword") || InStr(src_op, "qword")) {
        op_size = 64;
    }
    else if (InStr(dest_op, "dword") || InStr(src_op, "dword")) {
        op_size = 32;
    }
    else if (InStr(dest_op, "word") || InStr(src_op, "word")) {
        op_size = 16;
    }
    else if (InStr(dest_op, "byte") || InStr(src_op, "byte")) {
        op_size = 8;
    }
    if (mnemonic == "jmp" || mnemonic == "call" || mnemonic == "push" || mnemonic == "pop") {
    } else {
        if (op_size == 64) {
            REX += 0x08;
        }
    }
    if (op_size == 16) {
        out_hex = "66 ";
    }
    if (dest_is_mem == false && dest_reg_sz == 8 && dest_reg_id >= 4 && dest_reg_id <= 7 && dest_has_high == false) {
        force_rex = true;
    }
    if (src_is_mem == false && src_is_imm == false && src_reg_sz == 8 && src_reg_id >= 4 && src_reg_id <= 7 && src_has_high == false) {
        force_rex = true;
    }
    bool is_shift_rotate = (mnemonic == "shl" || mnemonic == "shr" || mnemonic == "sar" || mnemonic == "sal" || mnemonic == "rol" || mnemonic == "ror" || mnemonic == "rcl" || mnemonic == "rcr");
    int ext = 0;
    if (is_shift_rotate) {
        if (mnemonic == "rol") {
            ext = 0;
        }
        else if (mnemonic == "ror") {
            ext = 1;
        }
        else if (mnemonic == "rcl") {
            ext = 2;
        }
        else if (mnemonic == "rcr") {
            ext = 3;
        }
        else if (mnemonic == "shl" || mnemonic == "sal") {
            ext = 4;
        }
        else if (mnemonic == "shr") {
            ext = 5;
        }
        else if (mnemonic == "sar") {
            ext = 7;
        }
    }
    if (dest_is_mem || src_is_mem) {
        std::string mem_str = "";
        int reg_id = 0;
        int mem_size = 64;
        if (dest_is_mem) {
            mem_str = dest_op;
            if (InStr(mem_str, "byte")) {
                mem_size = 8;
            }
            else if (InStr(mem_str, "word")) {
                mem_size = 16;
            }
            else if (InStr(mem_str, "dword")) {
                mem_size = 32;
            }
            if (src_is_imm && is_shift_rotate == false) {
                mem_is_imm = true;
                std::string trim_src = Trim(src_op);
                if (RegExMatch(trim_src, "^'.*'$") > 0) {
                    std::string char1 = StrReplace(trim_src, "'", "");
                    imm_val = Asc(Trim(char1));
                } else {
                    imm_val = parse_number(trim_src);
                }
                if (mnemonic == "mov") {
                    opcode = (op_size == 8) ? 0xC6 : 0xC7;
                    reg_id = 0;
                }
                else if (mnemonic == "test") {
                    opcode = (op_size == 8) ? 0xF6 : 0xF7;
                    reg_id = 0;
                } else {
                    if (op_size > 8 && imm_val >= -128 && imm_val <= 127) {
                        opcode = 0x83;
                    } else {
                        opcode = (op_size == 8) ? 0x80 : 0x81;
                    }
                    reg_id = get_mnemonic_info(mnemonic, "imm_ext");
                }
            }
            else if (is_shift_rotate) {
                if (src_op == "1") {
                    opcode = (op_size == 8) ? 0xD0 : 0xD1;
                }
                else if (src_op == "cl") {
                    opcode = (op_size == 8) ? 0xD2 : 0xD3;
                }
                else if (src_is_imm) {
                    opcode = (op_size == 8) ? 0xC0 : 0xC1;
                    mem_is_imm = true;
                    imm_val = parse_number(src_op);
                } else {
                    return "NULL";
                }
                reg_id = ext;
            }
            else if (src_op == "") {
                if (mnemonic == "push" || mnemonic == "pop") {
                    if (op_size == 8 || op_size == 32) {
                        return "NULL";
                    }
                    if (mnemonic == "push") {
                        reg_id = 6;
                        opcode = 0xFF;
                    } else {
                        reg_id = 0;
                        opcode = 0x8F;
                    }
                } else {
                    int ext_single = 0;
                    if (mnemonic == "inc") {
                        ext_single = 0;
                    }
                    else if (mnemonic == "dec") {
                        ext_single = 1;
                    }
                    else if (mnemonic == "call") {
                        ext_single = 2;
                    }
                    else if (mnemonic == "not") {
                        ext_single = 2;
                    }
                    else if (mnemonic == "neg") {
                        ext_single = 3;
                    }
                    else if (mnemonic == "jmp") {
                        ext_single = 4;
                    }
                    else if (mnemonic == "mul") {
                        ext_single = 4;
                    }
                    else if (mnemonic == "imul") {
                        ext_single = 5;
                    }
                    else if (mnemonic == "div") {
                        ext_single = 6;
                    }
                    else if (mnemonic == "idiv") {
                        ext_single = 7;
                    }
                    reg_id = ext_single;
                    bool is_group3 = (mnemonic == "not" || mnemonic == "neg" || mnemonic == "mul" || mnemonic == "imul" || mnemonic == "div" || mnemonic == "idiv");
                    if (mnemonic == "call" || mnemonic == "jmp") {
                        opcode = 0xFF;
                    }
                    else if (is_group3) {
                        opcode = (op_size == 8) ? 0xF6 : 0xF7;
                    } else {
                        opcode = (op_size == 8) ? 0xFE : 0xFF;
                    }
                }
            } else {
                reg_id = src_reg_id;
                if (mnemonic == "mov") {
                    opcode = (op_size == 8) ? 0x88 : 0x89;
                }
                else if (mnemonic == "test") {
                    opcode = (op_size == 8) ? 0x84 : 0x85;
                } else {
                    int base_op = get_mnemonic_info(mnemonic, "base_op");
                    opcode = (op_size == 8) ? base_op : (base_op + 1);
                }
            }
        } else {
            mem_str = src_op;
            if (InStr(mem_str, "byte")) {
                mem_size = 8;
            }
            else if (InStr(mem_str, "word")) {
                mem_size = 16;
            }
            else if (InStr(mem_str, "dword")) {
                mem_size = 32;
            }
            reg_id = dest_reg_id;
            if (mnemonic == "mov") {
                opcode = (op_size == 8) ? 0x8A : 0x8B;
            }
            else if (mnemonic == "movzx") {
                has_0F = true;
                opcode = (mem_size == 8) ? 0xB6 : 0xB7;
            }
            else if (mnemonic == "movsx") {
                has_0F = true;
                opcode = (mem_size == 8) ? 0xBE : 0xBF;
            }
            else if (mnemonic == "movsxd") {
                opcode = 0x63;
            }
            else if (mnemonic == "test") {
                opcode = (op_size == 8) ? 0x84 : 0x85;
            }
            else if (mnemonic == "lea") {
                if (op_size == 8) {
                    return "NULL";
                }
                opcode = 0x8D;
            }
            else if (is_imul_3_op) {
                if (op_size == 8) {
                    return "NULL";
                }
                int64_t imul_imm_temp = parse_number(imul_imm_str);
                bool imm_fits_8 = (imul_imm_temp >= -128 && imul_imm_temp <= 127);
                opcode = imm_fits_8 ? 0x6B : 0x69;
            }
            else if (mnemonic == "imul") {
                if (op_size == 8) {
                    return "NULL";
                }
                has_0F = true;
                opcode = 0xAF;
            } else {
                int base_op = get_mnemonic_info(mnemonic, "base_op");
                opcode = (op_size == 8) ? (base_op + 2) : (base_op + 3);
            }
        }
        std::string inner = StrSplit(mem_str, "[", 2);
        inner = StrReplace(inner, "]", "");
        bool is_pure_disp = false;
        if (RegExMatch(inner, "^-?(0x)?[0-9a-fA-F]+$")) {
            is_pure_disp = true;
            disp = parse_number(inner);
            disp_size = 4;
            inner = "";
        }
        bool has_disp = false;
        std::string val = "";
        std::string op = "";
        if (is_pure_disp == false) {
            std::string p3 = Trim(StrSplit(inner, " + ", 3));
            std::string p2 = Trim(StrSplit(inner, " + ", 2));
            if (p3 != "" && RegExMatch(p3, "^-?(0x)?[0-9a-fA-F]+$")) {
                val = p3;
                op = "+";
                inner = Trim(StrSplit(inner, " + ", 1)) + " + " + Trim(StrSplit(inner, " + ", 2));
                has_disp = true;
            }
            else if (p2 != "" && RegExMatch(p2, "^-?(0x)?[0-9a-fA-F]+$")) {
                val = p2;
                op = "+";
                inner = Trim(StrSplit(inner, " + ", 1));
                has_disp = true;
            }
            if (has_disp == false) {
                std::string m3 = Trim(StrSplit(inner, " - ", 3));
                std::string m2 = Trim(StrSplit(inner, " - ", 2));
                if (m3 != "" && RegExMatch(m3, "^-?(0x)?[0-9a-fA-F]+$")) {
                    val = m3;
                    op = "-";
                    inner = Trim(StrSplit(inner, " - ", 1)) + " - " + Trim(StrSplit(inner, " - ", 2));
                    has_disp = true;
                }
                else if (m2 != "" && RegExMatch(m2, "^-?(0x)?[0-9a-fA-F]+$")) {
                    val = m2;
                    op = "-";
                    inner = Trim(StrSplit(inner, " - ", 1));
                    has_disp = true;
                }
            }
        }
        if (has_disp) {
            disp = parse_number(val);
            if (op == "-") {
                disp = -disp;
            }
            if (disp >= -128 && disp <= 127) {
                disp_size = 1;
            } else {
                disp_size = 4;
            }
        }
        bool b_is_none = false;
        bool is_sib = InStr(inner, "*");
        int imm_size = 4;
        if (is_pure_disp) {
            int64_t orig_disp = disp;
            if (reg_id == 0 && mem_is_imm == false && mnemonic == "mov") {
                if (dest_is_mem) {
                    opcode = (op_size == 8) ? 0xA2 : 0xA3;
                } else {
                    opcode = (op_size == 8) ? 0xA0 : 0xA1;
                }
                if (REX > 0x40 || force_rex) {
                    if (has_high_byte) {
                        return "NULL";
                    }
                    out_hex += dec_to_hex_pair(REX) + " ";
                }
                out_hex += dec_to_hex_pair(opcode);
                for (int A_Index28 = 0; A_Index28 < 8; A_Index28++) {
                    out_hex += " " + dec_to_hex_pair((orig_disp >> ((A_Index28)*8)) & 0xFF);
                }
            } else {
                if (reg_id >= 8) {
                    REX += 0x04;
                }
                if (op_size == 8) {
                    imm_size = 1;
                }
                else if (op_size == 16) {
                    imm_size = 2;
                }
                if (opcode == 0x83) {
                    imm_size = 1;
                }
                if (REX > 0x40 || force_rex) {
                    inst_len += 1;
                }
                if (op_size == 16) {
                    inst_len += 1;
                }
                if (mem_is_imm) {
                    inst_len += imm_size;
                }
                if (orig_disp >= 0) {
                    disp = orig_disp - (4194304 + current_address + inst_len);
                    modrm = 0x05 | ((reg_id & 7) << 3);
                    if (REX > 0x40 || force_rex) {
                        if (has_high_byte) {
                            return "NULL";
                        }
                        out_hex += dec_to_hex_pair(REX) + " ";
                    }
                    if (has_0F) {
                        out_hex += "0f ";
                    }
                    out_hex += dec_to_hex_pair(opcode) + " " + dec_to_hex_pair(modrm);
                } else {
                    modrm = 0x04 | ((reg_id & 7) << 3);
                    sib = 0x25;
                    if (REX > 0x40 || force_rex) {
                        if (has_high_byte) {
                            return "NULL";
                        }
                        out_hex += dec_to_hex_pair(REX) + " ";
                    }
                    if (has_0F) {
                        out_hex += "0f ";
                    }
                    out_hex += dec_to_hex_pair(opcode) + " " + dec_to_hex_pair(modrm) + " " + dec_to_hex_pair(sib);
                }
                out_hex += " " + dec_to_hex_pair(disp & 0xFF) + " " + dec_to_hex_pair((disp >> 8) & 0xFF) + " " + dec_to_hex_pair((disp >> 16) & 0xFF) + " " + dec_to_hex_pair((disp >> 24) & 0xFF);
            }
        }
        else if (is_sib) {
            std::string base_reg = "";
            std::string index_reg = "";
            std::string scale_str = "";
            int b_id = 0;
            int i_id = 0;
            if (InStr(inner, "+")) {
                base_reg = Trim(StrSplit(inner, "+", 1));
                std::string rest = Trim(StrSplit(inner, "+", 2));
                index_reg = Trim(StrSplit(rest, "*", 1));
                scale_str = Trim(StrSplit(rest, "*", 2));
                b_id = get_reg_id(base_reg, registers);
                i_id = get_reg_id(index_reg, registers);
            } else {
                index_reg = Trim(StrSplit(inner, "*", 1));
                scale_str = Trim(StrSplit(inner, "*", 2));
                b_id = 5;
                disp_size = 4;
                b_is_none = true;
                i_id = get_reg_id(index_reg, registers);
            }
            if (scale_str != "1" && scale_str != "2" && scale_str != "4" && scale_str != "8") {
                return "NULL";
            }
            if (reg_id >= 8) {
                REX += 0x04;
            }
            if (i_id >= 8) {
                REX += 0x02;
            }
            if (b_id >= 8 && b_is_none == false) {
                REX += 0x01;
            }
            if (disp_size == 0) {
                if (b_id == 5 || b_id == 13) {
                    if (b_is_none == false) {
                        disp_size = 1;
                        disp = 0LL;
                    }
                }
            }
            uint64_t mod_bits = 0x00;
            if (disp_size == 1) {
                mod_bits = 0x40;
            }
            else if (disp_size == 4) {
                mod_bits = 0x80;
            }
            if (b_is_none) {
                mod_bits = 0x00;
            }
            modrm = mod_bits | ((reg_id & 7) << 3) | 0x04;
            int ss = 0;
            if (scale_str == "2") {
                ss = 1;
            }
            else if (scale_str == "4") {
                ss = 2;
            }
            else if (scale_str == "8") {
                ss = 3;
            }
            sib = (ss << 6) | ((i_id & 7) << 3) | (b_id & 7);
            if (REX > 0x40 || force_rex) {
                if (has_high_byte) {
                    return "NULL";
                }
                out_hex += dec_to_hex_pair(REX) + " ";
            }
            if (has_0F) {
                out_hex += "0f ";
            }
            out_hex += dec_to_hex_pair(opcode) + " " + dec_to_hex_pair(modrm) + " " + dec_to_hex_pair(sib);
            if (disp_size == 1) {
                out_hex += " " + dec_to_hex_pair(disp & 0xFF);
            }
            else if (disp_size == 4) {
                out_hex += " " + dec_to_hex_pair(disp & 0xFF) + " " + dec_to_hex_pair((disp >> 8) & 0xFF) + " " + dec_to_hex_pair((disp >> 16) & 0xFF) + " " + dec_to_hex_pair((disp >> 24) & 0xFF);
            }
        } else {
            bool b_is_label = (inner != "rip" && get_reg_id(inner, registers) == -1);
            bool b_is_rip = (inner == "rip" || b_is_label);
            int b_id = 0;
            if (b_is_rip) {
                b_id = 5;
                disp_size = 4;
                if (b_is_label) {
                    int64_t label_addr = get_label_address(inner);
                    if (label_addr != -1) {
                        int inst_len = 6;
                        if (REX > 0x40 || force_rex) {
                            inst_len += 1;
                        }
                        if (op_size == 16) {
                            inst_len += 1;
                        }
                        if (mem_is_imm) {
                            inst_len += imm_size;
                        }
                        int64_t target_addr = label_addr;
                        target_addr = target_addr + disp;
                        disp = target_addr - (4194304 + current_address + inst_len);
                    } else {
                        int inst_len = 6;
                        if (REX > 0x40 || force_rex) {
                            inst_len += 1;
                        }
                        if (op_size == 16) {
                            inst_len += 1;
                        }
                        if (mem_is_imm) {
                            inst_len += imm_size;
                        }
                        int64_t target_addr = parse_number(inner);
                        target_addr = target_addr + disp;
                        disp = target_addr - (4194304 + current_address + inst_len);
                    }
                }
            } else {
                b_id = get_reg_id(inner, registers);
                if (b_id == -1) {
                    return "NULL";
                }
            }
            if (reg_id >= 8) {
                REX += 0x04;
            }
            if (b_id >= 8 && b_is_rip == false) {
                REX += 0x01;
            }
            if (disp_size == 0) {
                if (b_id == 5 || b_id == 13) {
                    if (b_is_rip == false) {
                        disp_size = 1;
                        disp = 0LL;
                    }
                }
            }
            uint64_t mod_bits = 0x00;
            if (disp_size == 1) {
                mod_bits = 0x40;
            }
            else if (disp_size == 4) {
                mod_bits = 0x80;
            }
            if (b_is_rip) {
                mod_bits = 0x00;
            }
            if (b_id == 4 || b_id == 12 && b_is_rip == false) {
                modrm = mod_bits | ((reg_id & 7) << 3) | 0x04;
                sib = 0x24;
                if (REX > 0x40 || force_rex) {
                    if (has_high_byte) {
                        return "NULL";
                    }
                    out_hex += dec_to_hex_pair(REX) + " ";
                }
                if (has_0F) {
                    out_hex += "0f ";
                }
                out_hex += dec_to_hex_pair(opcode) + " " + dec_to_hex_pair(modrm) + " " + dec_to_hex_pair(sib);
            } else {
                modrm = mod_bits | ((reg_id & 7) << 3) | (b_id & 7);
                if (REX > 0x40 || force_rex) {
                    if (has_high_byte) {
                        return "NULL";
                    }
                    out_hex += dec_to_hex_pair(REX) + " ";
                }
                if (has_0F) {
                    out_hex += "0f ";
                }
                out_hex += dec_to_hex_pair(opcode) + " " + dec_to_hex_pair(modrm);
            }
            if (disp_size == 1) {
                out_hex += " " + dec_to_hex_pair(disp & 0xFF);
            }
            else if (disp_size == 4) {
                out_hex += " " + dec_to_hex_pair(disp & 0xFF) + " " + dec_to_hex_pair((disp >> 8) & 0xFF) + " " + dec_to_hex_pair((disp >> 16) & 0xFF) + " " + dec_to_hex_pair((disp >> 24) & 0xFF);
            }
        }
        if (mem_is_imm) {
            if (op_size == 8 || opcode == 0x83) {
                out_hex += " " + dec_to_hex_pair(imm_val & 0xFF);
            }
            else if (op_size == 16) {
                out_hex += " " + dec_to_hex_pair(imm_val & 0xFF) + " " + dec_to_hex_pair((imm_val >> 8) & 0xFF);
            } else {
                out_hex += " " + dec_to_hex_pair(imm_val & 0xFF) + " " + dec_to_hex_pair((imm_val >> 8) & 0xFF) + " " + dec_to_hex_pair((imm_val >> 16) & 0xFF) + " " + dec_to_hex_pair((imm_val >> 24) & 0xFF);
            }
        }
    }
    else if (src_is_imm && is_shift_rotate == false) {
        int64_t imm = 0LL;
        std::string trim_src = Trim(src_op);
        if (RegExMatch(trim_src, "^'.*'$") > 0) {
            std::string char1 = StrReplace(trim_src, "'", "");
            imm = Asc(Trim(char1));
        } else {
            imm = parse_number(trim_src);
        }
        if (dest_reg_id >= 8) {
            REX += 0x01;
        }
        if (dest_reg_id >= 4 && dest_reg_id <= 7 && op_size == 8 && dest_has_high == false) {
            force_rex = true;
        }
        if (mnemonic != "mov") {
            if (imm < -2147483648 || imm > 2147483647) {
                return "NULL";
            }
        }
        bool use_short_form = false;
        if (op_size < 64) {
            use_short_form = true;
        } else {
            if (imm < -2147483648 || imm > 2147483647) {
                use_short_form = true;
            }
        }
        bool use_83 = false;
        if (mnemonic != "mov" && mnemonic != "test" && mnemonic != "imul" && op_size > 8) {
            if (imm >= -128 && imm <= 127) {
                use_83 = true;
            }
        }
        bool use_rax_shortcut = (dest_reg_id == 0 && mnemonic != "mov" && mnemonic != "imul" && use_83 == false);
        if (use_rax_shortcut) {
            if (mnemonic == "test") {
                opcode = (op_size == 8) ? 0xA8 : 0xA9;
            } else {
                int base_op = get_mnemonic_info(mnemonic, "base_op");
                opcode = (op_size == 8) ? (base_op + 4) : (base_op + 5);
            }
            if (REX > 0x40 || force_rex) {
                if (has_high_byte) {
                    return "NULL";
                }
                out_hex += dec_to_hex_pair(REX) + " ";
            }
            out_hex += dec_to_hex_pair(opcode);
            if (op_size == 8) {
                out_hex += " " + dec_to_hex_pair(imm & 0xFF);
            }
            else if (op_size == 16) {
                out_hex += " " + dec_to_hex_pair(imm & 0xFF) + " " + dec_to_hex_pair((imm >> 8) & 0xFF);
            } else {
                out_hex += " " + dec_to_hex_pair(imm & 0xFF) + " " + dec_to_hex_pair((imm >> 8) & 0xFF) + " " + dec_to_hex_pair((imm >> 16) & 0xFF) + " " + dec_to_hex_pair((imm >> 24) & 0xFF);
            }
        }
        else if (use_83) {
            opcode = 0x83;
            modrm = 0xC0 | (get_mnemonic_info(mnemonic, "imm_ext") << 3) | (dest_reg_id & 7);
            if (REX > 0x40 || force_rex) {
                if (has_high_byte) {
                    return "NULL";
                }
                out_hex += dec_to_hex_pair(REX) + " ";
            }
            out_hex += dec_to_hex_pair(opcode) + " " + dec_to_hex_pair(modrm) + " " + dec_to_hex_pair(imm & 0xFF);
        }
        else if (use_short_form && mnemonic == "mov") {
            if (op_size == 8) {
                opcode = 0xB0 + (dest_reg_id & 7);
                if (REX > 0x40 || force_rex) {
                    if (has_high_byte) {
                        return "NULL";
                    }
                    out_hex += dec_to_hex_pair(REX) + " ";
                }
                out_hex += dec_to_hex_pair(opcode) + " " + dec_to_hex_pair(imm & 0xFF);
            }
            else if (op_size == 16) {
                opcode = 0xB8 + (dest_reg_id & 7);
                if (REX > 0x40 || force_rex) {
                    if (has_high_byte) {
                        return "NULL";
                    }
                    out_hex += dec_to_hex_pair(REX) + " ";
                }
                out_hex += dec_to_hex_pair(opcode) + " " + dec_to_hex_pair(imm & 0xFF) + " " + dec_to_hex_pair((imm >> 8) & 0xFF);
            }
            else if (op_size == 32) {
                opcode = 0xB8 + (dest_reg_id & 7);
                if (REX > 0x40 || force_rex) {
                    if (has_high_byte) {
                        return "NULL";
                    }
                    out_hex += dec_to_hex_pair(REX) + " ";
                }
                out_hex += dec_to_hex_pair(opcode) + " " + dec_to_hex_pair(imm & 0xFF) + " " + dec_to_hex_pair((imm >> 8) & 0xFF) + " " + dec_to_hex_pair((imm >> 16) & 0xFF) + " " + dec_to_hex_pair((imm >> 24) & 0xFF);
            } else {
                opcode = 0xB8 + (dest_reg_id & 7);
                if (REX > 0x40 || force_rex) {
                    if (has_high_byte) {
                        return "NULL";
                    }
                    out_hex += dec_to_hex_pair(REX) + " ";
                }
                out_hex += dec_to_hex_pair(opcode);
                for (int A_Index29 = 0; A_Index29 < 8; A_Index29++) {
                    out_hex += " " + dec_to_hex_pair((imm >> ((A_Index29)*8)) & 0xFF);
                }
            }
        } else {
            if (mnemonic == "mov") {
                opcode = 0xC7;
                if (op_size == 8) {
                    opcode = 0xC6;
                }
                modrm = 0xC0 | (dest_reg_id & 7);
            }
            else if (mnemonic == "test") {
                opcode = (op_size == 8) ? 0xF6 : 0xF7;
                modrm = 0xC0 | (dest_reg_id & 7);
            }
            else if (mnemonic == "imul") {
                bool imm_fits_8 = (imm >= -128 && imm <= 127);
                opcode = imm_fits_8 ? 0x6B : 0x69;
                modrm = 0xC0 | ((dest_reg_id & 7) << 3) | (dest_reg_id & 7);
            } else {
                opcode = (op_size == 8) ? 0x80 : 0x81;
                modrm = 0xC0 | (get_mnemonic_info(mnemonic, "imm_ext") << 3) | (dest_reg_id & 7);
            }
            if (REX > 0x40 || force_rex) {
                if (has_high_byte) {
                    return "NULL";
                }
                out_hex += dec_to_hex_pair(REX) + " ";
            }
            out_hex += dec_to_hex_pair(opcode) + " " + dec_to_hex_pair(modrm);
            if (op_size == 8 || opcode == 0x6B) {
                out_hex += " " + dec_to_hex_pair(imm & 0xFF);
            }
            else if (op_size == 16) {
                out_hex += " " + dec_to_hex_pair(imm & 0xFF) + " " + dec_to_hex_pair((imm >> 8) & 0xFF);
            } else {
                out_hex += " " + dec_to_hex_pair(imm & 0xFF) + " " + dec_to_hex_pair((imm >> 8) & 0xFF) + " " + dec_to_hex_pair((imm >> 16) & 0xFF) + " " + dec_to_hex_pair((imm >> 24) & 0xFF);
            }
        }
    } else {
        if (dest_reg_id == -1) {
            return "NULL";
        }
        if (mnemonic == "push" || mnemonic == "pop") {
            if (op_size == 8 || op_size == 32) {
                return "NULL";
            }
            std::string prefix = "";
            if (op_size == 16) {
                prefix = "66 ";
            }
            if (dest_reg_id >= 8) {
                prefix += "41 ";
            }
            int base = (mnemonic == "push") ? 0x50 : 0x58;
            out_hex = prefix + dec_to_hex_pair(base + (dest_reg_id & 7));
            return out_hex;
        }
        if (is_shift_rotate) {
            if (src_op == "1") {
                opcode = (op_size == 8) ? 0xD0 : 0xD1;
            }
            else if (src_op == "cl") {
                opcode = (op_size == 8) ? 0xD2 : 0xD3;
            }
            else if (src_is_imm) {
                opcode = (op_size == 8) ? 0xC0 : 0xC1;
                imm_val = parse_number(src_op);
            } else {
                return "NULL";
            }
            if (dest_reg_id >= 8) {
                REX += 0x01;
            }
            modrm = 0xC0 | (ext << 3) | (dest_reg_id & 7);
            if (dest_reg_sz == 8 && dest_reg_id >= 4 && dest_reg_id <= 7 && dest_has_high == false) {
                force_rex = true;
            }
            if (REX > 0x40 || force_rex) {
                if (has_high_byte) {
                    return "NULL";
                }
                out_hex += dec_to_hex_pair(REX) + " ";
            }
            out_hex += dec_to_hex_pair(opcode) + " " + dec_to_hex_pair(modrm);
            if (src_is_imm && src_op != "1") {
                out_hex += " " + dec_to_hex_pair(imm_val & 0xFF);
            }
            return out_hex;
        }
        if (src_op == "") {
            int ext_single = 0;
            if (mnemonic == "inc") {
                ext_single = 0;
            }
            else if (mnemonic == "dec") {
                ext_single = 1;
            }
            else if (mnemonic == "call") {
                ext_single = 2;
            }
            else if (mnemonic == "not") {
                ext_single = 2;
            }
            else if (mnemonic == "neg") {
                ext_single = 3;
            }
            else if (mnemonic == "jmp") {
                ext_single = 4;
            }
            else if (mnemonic == "mul") {
                ext_single = 4;
            }
            else if (mnemonic == "imul") {
                ext_single = 5;
            }
            else if (mnemonic == "div") {
                ext_single = 6;
            }
            else if (mnemonic == "idiv") {
                ext_single = 7;
            }
            if (dest_reg_id >= 8) {
                REX += 0x01;
            }
            bool is_group3 = (mnemonic == "not" || mnemonic == "neg" || mnemonic == "mul" || mnemonic == "imul" || mnemonic == "div" || mnemonic == "idiv");
            if (mnemonic == "call" || mnemonic == "jmp") {
                opcode = 0xFF;
            }
            else if (is_group3) {
                opcode = (op_size == 8) ? 0xF6 : 0xF7;
            } else {
                opcode = (op_size == 8) ? 0xFE : 0xFF;
            }
            modrm = 0xC0 | (ext_single << 3) | (dest_reg_id & 7);
            if (dest_reg_sz == 8 && dest_reg_id >= 4 && dest_reg_id <= 7 && dest_has_high == false) {
                force_rex = true;
            }
            if (REX > 0x40 || force_rex) {
                if (has_high_byte) {
                    return "NULL";
                }
                out_hex += dec_to_hex_pair(REX) + " ";
            }
            out_hex += dec_to_hex_pair(opcode) + " " + dec_to_hex_pair(modrm);
            return out_hex;
        }
        bool is_ext_instr = (mnemonic == "movzx" || mnemonic == "movsx" || mnemonic == "movsxd" || mnemonic == "imul");
        if (is_ext_instr) {
            if (dest_reg_id >= 8) {
                REX += 0x04;
            }
            if (src_reg_id >= 8) {
                REX += 0x01;
            }
        } else {
            if (dest_reg_id >= 8) {
                REX += 0x01;
            }
            if (src_reg_id >= 8) {
                REX += 0x04;
            }
        }
        if (op_size == 8) {
            if (dest_reg_id >= 4 && dest_reg_id <= 7 && dest_has_high == false) {
                force_rex = true;
            }
            if (src_reg_id >= 4 && src_reg_id <= 7 && src_has_high == false) {
                force_rex = true;
            }
        }
        if (mnemonic == "mov") {
            opcode = (op_size == 8) ? 0x88 : 0x89;
        }
        else if (mnemonic == "movzx") {
            has_0F = true;
            opcode = (src_reg_sz == 8) ? 0xB6 : 0xB7;
        }
        else if (mnemonic == "movsx") {
            has_0F = true;
            opcode = (src_reg_sz == 8) ? 0xBE : 0xBF;
        }
        else if (mnemonic == "movsxd") {
            opcode = 0x63;
        }
        else if (mnemonic == "test") {
            opcode = (op_size == 8) ? 0x84 : 0x85;
        }
        else if (is_imul_3_op) {
            if (op_size == 8) {
                return "NULL";
            }
            int64_t imul_imm_temp = parse_number(imul_imm_str);
            bool imm_fits_8 = (imul_imm_temp >= -128 && imul_imm_temp <= 127);
            opcode = imm_fits_8 ? 0x6B : 0x69;
        }
        else if (mnemonic == "imul") {
            if (op_size == 8) {
                return "NULL";
            }
            has_0F = true;
            opcode = 0xAF;
        } else {
            int base_op = get_mnemonic_info(mnemonic, "base_op");
            opcode = (op_size == 8) ? (base_op + 2) : (base_op + 3);
        }
        is_ext_instr = (mnemonic == "movzx" || mnemonic == "movsx" || mnemonic == "movsxd" || mnemonic == "imul");
        bool uses_reg_rm_style = (is_ext_instr || (mnemonic != "mov"));
        if (uses_reg_rm_style) {
            modrm = 0xC0 | ((dest_reg_id & 7) << 3) | (src_reg_id & 7);
        } else {
            modrm = 0xC0 | ((src_reg_id & 7) << 3) | (dest_reg_id & 7);
        }
        if (REX > 0x40 || force_rex) {
            if (has_high_byte) {
                return "NULL";
            }
            out_hex += dec_to_hex_pair(REX) + " ";
        }
        if (has_0F) {
            out_hex += "0f ";
        }
        out_hex += dec_to_hex_pair(opcode) + " " + dec_to_hex_pair(modrm);
    }
    if (is_imul_3_op) {
        int64_t imul_imm = parse_number(imul_imm_str);
        bool imm_fits_8 = (imul_imm >= -128 && imul_imm <= 127);
        if (imm_fits_8) {
            out_hex += " " + dec_to_hex_pair(imul_imm & 0xFF);
        } else {
            if (op_size == 16) {
                out_hex += " " + dec_to_hex_pair(imul_imm & 0xFF) + " " + dec_to_hex_pair((imul_imm >> 8) & 0xFF);
            } else {
                out_hex += " " + dec_to_hex_pair(imul_imm & 0xFF) + " " + dec_to_hex_pair((imul_imm >> 8) & 0xFF) + " " + dec_to_hex_pair((imul_imm >> 16) & 0xFF) + " " + dec_to_hex_pair((imul_imm >> 24) & 0xFF);
            }
        }
    }
    return out_hex;
}
std::string str0 = "";
std::string str1 = "";
std::string str2 = "";
std::string str3 = "";
std::string str4 = "";
std::string str5 = "";
std::string str6 = "";
std::string str7 = "";
std::string str8 = "";
std::string str9 = "";
std::string str10 = "";
std::string str11 = "";
std::string str12 = "";
std::string str13 = "";
std::string str14 = "";
std::string str15 = "";
std::string str16 = "";
std::string str17 = "";
std::string str18 = "";
std::string str19 = "";
std::string str20 = "";
// tundra GLOBALS
// tundra GLOBALS
std::vector<std::string> tokens;
std::vector<std::string> locations;
std::vector<std::string> file_name;
std::vector<std::string> strings;
int strings_count = 0;
std::string keyWordEscape = Chr(96);
std::vector<std::string> prog_blocks;
int prog_blocks_count = 0;
std::vector<std::string> arradds;
int arradds_count = 0;
// tundra GLOBALS
// tundra GLOBALS
//;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;
// need for handle_include
std::string main_file_name = "";
int doseHaveInclude_int = 0;
// need for handle_include
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// TUNDRA PARSER AND TOKENIZER
// TUNDRA PARSER AND TOKENIZER
// TUNDRA PARSER AND TOKENIZER
std::string remove_comments(std::string code, std::string sl_mark, std::string ml_start, std::string ml_end, std::string escape_char) {
    std::string new_code = "";
    int code_len = 0;
    int i_pos = 0;
    std::string c = "";
    int skip = 0;
    bool in_str = false;
    bool in_slc = false;
    bool in_mlc = false;
    bool is_escaped = false;
    int sl_len = 0;
    int mls_len = 0;
    int mle_len = 0;
    std::string p_char = "";
    bool can_start_comment = false;
    code_len = StrLen(code);
    sl_len = StrLen(sl_mark);
    mls_len = StrLen(ml_start);
    mle_len = StrLen(ml_end);
    for (int A_Index30 = 0; A_Index30 < code_len; A_Index30++) {
        if (skip > 0) {
            skip--;
            continue;
        }
        i_pos = A_Index30 + 1;
        c = SubStr(code, i_pos, 1);
        if (c == Chr(10)) {
            in_slc = false;
            is_escaped = false;
            new_code += c;
            continue;
        }
        if (in_slc == true) {
            continue;
        }
        if (in_mlc == true) {
            if (mle_len > 0) {
                if (SubStr(code, i_pos, mle_len) == ml_end) {
                    in_mlc = false;
                    skip = mle_len - 1;
                }
            }
            continue;
        }
        if (is_escaped == true) {
            is_escaped = false;
            new_code += c;
            continue;
        }
        if (in_str == true) {
            if (c == escape_char) {
                is_escaped = true;
            }
            else if (c == Chr(34)) {
                in_str = false;
            }
            new_code += c;
            continue;
        }
        if (c == Chr(34)) {
            in_str = true;
            new_code += c;
            continue;
        }
        // --- THE FIX: Only start SL comment if preceded by whitespace or Start-of-Line ---
        if (sl_len > 0) {
            if (SubStr(code, i_pos, sl_len) == sl_mark) {
                can_start_comment = false;
                if (i_pos == 1) {
                    can_start_comment = true;
                } else {
                    p_char = SubStr(code, i_pos - 1, 1);
                    if (p_char == " " || p_char == Chr(9) || p_char == Chr(10) || p_char == Chr(13)) {
                        can_start_comment = true;
                    }
                }
                if (can_start_comment == true) {
                    in_slc = true;
                    skip = sl_len - 1;
                    continue;
                }
            }
        }
        if (mls_len > 0) {
            if (SubStr(code, i_pos, mls_len) == ml_start) {
                in_mlc = true;
                skip = mls_len - 1;
                continue;
            }
        }
        new_code += c;
    }
    return new_code;
}
std::string restoreStrings_tundra(std::string code) {
    for (int A_Index31 = 0; A_Index31 < strings_count; A_Index31++) {
        code = StrReplace(code, "vsiauydvgbausvbausodyfhidoufcgsiyouysfivgidfvasdkubfcasAA" + STR(A_Index31 + 1) + "AA", strings[A_Index31]);
    }
    return code;
}
bool tokenize_first_help(std::string c1, std::string c2) {
    if (RegExMatch(c1, "^\\w$") && RegExMatch(c2, "^\\w$")) {
        return false;
    }
    return true;
}
void tokenize_first(std::string code) {
    std::vector<std::string> future;
    std::string hold_temp = "";
    std::string hold_temp_strings = "";
    int inChr34 = 0;
    int wasChr34 = 0;
    int countNewLines = 0;
    int countChars_num_on_same_line = 1;
    bool is_escaped = false;
    std::vector<std::string> items32 = LoopParseFunc(code);
    for (size_t A_Index32 = 0; A_Index32 < items32.size(); A_Index32++) {
        std::string A_LoopField32 = items32[A_Index32 - 0];
        HTVM_Append(future, A_LoopField32);
    }
    HTVM_Append(future, " ");
    std::vector<std::string> items33 = LoopParseFunc(code);
    for (size_t A_Index33 = 0; A_Index33 < items33.size(); A_Index33++) {
        std::string A_LoopField33 = items33[A_Index33 - 0];
        if (A_LoopField33 == Chr(10)) {
            countNewLines++;
            countChars_num_on_same_line = 0;
        } else {
            countChars_num_on_same_line++;
        }
        hold_temp += A_LoopField33;
        if (tokenize_first_help(A_LoopField33, future[A_Index33 + 1])) {
            if (is_escaped == true) {
                is_escaped = false;
            }
            else if (A_LoopField33 == keyWordEscape) {
                is_escaped = true;
            }
            else if (A_LoopField33 == Chr(34)) {
                inChr34++;
                wasChr34 = 1;
            }
            if (inChr34 == 2) {
                hold_temp_strings += hold_temp;
                inChr34 = 0;
                wasChr34 = 0;
                strings_count++;
                HTVM_Append(strings, hold_temp_strings);
                HTVM_Append(tokens, "vsiauydvgbausvbausodyfhidoufcgsiyouysfivgidfvasdkubfcasAA" + STR(strings_count) + "AA");
                hold_temp_strings = "";
                HTVM_Append(locations, STR(countNewLines + 1) + ":" + STR(countChars_num_on_same_line));
            } else {
                if (wasChr34 == 0) {
                    if (Trim(hold_temp) != "") {
                        HTVM_Append(tokens, hold_temp);
                        HTVM_Append(locations, STR(countNewLines + 1) + ":" + STR(countChars_num_on_same_line));
                    }
                } else {
                    hold_temp_strings += hold_temp;
                }
            }
            hold_temp = "";
        }
    }
}
void tokenize_add_semicolons() {
    std::vector<std::string> new_tokens;
    std::vector<std::string> new_locations;
    int paren_depth = 0;
    int bracket_depth = 0;
    bool should_insert_semicolon = false;
    std::string current_token = "";
    std::string next_token = "";
    std::string current_location = "";
    std::string next_location = "";
    std::string current_line_num = "";
    std::string next_line_num = "";
    bool is_on_new_line = false;
    int loop_iterations = 0;
    int original_tokens_initial_size = 0;
    int new_tokens_final_size = 0;
    int next_idx = 0;
    int nt_size = 0;
    int last_idx = 0;
    bool is_paren_brace_combo = false;
    loop_iterations = HTVM_Size(tokens);
    for (int A_Index34 = 0; A_Index34 < loop_iterations; A_Index34++) {
        current_token = tokens[A_Index34];
        current_location = locations[A_Index34];
        next_idx = A_Index34 + 1;
        if (next_idx < loop_iterations) {
            next_token = tokens[next_idx];
            next_location = locations[next_idx];
        } else {
            next_token = "SENTINEL_END_OF_INPUT";
            next_location = current_location;
        }
        HTVM_Append(new_tokens, current_token);
        HTVM_Append(new_locations, current_location);
        if (current_token == "(") {
            paren_depth++;
        }
        else if (current_token == ")") {
            paren_depth--;
        }
        else if (current_token == "[") {
            bracket_depth++;
        }
        else if (current_token == "]") {
            bracket_depth--;
        }
        should_insert_semicolon = false;
        is_on_new_line = false;
        is_paren_brace_combo = false;
        if (next_token != "SENTINEL_END_OF_INPUT") {
            current_line_num = StrSplit(current_location, ":", 1);
            next_line_num = StrSplit(next_location, ":", 1);
            if (current_line_num != next_line_num) {
                is_on_new_line = true;
            }
        }
        if (current_token == ")" && next_token == "{") {
            is_paren_brace_combo = true;
        }
        if (paren_depth == 0 && bracket_depth == 0) {
            if (current_token != ";" && current_token != "{" && current_token != "}") {
                if (is_on_new_line == true) {
                    if (current_token != "else" && is_paren_brace_combo == false && next_token != "{" && next_token != "else" && next_token != "SENTINEL_END_OF_INPUT") {
                        should_insert_semicolon = true;
                    }
                }
            }
            if (next_token == "}" && is_on_new_line == true && current_token != "{" && current_token != ";") {
                should_insert_semicolon = true;
            }
        }
        if (should_insert_semicolon == true) {
            nt_size = HTVM_Size(new_tokens);
            if (nt_size > 0) {
                last_idx = nt_size - 1;
                if (new_tokens[last_idx] == ";") {
                    should_insert_semicolon = false;
                }
            }
        }
        if (should_insert_semicolon == true) {
            HTVM_Append(new_tokens, ";");
            HTVM_Append(new_locations, current_location);
        }
    }
    original_tokens_initial_size = HTVM_Size(tokens);
    for (int A_Index35 = 0; A_Index35 < original_tokens_initial_size; A_Index35++) {
        HTVM_Pop(tokens);
        HTVM_Pop(locations);
    }
    new_tokens_final_size = HTVM_Size(new_tokens);
    for (int A_Index36 = 0; A_Index36 < new_tokens_final_size; A_Index36++) {
        HTVM_Append(tokens, new_tokens[A_Index36]);
        HTVM_Append(locations, new_locations[A_Index36]);
    }
    HTVM_Append(tokens, " ");
    HTVM_Append(tokens, " ");
}
std::string getLastWordFromToken(std::string token) {
    std::string out = "";
    if (Trim(token) == "") {
        return Trim(token);
    }
    std::vector<std::string> items37 = LoopParseFunc(token, " ");
    for (size_t A_Index37 = 0; A_Index37 < items37.size(); A_Index37++) {
        std::string A_LoopField37 = items37[A_Index37 - 0];
        out = Trim(A_LoopField37);
    }
    return out;
}
std::string mask_arradd(std::string code) {
    std::string new_code = "";
    int code_len = 0;
    int i = 1;
    std::string c = "";
    int skip = 0;
    int start_pos = 0;
    std::string block_content = "";
    std::string rest_of_code = "";
    int found_in_rest = 0;
    bool in_str = false;
    bool is_escaped = false;
    bool can_start = false;
    std::string p_char = "";
    code_len = StrLen(code);
    for (int A_Index38 = 0; A_Index38 < code_len; A_Index38++) {
        if (skip > 0) {
            skip--;
            continue;
        }
        i = A_Index38 + 1;
        c = SubStr(code, i, 1);
        // --- String Protection ---
        if (is_escaped == true) {
            is_escaped = false;
            new_code += c;
            continue;
        }
        if (in_str == true) {
            if (c == keyWordEscape) {
                is_escaped = true;
            }
            else if (c == Chr(34)) {
                in_str = false;
            }
            new_code += c;
            continue;
        }
        if (c == Chr(34)) {
            in_str = true;
            new_code += c;
            continue;
        }
        // --- Detect arradd ---
        if (StrLower(c) == "a") {
            // Check if it is "arradd" followed by a space or a tab
            if (StrLower(SubStr(code, i, 7)) == "arradd " || StrLower(SubStr(code, i, 7)) == "arradd" + Chr(9)) {
                // Only trigger if it is at the start of a line or separated by whitespace
                can_start = false;
                if (i == 1) {
                    can_start = true;
                } else {
                    p_char = SubStr(code, i - 1, 1);
                    if (p_char == " " || p_char == Chr(9) || p_char == Chr(10) || p_char == Chr(13)) {
                        can_start = true;
                    }
                }
                if (can_start == true) {
                    start_pos = i;
                    // Look ahead for the line break
                    rest_of_code = SubStr(code, start_pos);
                    found_in_rest = InStr(rest_of_code, Chr(10));
                    if (found_in_rest > 0) {
                        // Grab everything up to the newline, but DO NOT include the Chr(10)
                        block_content = SubStr(code, start_pos, found_in_rest - 1);
                    } else {
                        // If there is no newline, it goes to the end of the file
                        block_content = rest_of_code;
                    }
                    // Strip trailing carriage return Chr(13) if Windows-style line endings \r\n
                    if (SubStr(block_content, StrLen(block_content), 1) == Chr(13)) {
                        block_content = SubStr(block_content, 1, StrLen(block_content) - 1);
                    }
                    arradds_count++;
                    HTVM_Append(arradds, block_content);
                    // Swap out with a unique token that passes the tokenizers "^\w+$" regex
                    new_code += "vgdskjbhfgkjbfsgkjbdfsgjksdfgARRADD" + STR(arradds_count) + "AA";
                    skip = StrLen(block_content) - 1;
                    continue;
                }
            }
        }
        new_code += c;
    }
    return new_code;
}
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// main Tundra()
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
std::string Tundra_for_asm(std::string code) {
    tokens = {};
    locations = {};
    strings = {};
    strings_count = 0;
    keyWordEscape = Chr(96);
    prog_blocks = {};
    prog_blocks_count = 0;
    arradds = {};
    arradds_count = 0;
    std::string token = "";
    std::string code_original = "";
    std::string out = "";
    int skip = 0;
    code_original = code;
    code = remove_comments(code, ";", "'''1", "'''2", keyWordEscape);
    code = mask_arradd(code);
    tokenize_first(code);
    tokenize_add_semicolons();
    //Loop, tokens.size() - 2 {
    //    print("element" . STR(A_Index + 1) . ": " . tokens[A_Index] . "      Location: " . locations[A_Index])
    //}
    //print("===============================")
    //Loop, strings_count {
    //print(strings[A_Index])
    //}
    std::vector<std::string> object_names;
    int max_tokens = HTVM_Size(tokens);
    std::string t_low = "";
    int next1 = 0;
    int next2 = 0;
    int next3 = 0;
    std::string as_kw = "";
    for (int A_Index39 = 0; A_Index39 < max_tokens; A_Index39++) {
        t_low = StrLower(tokens[A_Index39]);
        if (t_low == "alliance" || t_low == "crew" || t_low == "struct") {
            next1 = A_Index39 + 1;
            if (next1 < max_tokens) {
                HTVM_Append(object_names, tokens[next1]);
            }
        }
        else if (t_low == "instance") {
            next2 = A_Index39 + 2;
            next3 = A_Index39 + 3;
            if (next3 < max_tokens) {
                as_kw = StrLower(tokens[next2]);
                if (as_kw == "as") {
                    HTVM_Append(object_names, tokens[next3]);
                }
            }
        }
    }
    HTVM_Append(tokens, " ");
    HTVM_Append(tokens, " ");
    token = "";
    std::string next_token = "";
    std::string next_location = "";
    std::string current_location = "";
    int next_idx = 0;
    std::string next2_token = "";
    int next2_idx = 0;
    std::string next3_token = "";
    int next3_idx = 0;
    std::string prev_token = "";
    bool add_space = true;
    bool c_is_word = false;
    bool n_is_word = false;
    bool curr_is_obj = false;
    bool prev_is_obj = false;
    std::string l1_line = "";
    int l1_col = 0;
    std::string l2_line = "";
    int l2_col = 0;
    int expected_col = 0;
    for (int A_Index40 = 0; A_Index40 < HTVM_Size(tokens) - 4; A_Index40++) {
        if (skip > 0) {
            skip--;
            continue;
        }
        token = tokens[A_Index40];
        current_location = locations[A_Index40];
        next_token = "";
        next2_token = "";
        next3_token = "";
        prev_token = "";
        add_space = true;
        c_is_word = false;
        n_is_word = false;
        curr_is_obj = false;
        prev_is_obj = false;
        next_location = "";
        next_idx = A_Index40 + 1;
        if (next_idx < HTVM_Size(tokens)) {
            next_token = tokens[next_idx];
        }
        // PROTECT LOCATIONS ARRAY FROM OUT-OF-BOUNDS CRASH
        if (next_idx < HTVM_Size(locations)) {
            next_location = locations[next_idx];
        } else {
            next_location = "";
        }
        next2_idx = A_Index40 + 2;
        if (next2_idx < HTVM_Size(tokens)) {
            next2_token = tokens[next2_idx];
        }
        next3_idx = A_Index40 + 3;
        if (next3_idx < HTVM_Size(tokens)) {
            next3_token = tokens[next3_idx];
        }
        if (A_Index40 > 1) {
            prev_token = tokens[A_Index40 - 1];
        }
        for (int A_Index41 = 0; A_Index41 < HTVM_Size(object_names); A_Index41++) {
            if (token == object_names[A_Index41]) {
                curr_is_obj = true;
            }
            if (prev_token == object_names[A_Index41]) {
                prev_is_obj = true;
            }
        }
        if (token == ";") {
            out += Chr(10);
        }
        else if (token == "{") {
            out += Chr(10) + "{" + Chr(10);
        }
        else if (token == "}") {
            out += Chr(10) + "}" + Chr(10);
        } else {
            if (RegExMatch(token, "^\\w+$") > 0) {
                c_is_word = true;
            }
            if (RegExMatch(next_token, "^\\w+$") > 0) {
                n_is_word = true;
            }
            if (next_token == "(" || next_token == ")" || next_token == "]") {
                add_space = false;
            }
            else if (next_token == "[") {
                if (c_is_word == true) {
                    add_space = false;
                } else {
                    add_space = true;
                }
            }
            else if (token == "(" || token == "[") {
                add_space = false;
            }
            else if (next_token == ",") {
                add_space = false;
            }
            else if (c_is_word == false && n_is_word == false) {
                add_space = false;
            }
            if (next_token == "=") {
                add_space = true;
            }
            if (token == "=") {
                add_space = true;
            }
            if (token == keyWordEscape) {
                add_space = false;
            }
            if (token == "!" || next_token == ":") {
                add_space = false;
            }
            if (token == "." || next_token == ".") {
                add_space = false;
            }
            if (next_token == "-" || next_token == "[") {
                add_space = true;
            }
            if (token == "-") {
                l1_line = StrSplit(current_location, ":", 1);
                l1_col = INT(StrSplit(current_location, ":", 2));
                l2_line = StrSplit(next_location, ":", 1);
                l2_col = INT(StrSplit(next_location, ":", 2));
                if (l1_line == l2_line) {
                    expected_col = l1_col + StrLen(next_token);
                    if (expected_col == l2_col) {
                        add_space = false;
                    } else {
                        add_space = true;
                    }
                } else {
                    add_space = true;
                }
            }
            if (token == "&" || token == "|") {
                if (next_token == "(") {
                    add_space = true;
                }
            }
            if (token == "$") {
                add_space = false;
            }
            if (token == ",") {
                add_space = true;
            }
            if (token == ")" || token == "]") {
                add_space = true;
                if (next_token == ")" || next_token == "]" || next_token == "," || next_token == ";" || next_token == "." || next_token == ":") {
                    add_space = false;
                }
            }
            if (add_space == true) {
                out += token + " ";
            } else {
                out += token;
            }
        }
    }
    std::string out2 = "";
    std::vector<std::string> items42 = LoopParseFunc(out, "\n", "\r");
    for (size_t A_Index42 = 0; A_Index42 < items42.size(); A_Index42++) {
        std::string A_LoopField42 = items42[A_Index42 - 0];
        std::string line = A_LoopField42;
        std::string trimmed = Trim(line);
        if (SubStr(trimmed, 1, 4) == "jmp.") {
            line = StrReplace(line, "jmp.", "jmp .");
        }
        else if (SubStr(trimmed, 1, 3) == "je.") {
            line = StrReplace(line, "je.", "je .");
        }
        else if (SubStr(trimmed, 1, 4) == "jne.") {
            line = StrReplace(line, "jne.", "jne .");
        }
        else if (SubStr(trimmed, 1, 3) == "jl.") {
            line = StrReplace(line, "jl.", "jl .");
        }
        else if (SubStr(trimmed, 1, 4) == "jle.") {
            line = StrReplace(line, "jle.", "jle .");
        }
        else if (SubStr(trimmed, 1, 3) == "jg.") {
            line = StrReplace(line, "jg.", "jg .");
        }
        else if (SubStr(trimmed, 1, 4) == "jge.") {
            line = StrReplace(line, "jge.", "jge .");
        }
        else if (SubStr(trimmed, 1, 3) == "ja.") {
            line = StrReplace(line, "ja.", "ja .");
        }
        else if (SubStr(trimmed, 1, 4) == "jae.") {
            line = StrReplace(line, "jae.", "jae .");
        }
        else if (SubStr(trimmed, 1, 3) == "jb.") {
            line = StrReplace(line, "jb.", "jb .");
        }
        else if (SubStr(trimmed, 1, 4) == "jbe.") {
            line = StrReplace(line, "jbe.", "jbe .");
        }
        else if (SubStr(trimmed, 1, 3) == "jc.") {
            line = StrReplace(line, "jc.", "jc .");
        }
        else if (SubStr(trimmed, 1, 4) == "jnc.") {
            line = StrReplace(line, "jnc.", "jnc .");
        }
        else if (SubStr(trimmed, 1, 3) == "jo.") {
            line = StrReplace(line, "jo.", "jo .");
        }
        else if (SubStr(trimmed, 1, 4) == "jno.") {
            line = StrReplace(line, "jno.", "jno .");
        }
        else if (SubStr(trimmed, 1, 3) == "js.") {
            line = StrReplace(line, "js.", "js .");
        }
        else if (SubStr(trimmed, 1, 4) == "jns.") {
            line = StrReplace(line, "jns.", "jns .");
        }
        else if (SubStr(trimmed, 1, 4) == "jpe.") {
            line = StrReplace(line, "jpe.", "jpe .");
        }
        else if (SubStr(trimmed, 1, 4) == "jpo.") {
            line = StrReplace(line, "jpo.", "jpo .");
        }
        else if (SubStr(trimmed, 1, 3) == "jz.") {
            line = StrReplace(line, "jz.", "jz .");
        }
        else if (SubStr(trimmed, 1, 4) == "jnz.") {
            line = StrReplace(line, "jnz.", "jnz .");
        }
        out2 += line + Chr(10);
    }
    out = StringTrimRight(out2, 1);
    out = StrReplace(out, "''", "32");
    //out := indent_nested_curly_braces(out)
    out = restoreStrings_tundra(out);
    //print("================== OUT ==================")
    //print("================== OUT ==================")
    //print("================== OUT ==================")
    //print(out)
    return out;
}
// TUNDRA PARSER AND TOKENIZER
// TUNDRA PARSER AND TOKENIZER
// TUNDRA PARSER AND TOKENIZER
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
std::string error_line = "";
int assembler(std::string code, std::string file_name) {
    std::vector<std::string> allData;
    std::vector<std::string> header;
    std::string start1 = "00";
    std::string start2 = "00";
    std::string start3 = "00";
    std::string start4 = "00";
    code = Tundra_for_asm(code);
    //print("========== in assembler ==========")
    HT_LIB_theIdNumOfThe34 = 0;
    HT_Lib_theIdNumOfThe34theVar = {};
    // The ultimate 2-segment, 174-byte header template
    // PHDR 1: Maps the ENTIRE file (Data + Code) at 0x400000
    // PHDR 2: Maps the BSS variables at 0x10000000
    header = {"7f", "45", "4c", "46", "02", "01", "01", "03", "00", "00", "00", "00", "00", "00", "00", "00", "02", "00", "3e", "00", "01", "00", "00", "00", start1, start2, start3, start4, "00", "00", "00", "00", "40", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "40", "00", "38", "00", "02", "00", "40", "00", "00", "00", "00", "00", "01", "00", "00", "00", "07", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "40", "00", "00", "00", "00", "00", "00", "00", "40", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "10", "00", "00", "00", "00", "00", "00", "01", "00", "00", "00", "06", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "10", "00", "00", "00", "00", "00", "00", "00", "10", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "10", "00", "00", "00", "00"};
    std::string temp_encode_out = "";
    std::string save_start_temp = "";
    std::vector<std::string> str_names;
    std::vector<int> str_size;
    std::vector<std::string> str_size_name;
    std::vector<std::string> str_name_size_num;
    std::vector<std::string> str_names_arrd;
    uint64_t mock_address_str = 0ULL;
    int str_names_size = 0;
    std::vector<std::string> equ_names;
    std::vector<std::string> equ_values;
    // Your exact original BSS logic
    uint64_t current_bss_addr = 268435456ULL;
    int total_bss_size = 0;
    std::string keyWordEscpaeChar = Chr(92);
    code = preserveStrings(code);
    std::string code_out = "";
    std::vector<std::string> items43 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index43 = 0; A_Index43 < items43.size(); A_Index43++) {
        std::string A_LoopField43 = items43[A_Index43 - 0];
        std::string trimmed_line = Trim(A_LoopField43);
        if (InStr(trimmed_line, "format ") == 1 || InStr(trimmed_line, "entry ") == 1 || InStr(trimmed_line, "segment ") == 1) {
            continue;
        }
        if (InStr(A_LoopField43, " db ")) {
            str_names_size = 0;
            str1 = Trim(StrSplit(A_LoopField43, " db ", 1));
            str2 = Trim(StrSplit(A_LoopField43, " db ", 2));
            str3 = Trim(StrSplit(str2, ",", 1));
            str4 = Trim(StrSplit(str2, ",", 2));
            for (int A_Index44 = 0; A_Index44 < HT_LIB_theIdNumOfThe34; A_Index44++) {
                if (HT_LIB_theIdNumOfThe34 == A_Index44 + 1) {
                    str3 = StrReplace(str3, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(A_Index44 + 1) + Chr(65) + Chr(65), StrReplace(HT_Lib_theIdNumOfThe34theVar[A_Index44 + 1], keyWordEscpaeChar, "\\") + Chr(34));
                } else {
                    str3 = StrReplace(str3, "VYIGUOYIYVIUCFCYIUCFCYIGCYGICFHYFHCTCFTFDFGYGFC" + Chr(65) + Chr(65) + STR(A_Index44 + 1) + Chr(65) + Chr(65), StrReplace(HT_Lib_theIdNumOfThe34theVar[A_Index44 + 1], keyWordEscpaeChar, "\\"));
                }
            }
            bool is_string = false;
            if (SubStr(str3, 1, 1) == Chr(34) || SubStr(str3, 1, 1) == "'") {
                is_string = true;
                str3 = StringTrimLeft(str3, 1);
                str3 = StringTrimRight(str3, 1);
            }
            mock_address_str = HTVM_Size(header);
            save_start_temp = address_to_hexstr(4194304 + mock_address_str);
            HTVM_Append(str_names, str1);
            HTVM_Append(str_names_arrd, save_start_temp);
            if (Trim(str4) == "") {
                if (str3 != "") {
                    if (is_string == false) {
                        HTVM_Append(header, dec_to_hex_pair(parse_number(str3)));
                        str_names_size++;
                    } else {
                        std::vector<std::string> items45 = LoopParseFunc(str3);
                        for (size_t A_Index45 = 0; A_Index45 < items45.size(); A_Index45++) {
                            std::string A_LoopField45 = items45[A_Index45 - 0];
                            HTVM_Append(header, char_to_hex(A_LoopField45));
                            str_names_size++;
                        }
                    }
                }
            } else {
                if (str3 == "") {
                    HTVM_Append(header, char_to_hex(Chr(parse_number(str4))));
                    str_names_size++;
                } else {
                    if (is_string == false) {
                        HTVM_Append(header, dec_to_hex_pair(parse_number(str3)));
                        str_names_size++;
                    } else {
                        std::vector<std::string> items46 = LoopParseFunc(str3);
                        for (size_t A_Index46 = 0; A_Index46 < items46.size(); A_Index46++) {
                            std::string A_LoopField46 = items46[A_Index46 - 0];
                            HTVM_Append(header, char_to_hex(A_LoopField46));
                            str_names_size++;
                        }
                    }
                    HTVM_Append(header, char_to_hex(Chr(parse_number(str4))));
                    str_names_size++;
                }
            }
            HTVM_Append(str_size, str_names_size);
        }
        else if (InStr(A_LoopField43, " dq ")) {
            str1 = Trim(StrSplit(A_LoopField43, " dq ", 1));
            str2 = Trim(StrSplit(A_LoopField43, " dq ", 2));
            mock_address_str = HTVM_Size(header);
            save_start_temp = address_to_hexstr(4194304 + mock_address_str);
            HTVM_Append(str_names, str1);
            HTVM_Append(str_names_arrd, save_start_temp);
            std::string dq_hex = size_to_hex64(parse_number(str2));
            std::vector<std::string> items47 = LoopParseFunc(dq_hex, " ");
            for (size_t A_Index47 = 0; A_Index47 < items47.size(); A_Index47++) {
                std::string A_LoopField47 = items47[A_Index47 - 0];
                HTVM_Append(header, A_LoopField47);
            }
            HTVM_Append(str_size, 8);
        }
        else if (InStr(A_LoopField43, " rb ")) {
            str1 = Trim(StrSplit(A_LoopField43, " rb ", 1));
            str2 = Trim(StrSplit(A_LoopField43, " rb ", 2));
            HTVM_Append(str_names, str1);
            HTVM_Append(str_names_arrd, address_to_hexstr(current_bss_addr));
            // FIXED: Keep the str_size array synchronized
            HTVM_Append(str_size, INT(str2));
            current_bss_addr = current_bss_addr + INT(str2);
            total_bss_size = total_bss_size + INT(str2);
        }
        else if (InStr(A_LoopField43, " rq ")) {
            str1 = Trim(StrSplit(A_LoopField43, " rq ", 1));
            str2 = Trim(StrSplit(A_LoopField43, " rq ", 2));
            HTVM_Append(str_names, str1);
            HTVM_Append(str_names_arrd, address_to_hexstr(current_bss_addr));
            int reserved_bytes = INT(str2) * 8;
            // FIXED: Keep the str_size array synchronized
            HTVM_Append(str_size, reserved_bytes);
            current_bss_addr = current_bss_addr + reserved_bytes;
            total_bss_size = total_bss_size + reserved_bytes;
        }
        else if (InStr(A_LoopField43, "=") && InStr(A_LoopField43, "$-") == 0) {
            str1 = Trim(StrSplit(A_LoopField43, "=", 1));
            str2 = Trim(StrSplit(A_LoopField43, "=", 2));
            HTVM_Append(equ_names, str1);
            HTVM_Append(equ_values, str2);
        }
        else if (InStr(A_LoopField43, " = $-") || InStr(A_LoopField43, "= $-") || InStr(A_LoopField43, " =$-") || InStr(A_LoopField43, "=$-")) {
            if (InStr(A_LoopField43, " = $-")) {
                str1 = Trim(StrSplit(A_LoopField43, " = $-", 1));
                str2 = Trim(StrSplit(A_LoopField43, " = $-", 2));
            }
            else if (InStr(A_LoopField43, "= $-")) {
                str1 = Trim(StrSplit(A_LoopField43, "= $-", 1));
                str2 = Trim(StrSplit(A_LoopField43, "= $-", 2));
            }
            else if (InStr(A_LoopField43, " =$-")) {
                str1 = Trim(StrSplit(A_LoopField43, " =$-", 1));
                str2 = Trim(StrSplit(A_LoopField43, " =$-", 2));
            }
            else if (InStr(A_LoopField43, "=$-")) {
                str1 = Trim(StrSplit(A_LoopField43, "=$-", 1));
                str2 = Trim(StrSplit(A_LoopField43, "=$-", 2));
            }
            HTVM_Append(str_size_name, str1);
            for (int A_Index48 = 0; A_Index48 < HTVM_Size(str_names); A_Index48++) {
                if (str2 == str_names[A_Index48]) {
                    HTVM_Append(str_name_size_num, STR(str_size[A_Index48]));
                    break;
                }
            }
        } else {
            code_out += A_LoopField43 + Chr(10);
        }
    }
    code = Trim(code_out);
    for (int A_Index49 = 0; A_Index49 < HTVM_Size(str_names); A_Index49++) {
        code = RegExReplace(code, "\\b" + str_names[A_Index49] + "\\b", str_names_arrd[A_Index49]);
    }
    for (int A_Index50 = 0; A_Index50 < HTVM_Size(str_size_name); A_Index50++) {
        code = RegExReplace(code, "\\b" + str_size_name[A_Index50] + "\\b", str_name_size_num[A_Index50]);
    }
    for (int A_Index51 = 0; A_Index51 < HTVM_Size(equ_names); A_Index51++) {
        std::string safe_name = StrReplace(equ_names[A_Index51], ".", "\\.");
        code = RegExReplace(code, "\\b" + safe_name + "\\b", equ_values[A_Index51]);
    }
    set_first_pass(true);
    uint64_t mock_address = HTVM_Size(header);
    std::vector<std::string> items52 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index52 = 0; A_Index52 < items52.size(); A_Index52++) {
        std::string A_LoopField52 = items52[A_Index52 - 0];
        std::string line = Trim(A_LoopField52);
        if (line == "") {
            continue;
        }
        if (RegExMatch(line, "^\\s*([a-zA-Z._][a-zA-Z0-9$_.]*):")) {
            std::string label_name = StrReplace(line, ":", "");
            if (line == "_start:") {
                save_start_temp = size_to_hex64(4194304 + mock_address);
                std::vector<std::string> items53 = LoopParseFunc(save_start_temp, " ");
                for (size_t A_Index53 = 0; A_Index53 < items53.size(); A_Index53++) {
                    std::string A_LoopField53 = items53[A_Index53 - 0];
                    if (A_Index53 == 0) {
                        header[24] = A_LoopField53;
                    }
                    else if (A_Index53 == 1) {
                        header[25] = A_LoopField53;
                    }
                    else if (A_Index53 == 2) {
                        header[26] = A_LoopField53;
                    }
                    else if (A_Index53 == 3) {
                        header[27] = A_LoopField53;
                    }
                }
            } else {
                add_label(label_name, 4194304 + mock_address);
            }
        } else {
            set_current_address(mock_address);
            temp_encode_out = encode(line);
            int bc = get_byte_count(temp_encode_out);
            mock_address = mock_address + bc;
        }
    }
    set_first_pass(false);
    std::vector<std::string> items54 = LoopParseFunc(code, "\n", "\r");
    for (size_t A_Index54 = 0; A_Index54 < items54.size(); A_Index54++) {
        std::string A_LoopField54 = items54[A_Index54 - 0];
        std::string line = Trim(A_LoopField54);
        if (line == "") {
            continue;
        }
        if (RegExMatch(line, "^\\s*([a-zA-Z._][a-zA-Z0-9$_.]*):")) {
            continue;
        } else {
            set_current_address(HTVM_Size(header));
            temp_encode_out = encode(line);
            if (temp_encode_out == "NULL") {
                error_line = line;
                return 1;
            }
            std::vector<std::string> items55 = LoopParseFunc(temp_encode_out, " ");
            for (size_t A_Index55 = 0; A_Index55 < items55.size(); A_Index55++) {
                std::string A_LoopField55 = items55[A_Index55 - 0];
                if (Trim(A_LoopField55) != "") {
                    HTVM_Append(header, Trim(A_LoopField55));
                }
            }
        }
    }
    // --- THE FINAL HEADER SIZE WRITES ---
    // Segment 1 (File Data + Code)
    std::vector<std::string> file_size_hex = size_to_hex32(HTVM_Size(header));
    header[96] = file_size_hex[0];
    header[97] = file_size_hex[1];
    header[98] = file_size_hex[2];
    header[99] = file_size_hex[3];
    header[104] = file_size_hex[0];
    header[105] = file_size_hex[1];
    header[106] = file_size_hex[2];
    header[107] = file_size_hex[3];
    // Segment 2 (BSS)
    if (total_bss_size > 0) {
        std::vector<std::string> bss_size_hex = size_to_hex32(total_bss_size);
        header[160] = bss_size_hex[0];
        header[161] = bss_size_hex[1];
        header[162] = bss_size_hex[2];
        header[163] = bss_size_hex[3];
    }
    final_assemble(file_name, header);
    print("HTLL-Assembler: Success");
    return 0;
}
//;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;
void main_assembler(std::string code, std::string paramsTemp_fileName) {
    // Reset those global variables.
    HT_LIB_theIdNumOfThe34 = 0;
    HT_Lib_theIdNumOfThe34theVar = {};
    int assembler_out = 0;
    assembler_out = assembler(code, paramsTemp_fileName);
    if (assembler_out == 0) {
        print("Compilation finished: " + paramsTemp_fileName + " generated.");
    } else {
        print("Assembling failed. Something went wrong.");
        print("This is line that caused it:");
        print(error_line);
    }
}
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
int main(int argc, char* argv[]) {
    std::string params = Trim(GetParams());
    std::string paramsTemp = "";
    std::string paramsTemp_fileName = "";
    int enable_custom_assembler_x86_64_linux = 1;
    if (params == "") {
        print("Usage:" + Chr(10) + "./HT-Assembler your_file.s" + Chr(10));
    } else {
        std::vector<std::string> items56 = LoopParseFunc(params, "\n", "\r");
        for (size_t A_Index56 = 0; A_Index56 < items56.size(); A_Index56++) {
            std::string A_LoopField56 = items56[A_Index56 - 0];
            paramsTemp = Trim(A_LoopField56);
            if (A_Index56 == 0) {
                paramsTemp_fileName = Trim(StringTrimRight(paramsTemp, 2));
                main_assembler(FileRead(Trim(paramsTemp)), paramsTemp_fileName);
                break;
            }
        }
    }
    

    return 0;
}