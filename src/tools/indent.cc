// Copyright (C) 2010 by Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include <cassert>
#include <iomanip>
#include <ostream>

#include "llvc/tools/indent.hh"

namespace trackingClient
{
  inline long int& indent (std::ostream& o)
  {
    // The slot to store the current indentation level.
    static const long int indent_index = std::ios::xalloc ();
    return o.iword (indent_index);
  }

  std::ostream& incindent (std::ostream& o)
  {
    indent (o) += 2;
    return o;
  }

  std::ostream& decindent (std::ostream& o)
  {
    assert (indent (o));
    indent (o) -= 2;
    return o;
  }

  std::ostream& resetindent (std::ostream& o)
  {
    indent (o) = 0;
    return o;
  }

  std::ostream& iendl (std::ostream& o)
  {
    o << std::endl;
    // Be sure to be able to restore the stream flags.
    char fill = o.fill (' ');
    return o << std::setw (indent (o))
	     << ""
	     << std::setfill (fill);
  }

  std::ostream& incendl (std::ostream& o)
  {
    return o << incindent << iendl;
  }

  std::ostream& decendl (std::ostream& o)
  {
    return o << decindent << iendl;
  }

} // end of namespace trackingClient.
