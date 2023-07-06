#include "StreamRedirector.h"

StreamRedirector::StreamRedirector(std::ostream& stream, std::streambuf* newBuffer)
	:
        stream( stream ),
        oldBuffer( stream.rdbuf() ),
        newBuffer( newBuffer ),
        isRedirected_( true )
{
	stream.rdbuf( newBuffer );
}

StreamRedirector::~StreamRedirector( void )
{
	if ( isRedirected_ ) {
		stream.rdbuf( oldBuffer );
	}
}

void StreamRedirector::disable( void )
{
	if ( isRedirected_ ) {
		stream.rdbuf( oldBuffer );
		isRedirected_ = false;
	}
}

void StreamRedirector::enable( void )
{
	if ( !isRedirected_ ) {
		stream.rdbuf( newBuffer );
		isRedirected_ = true;
	}
}