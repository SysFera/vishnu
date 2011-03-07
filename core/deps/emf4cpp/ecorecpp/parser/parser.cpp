// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * parser/parser.cpp
 * Copyright (C) Cátedra SAES-UMU 2010 <andres.senac@um.es>
 *
 * EMF4CPP is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EMF4CPP is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "parser.hpp"
#include "handler.hpp"
#include "../util/debug.hpp"
#include <iostream>
#include <fstream>
#include "../mapping.hpp"
#include <string.h>


#ifdef DEBUG
#include <sys/time.h>
#include <unistd.h>
#endif

using namespace ::ecorecpp::parser;

parser::parser()
{
}

parser::~parser()
{
}

::ecore::EObject_ptr parser::load(const std::string myString)
{
    //
    //  Create our SAX handler object and install it on the parser, as the
    //  document and error handler.
    //
    handler _handler;

    xml_parser::SemanticState< handler > ss (_handler);

    ::ecorecpp::mapping::type_traits::char_t* buffer;
    int length = myString.length();
    /*{
        // Read file
        std::ifstream is (_file);
        // get length of file:
        is.seekg (0, std::ios::end);
        length = is.tellg();
        is.seekg (0, std::ios::beg);
        // allocate memory:
        buffer = new ::ecorecpp::mapping::type_traits::char_t [length];
        // read data as a block:
        is.read (buffer,length);

        is.close();
    }*/
    buffer = strdup(myString.c_str());
    State< xml_parser::SemanticState< handler > >
        st(ss, buffer, length);

    //
    //  Get the starting time and kick off the parse of the indicated
    //  file. Catch any exceptions that might propogate out of it.
    //
#ifdef DEBUG
    struct timeval start, end;
    long mtime, seconds, useconds;
#endif

#ifdef DEBUG
    gettimeofday(&start, NULL);
#endif
    xml_parser::grammar::the_xml::match(st);
#ifdef DEBUG
    gettimeofday(&end, NULL);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = (long)((seconds) * 1000 + useconds/1000.0) + 0.5;
#endif
    DEBUG_MSG(cout,"--- DURATION: " << mtime);

    //
    //  Delete the buffer
    //
    delete[] buffer;

    _handler.resolveReferences();

    ::ecore::EObject_ptr _r = _handler.getRootElement();
    _r->_initialize();
    return _r;
}
