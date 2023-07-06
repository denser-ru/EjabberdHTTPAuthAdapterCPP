#pragma once

#include <iostream>

class StreamRedirector {
public:
	~StreamRedirector( void );
	explicit StreamRedirector( std::ostream& stream, std::streambuf* newBuffer );

	void	disable( void );
	void	enable( void );

private:
	std::ostream&		stream;
	std::streambuf*	 	oldBuffer;
	std::streambuf*		newBuffer;
	bool				isRedirected_;
};
