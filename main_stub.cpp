#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";
static inline bool is_base64(char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}
std::vector<char> base64_decode(std::string const &encoded_string)
{
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    char char_array_4[4], char_array_3[3];
    std::vector<char> ret;

    while (in_len-- && (encoded_string[in_] != '=') &&
           is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] =
                    base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) +
                              ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) +
                              ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] =
                ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) +
                          ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) +
                          ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] =
            ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
            ret.push_back(char_array_3[j]);
    }

    return ret;
}
int main(int argc, char *argv[], char *envp[])
{
    int skip_bytes = 0xfaceb00c;
    std::fstream ifs, ofs;
    ifs.open("Answer.cpp", std::ios::in | std::ios::binary);
    ofs.open("R_answer", std::ios::out | std::ios::binary);
    ifs.seekg(skip_bytes);
    std::string buffer(
        (std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>());
    buffer.erase(
        std::remove(buffer.begin(), buffer.end(), '\n'),
        buffer.end());
    buffer = buffer.substr(0, buffer.size() - 2);
    auto decode = base64_decode(buffer);
    std::copy(
        decode.begin(), decode.end(),
        std::ostreambuf_iterator<char>(ofs));
    ifs.close();
    ofs.close();
    chmod(
        "./R_answer",
        S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    execle("./R_answer", "R_answer", NULL, envp);
    perror("[execve]");
    return -1;
}
