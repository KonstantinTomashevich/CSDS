#include "File.hpp"

IterableInputFile::ByteIterator::~ByteIterator ()
{
    if (file_ != nullptr)
    {
        fclose (file_);
    }
}

char IterableInputFile::ByteIterator::operator* () const
{
    return lastReadResult_ == EOF ? 0 : (char) lastReadResult_;
}

void IterableInputFile::ByteIterator::operator++ ()
{
    if (file_ != nullptr)
    {
        lastReadResult_ = fgetc (file_);
    }

    if (lastReadResult_ == EOF)
    {
        file_ = nullptr;
    }
}

bool IterableInputFile::ByteIterator::operator== (const IterableInputFile::ByteIterator &rhs) const
{
    return file_ == rhs.file_;
}

bool IterableInputFile::ByteIterator::operator!= (const IterableInputFile::ByteIterator &rhs) const
{
    return !(rhs == *this);
}

IterableInputFile::ByteIterator::ByteIterator (const char *path, bool binary)
    : file_ (fopen (path, binary ? "rb" : "r"))
{
    operator++ ();
}

IterableInputFile::ByteIterator::ByteIterator ()
    : file_ (nullptr)
{
}

IterableInputFile::IterableInputFile (const char *path, bool binary)
    : path_ (path),
      binary_ (binary)
{
}

IterableInputFile::ByteIterator IterableInputFile::begin ()
{
    return IterableInputFile::ByteIterator (path_, binary_);
}

IterableInputFile::ByteIterator IterableInputFile::end ()
{
    return IterableInputFile::ByteIterator ();
}
