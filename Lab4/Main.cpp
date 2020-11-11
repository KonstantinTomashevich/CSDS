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
#define MIN_ORDER_FOR_OK_GROUP 11

    uint64_t exchangeSuccesses = 0;
    uint64_t exchangeFails = 0;
    uint64_t exchangeMathFails = 0;

    uint64_t signatureSuccesses = 0;
    uint64_t signatureFails = 0;
    uint64_t signatureNotCalculatedFails = 0;
    uint64_t signatureMathFails = 0;

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
            ++exchangeMathFails;
            continue;
        }

        std::cout << "G: (" << g.value ().x_ << ", " << g.value ().y_ << ")." << std::endl;
        std::cout << "Order: " << order << "." << std::endl;

        if (order < MIN_ORDER_FOR_OK_GROUP)
        {
            std::cout << "Skipping, because order is too low!" << std::endl;
            --index;
            continue;
        }

        EM::EllipticGroup::PrivateKey alicePrivate = group.GeneratePrivateKey (order);
        EM::EllipticGroup::PrivateKey bobPrivate = group.GeneratePrivateKey (order);

        std::cout << "Alice private: " << alicePrivate.multiplier_ << "." << std::endl;
        std::cout << "Bob private: " << bobPrivate.multiplier_ << "." << std::endl;

        try
        {
            EM::EllipticGroup::PublicKey alicePublic = group.AssemblePublicKey (alicePrivate, g.value ());
            std::cout << "Alice public: (" << alicePublic.value_.x_ << ", " <<
                      alicePublic.value_.y_ << ")." << std::endl;

            EM::EllipticGroup::PublicKey bobPublic = group.AssemblePublicKey (bobPrivate, g.value ());
            std::cout << "Bob public: (" << bobPublic.value_.x_ << ", " <<
                      bobPublic.value_.y_ << ")." << std::endl;

            EM::EllipticGroup::CommonKey aliceCommon = group.AssembleCommonKey (alicePrivate, bobPublic);
            std::cout << "Alice view of common key: (" << aliceCommon.value_.x_ << ", " <<
                      aliceCommon.value_.y_ << ")." << std::endl;

            EM::EllipticGroup::CommonKey bobCommon = group.AssembleCommonKey (bobPrivate, alicePublic);
            std::cout << "Bob view of common key: (" << bobCommon.value_.x_ << ", " <<
                      bobCommon.value_.y_ << ")." << std::endl;

            if (aliceCommon.value_ == bobCommon.value_)
            {
                std::cout << "Exchange successful!" << std::endl;
                ++exchangeSuccesses;
            }
            else
            {
                std::cout << "Exchange failed!" << std::endl;
                ++exchangeFails;
            }

            uint64_t messageHash = MathUtils::Random::Get ().Next ();
            std::optional <EM::EllipticGroup::Signature> signature =
                group.Sign (messageHash, alicePrivate, g.value (), order);

            if (!signature.has_value ())
            {
                std::cout << "Unable to calculate signature!" << std::endl;
                ++signatureNotCalculatedFails;
                continue;
            }

            switch (group.Check (signature.value (), messageHash, alicePublic, g.value (), order))
            {
                case EM::EllipticGroup::SignatureCheckResult::SUCCESSFUL:
                    std::cout << "Signature checked successfully!" << std::endl;
                    ++signatureSuccesses;
                    break;

                case EM::EllipticGroup::SignatureCheckResult::MATH_ERROR:
                    std::cout << "Signature check failed due to math error!" << std::endl;
                    ++signatureMathFails;
                    break;

                case EM::EllipticGroup::SignatureCheckResult::FAILED:
                    std::cout << "Signature check failed!" << std::endl;
                    ++signatureFails;
                    break;
            }
        }
        catch (std::bad_optional_access &exception)
        {
            std::cout << "Caught infinity during key assembling!" << std::endl;
            ++exchangeMathFails;
        }
    }

    std::cout << std::endl;
    std::cout << "Exchange successes: " << exchangeSuccesses << std::endl;
    std::cout << "Exchange fails: " << exchangeFails << std::endl;
    std::cout << "Exchange math fails: " << exchangeMathFails << std::endl;

    std::cout << std::endl;
    std::cout << "Signature successes: " << signatureSuccesses << std::endl;
    std::cout << "Signature fails: " << signatureFails << std::endl;
    std::cout << "Signature not calculated fails: " << signatureNotCalculatedFails << std::endl;
    std::cout << "Signature math fails: " << signatureMathFails << std::endl;
    return 0;
}
