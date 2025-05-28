#include "ReadFile.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::string ReadFile(const std::string& path) {
    std::string out;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(path);

        std::stringstream shaderStream;
        shaderStream << file.rdbuf();

        file.close();
        out = shaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR: Could not successfully read file with path: " << path << "And error: " << e.what() << std::endl;
    }

    return out;
}