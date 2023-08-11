#pragma once
#include "irc.hpp"

class	ErrException: public std::exception
{
	private:
		std::string	*_message;

	public:
		ErrException(const char * message) {
			this->_message = ft_strdup(message);
		}
		virtual	~ErrException() throw() {
			delete this->_message;
		}
		virtual const char * what() const throw() {
			return (this->_message->c_str());
		}
};
