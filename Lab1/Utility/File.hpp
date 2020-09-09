#pragma once
#include <cstdio>
#include <memory>

class IterableInputFile final
{
public:
    class ByteIterator final
    {
    public:
        ByteIterator (const ByteIterator &other);
        ~ByteIterator ();
        std::size_t GetPosition ();

        char operator* () const;
        void operator++ ();
        ByteIterator &operator+= (int offset);

        bool operator== (const ByteIterator &rhs) const;
        bool operator!= (const ByteIterator &rhs) const;

        friend class IterableInputFile;

    private:
        ByteIterator (const char *path, bool binary);
        ByteIterator ();

        std::shared_ptr <FILE> file_;
        int lastReadResult_;
    };

    IterableInputFile (const char *path, bool binary);
    ~IterableInputFile () = default;

    ByteIterator begin ();
    ByteIterator end ();

private:
    const char *path_;
    bool binary_;
};
