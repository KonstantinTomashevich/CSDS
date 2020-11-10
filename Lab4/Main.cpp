#include <iostream>
#include <exception>

#include "EM.hpp"
#include "MathUtils.hpp"

#define ERROR_INCORRECT_ARGS 1
#define ERROR_UNABLE_TO_PICK_G 2

int main (int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Expected call format:" << std::endl << "    <executable path> <M>";
        return ERROR_INCORRECT_ARGS;
    }

    int64_t M;
    {
        int parsedM = std::stoi (std::string (argv[1]));
        if (parsedM < 2)
        {
            std::cout << "M must be > 1." << std::endl;
            return ERROR_INCORRECT_ARGS;
        }

        if (!MathUtils::BruteForceIsPrime (parsedM))
        {
            std::cout << "M must be prime!" << std::endl;
            return ERROR_INCORRECT_ARGS;
        }

        M = parsedM;
    }

#define TEST_COUNT 1000
    uint64_t successes = 0;
    uint64_t fails = 0;

    for (uint64_t index = 0; index < TEST_COUNT; ++index)
    {
        std::cout << std::endl << "Test #" << index << std::endl;
        int64_t a;
        int64_t b;
        EM::RandomGenerate (M, a, b);

        std::cout << "Generated EM(" << a << ", " << b << ") for M = " << M << "." << std::endl;
        EM::EllipticGroup group {{M, a, b}};
        std::cout << "EM values:" << std::endl;

        for (EM::EllipticGroup::Value value : group)
        {
            std::cout << "    (" << value.x_ << ", " << value.y_ << ")" << std::endl;
        }

        uint64_t order;
        std::optional <EM::EllipticGroup::Value> g = group.PickRandomG (order);

        if (!g.has_value ())
        {
            std::cout << "Unable to pick G!" << std::endl;
            continue;
        }

        std::cout << "G: (" << g.value ().x_ << ", " << g.value ().y_ << ")." << std::endl;
        std::cout << "Order:" << order << "." << std::endl;

        EM::EllipticGroup::PrivateKey alicePrivate = group.GeneratePrivateKey (order);
        EM::EllipticGroup::PrivateKey bobPrivate = group.GeneratePrivateKey (order);

        try
        {
            EM::EllipticGroup::PublicKey alicePublic = group.AssemblePublicKey (alicePrivate, g.value ());
            EM::EllipticGroup::PublicKey bobPublic = group.AssemblePublicKey (bobPrivate, g.value ());

            EM::EllipticGroup::CommonKey aliceCommon = group.AssembleCommonKey (alicePrivate, bobPublic);
            EM::EllipticGroup::CommonKey bobCommon = group.AssembleCommonKey (bobPrivate, alicePublic);

            std::cout << "Alice private: " << alicePrivate.multiplier_ << "." << std::endl;
            std::cout << "Alice public: (" << alicePublic.value_.x_ << ", " <<
                      alicePublic.value_.y_ << ")." << std::endl;

            std::cout << "Alice view of common key: (" << aliceCommon.value_.x_ << ", " <<
                      aliceCommon.value_.y_ << ")." << std::endl;

            std::cout << "Bob private: " << bobPrivate.multiplier_ << "." << std::endl;
            std::cout << "Bob public: (" << bobPublic.value_.x_ << ", " <<
                      bobPublic.value_.y_ << ")." << std::endl;

            std::cout << "Bob view of common key: (" << bobCommon.value_.x_ << ", " <<
                      bobCommon.value_.y_ << ")." << std::endl;

            if (aliceCommon.value_ == bobCommon.value_)
            {
                ++successes;
            }
            else
            {
                ++fails;
            }
        }
        catch (std::bad_optional_access exception)
        {
            std::cout << "Caught infinity during key assembling!" << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << "Successes: " << successes << std::endl;
    std::cout << "Fails: " << fails << std::endl;
    return 0;
}
