#include <boost/test/unit_test.hpp>
#include <ctime>
#include <Shared/Idea.hpp>

BOOST_AUTO_TEST_SUITE (TestIdea)

BOOST_AUTO_TEST_CASE (BlockEncodeDecode)
{
    Idea::Block testBlock = {13, 14, 77, 99, 0, 23, 44, 11};
    Idea::Block testBlockCopy = testBlock;

    Idea::Key key = {14, 66, 99, 42, 48, 99, 164, 244, 122, 90, 49, 11, 23, 41, 54, 67};
    Idea::EncodeBlock (testBlock, key);
    BOOST_REQUIRE (testBlock != testBlockCopy);

    Idea::DecodeBlock (testBlock, key);
    BOOST_REQUIRE (testBlock == testBlockCopy);
}

BOOST_AUTO_TEST_CASE (ChainedEncodeDecode)
{
#define BLOCK_COUNT 8
    std::array <Idea::Block, 8> input {};
    std::array <Idea::Block, BLOCK_COUNT> encodedOutput {};
    std::array <Idea::Block, BLOCK_COUNT> decodedOutput {};

    for (uint32_t blockIndex = 0; blockIndex < input.size (); ++blockIndex)
    {
        for (uint32_t elementIndex = 0; blockIndex < input[blockIndex].size (); ++blockIndex)
        {
            input[blockIndex][elementIndex] = (uint8_t) rand ();
        }
    }

    Idea::Block initial = {13, 14, 77, 99, 0, 23, 44, 11};
    Idea::Key key = {14, 66, 99, 42, 48, 99, 164, 244, 122, 90, 49, 11, 23, 41, 54, 67};

    struct Producer
    {
        Producer (std::array <Idea::Block, 8> &input)
            : input (input),
              iterator (input.begin ())
        {
        }

        bool operator() (Idea::Block &output)
        {
            if (iterator == input.end ())
            {
                return false;
            }
            else
            {
                output = *iterator;
                ++iterator;
                return true;
            }
        }

        std::array <Idea::Block, BLOCK_COUNT> &input;
        std::array <Idea::Block, BLOCK_COUNT>::iterator iterator;
    };

    struct Consumer
    {
        Consumer (std::array <Idea::Block, 8> &output)
            : output (output),
              iterator (output.begin ())
        {
        }

        void operator() (const Idea::Block &input)
        {
            if (iterator != output.end ())
            {
                *iterator = input;
                ++iterator;
            }
        }

        std::array <Idea::Block, BLOCK_COUNT> &output;
        std::array <Idea::Block, BLOCK_COUNT>::iterator iterator;
    };

    Producer inputProducer {input};
    Consumer encodedOutputConsumer {encodedOutput};
    Idea::EncodeCBC (initial, key, inputProducer, encodedOutputConsumer);
    BOOST_REQUIRE (input != encodedOutput);

    Producer encodedOutputProducer {encodedOutput};
    Consumer decodedOutputConsumer {decodedOutput};
    Idea::DecodeCBC (initial, key, encodedOutputProducer, decodedOutputConsumer);
    BOOST_REQUIRE (input == decodedOutput);
}

BOOST_AUTO_TEST_SUITE_END ()
