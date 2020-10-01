#pragma once
#include <cstdint>
#include <functional>

namespace Idea
{
typedef std::array <uint8_t, 8> Block;

typedef std::array <uint8_t, 16> Key;

void EncodeBlock (Block &block, const Key &key);
void DecodeBlock (Block &block, const Key &key);

template <typename ProducerFunctor, typename ConsumerFunctor>
void EncodeCBC (const Block &initial, const Key &key, ProducerFunctor &producer, ConsumerFunctor &consumer)
{
    Block state = initial;
    Block working;

    while (producer (working))
    {
        for (uint32_t index = 0; index < working.size (); ++index)
        {
            working[index] ^= state[index];
        }

        EncodeBlock (working, key);
        state = working;
        consumer (working);
    }
}

template <typename ProducerFunctor, typename ConsumerFunctor>
void DecodeCBC (const Block &initial,  const Key &key, ProducerFunctor &producer, ConsumerFunctor &consumer)
{
    Block state = initial;
    Block working;

    while (producer (working))
    {
        Block previousState = state;
        state = working;

        DecodeBlock (working, key);
        for (uint32_t index = 0; index < working.size (); ++index)
        {
            working[index] ^= previousState[index];
        }

        consumer (working);
    }
}
}
