#include "StreamRedirector.h"

StreamRedirector::StreamRedirector(std::ostream& stream, std::streambuf* newBuffer)
    : stream(stream), oldBuffer(stream.rdbuf()), newBuffer(newBuffer), isRedirected(true)
{
    stream.rdbuf(newBuffer);
}

StreamRedirector::~StreamRedirector( void )
{
    if (isRedirected) {
        stream.rdbuf(oldBuffer);
    }
}

void StreamRedirector::disable( void )
{
    if (isRedirected) {
        stream.rdbuf(oldBuffer);
        isRedirected = false;
    }
}

void StreamRedirector::enable( void )
{
    if (!isRedirected) {
        stream.rdbuf(newBuffer);
        isRedirected = true;
    }
}