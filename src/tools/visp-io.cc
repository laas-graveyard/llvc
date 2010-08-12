// Copyright (C) 2010 by Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include <iostream>

#include <visp/vpMatrix.h>

#include "llvc/tools/indent.hh"
#include "llvc/tools/visp-io.hh"

namespace trackingClient
{
  //FIXME: use Boost.Format to align values.
  std::ostream& display(std::ostream& stream, const vpMatrix& matrix)
  {
    int m = matrix.getRows();
    int n = matrix.getCols();

    for (int i = 0; i < m; ++i)
      {
	for (int j = 0; j < n; ++j)
	  {
	    stream << matrix[i][j] << " ";
	  }
	stream << iendl;
      }
    return stream;
  }

  //FIXME: implement me.
  std::ostream& operator<<(std::ostream& stream, const vpColor& color)
  {
    stream << "FIXME";
    return stream;
      
  }
} // end of namespace trackingClient.
