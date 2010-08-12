// Copyright (C) 2010 by Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#ifndef LLVC_TOOLS_VISP_IO_HH
# define LLVC_TOOLS_VISP_IO_HH
# include <iosfwd>

# include <visp/vpColor.h>
# include <visp/vpMatrix.h>

namespace trackingClient
{
  std::ostream& display(std::ostream& stream, const vpMatrix& matrix);

  std::ostream& operator<<(std::ostream& stream, const vpColor& color);
} // end of namespace trackingClient.

#endif // ! LLVC_TOOLS_VISP_IO_HH
