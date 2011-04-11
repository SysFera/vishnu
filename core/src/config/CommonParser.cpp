
#include "CommonParser.hpp"

#ifdef DEBUG
std::ostream debug(std::cerr.rdbuf());
#else
std::ostream debug(0);
#endif

std::string nullString("");
ConfigMap *configPtr = new ConfigMap;

bool StartsWith::operator() (const std::string& s2) const
{
    return (0 == s2.compare(0, s1.size(), s1)) ? true : false;
}


Splitter::Splitter()
    : delim_('='), hasNext_(false),
      previous_(0), current_(0)
{
}


Splitter::Splitter(const char c)
    : delim_(c), hasNext_(false),
      previous_(0), current_(0)
{
}


Splitter::Splitter(const std::string& str, const char c)
    : delim_(c), previous_(0), current_(0), str_(str)
{
    hasNext_ = (str_.empty() ? false : true);
}


void Splitter::reset(const std::string& str)
{
    reset(str, delim_);
}


void Splitter::reset(const std::string& str, const char c)
{
    if (str != str_) {
    	str_ = str;
    }

    hasNext_ = (str_.empty() ? false : true);
    delim_ = c;
    previous_ = 0;
    current_ = 0;
}


std::string& Splitter::operator() ()
{
    if (!hasNext_) {
	token_.clear();
	return token_;
    }

    std::string::size_type current_ = str_.find(delim_, previous_);

    std::string sub = str_.substr(previous_, current_);
    token_.swap(sub);
    previous_ = (std::string::npos == current_) ?
	(str_.length() + 1) : (current_ + 1);

    // can't generate tokens anymore :'(
    if (str_.length() < previous_) {
	hasNext_ = false;
    }

    return token_;
}
