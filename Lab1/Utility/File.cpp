#include "File.hpp"

struct FileDeleter
{
    void operator() (FILE *file)
    {
        fclose (file);
    }
};

IterableInputFile::ByteIterator::ByteIterator (const IterableInputFile::ByteIterator &other)
    : file_ (other.file_), lastReadResult_ (other.lastReadResult_)
{
}

IterableInputFile::ByteIterator::~ByteIterator ()
{
}

std::size_t IterableInputFile::ByteIterator::GetPosition ()
{
    if (file_ == nullptr)
    {
        return -1;
    }
    else
    {
        return ftell (file_.get ());
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
        lastReadResult_ = fgetc (file_.get ());
    }

    if (lastReadResult_ == EOF)
    {
        file_ = nullptr;
    }
}

IterableInputFile::ByteIterator &IterableInputFile::ByteIterator::operator+= (int offset)
{
    if (file_ != nullptr && fseek (file_.get (), offset, SEEK_CUR) == 0)
    {
        operator++ ();
    }
    else
    {
        file_ = nullptr;
    }

    return *this;
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
    : file_ (fopen (path, binary ? "rb" : "r"), FileDeleter ())
{
    operator++ ();
}

IterableInputFile::ByteIterator::ByteIterator ()
    : file_ (nullptr, FileDeleter ())
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
