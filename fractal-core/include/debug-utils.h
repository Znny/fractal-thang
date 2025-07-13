#include <iostream>
#include <glm/glm.hpp>

void printMatrix(const glm::mat4& matrix) {
    std::cout << "Matrix:" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}