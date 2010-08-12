// Copyright (C) 2010 by Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#ifndef LLVC_TOOLS_INDENT_HH
# define LLVC_TOOLS_INDENT_HH
# include <iosfwd>

namespace trackingClient
{
  /// \brief The current indentation level for \a o.
  long int& indent (std::ostream& o);

  /// \brief Increment the indentation.
  std::ostream& incindent (std::ostream& o);

  /// \brief Decrement the indentation.
  std::ostream& decindent (std::ostream& o);

  /// \brief Reset the indentation.
  std::ostream& resetindent (std::ostream& o);

  /// \brief Print an end of line, then set the indentation.
  std::ostream& iendl (std::ostream& o);

  /// \brief Increment the indentation, print an end of line,
  /// and set the indentation.
  std::ostream& incendl (std::ostream& o);

  /// \brief  Decrement the indentation, print an end of line,
  /// and set the indentation.
  std::ostream& decendl (std::ostream& o);

} // end of namespace trackingClient.

#endif // ! LLVC_TOOLS_INDENT_HH
