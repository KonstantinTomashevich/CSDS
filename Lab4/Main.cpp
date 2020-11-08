#include <iostream>
#include <cmath>

#include "EM.hpp"

#define ERROR_INCORRECT_ARGS 1
#define ERROR_UNABLE_TO_GENERATE_GROUP 2

int main (int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Expected call format:" << std::endl << "    <executable path> <M>";
        return ERROR_INCORRECT_ARGS;
    }

    uint64_t M;
    {
        int parsedM = std::stoi (std::string (argv[1]));
        if (parsedM < 2 || parsedM > 100)
        {
            std::cout << "M must be > 1 and < 100." << std::endl;
            return ERROR_INCORRECT_ARGS;
        }

        for (uint64_t divCheck = 2; divCheck <= sqrt (parsedM); ++divCheck)
        {
            if (parsedM % divCheck == 0)
            {
                std::cout << "M must be prime, but it is divisible by " << divCheck << "!" << std::endl;
                return ERROR_INCORRECT_ARGS;
            }
        }

        M = parsedM;
    }

    uint64_t a;
    uint64_t b;
    if (!EM::BruteForceGenerate (M, a, b))
    {
        std::cout << "Unable to generate EM for M = " << M << "." << std::endl;
        return ERROR_UNABLE_TO_GENERATE_GROUP;
    }

    std::cout << "Generated EM(" << a << " " << b << ") for M = " << M << "." << std::endl;
    EM::EllipticGroup group {{M, a, b}};
    std::cout << "EM values:" << std::endl;

    for (EM::EllipticGroup::Value value : group)
    {
        std::cout << "    " << value.x_ << " " << value.y_ << std::endl;
    }

    return 0;
}
