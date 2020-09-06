#pragma once
#include <cstdio>

class IterableInputFile final
{
public:
    class ByteIterator final
    {
    public:
        ~ByteIterator ();

        char operator *() const;
        void operator++();

        bool operator== (const ByteIterator &rhs) const;
        bool operator!= (const ByteIterator &rhs) const;

        friend class IterableInputFile;

    private:
        ByteIterator (const char *path, bool binary);
        ByteIterator ();

        FILE *file_;
        int lastReadResult_;
    };

    IterableInputFile(const char *path, bool binary);
    ~IterableInputFile () = default;

    ByteIterator begin ();
    ByteIterator end ();

private:
    const char *path_;
    bool binary_;
};
