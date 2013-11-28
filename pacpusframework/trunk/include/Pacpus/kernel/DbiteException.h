// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @file
/// @author  Marek Kurdej <firstname.surname@utc.fr>
/// @date    Month, 2012
/// @version $Id: DbiteException.h 117 2013-06-25 11:50:46Z kurdejma $
/// @copyright Copyright (c) UTC/CNRS Heudiasyc 2006 - 2013. All rights reserved.
/// @brief Brief description.
///
/// Detailed description.

#ifndef DEF_PACPUS_DBITEEXCEPTION_H
#define DEF_PACPUS_DBITEEXCEPTION_H

#include <Pacpus/kernel/FileLibConfig.h>
#include <Pacpus/kernel/PacpusException.h>

#include <exception>
#include <string>

namespace pacpus {

/// Exception thrown when an error during manipulation a dbite file occurs.
class FILELIB_API DbiteException
    : virtual public PacpusException
{
public:
    /// Ctor.
    /// @param what Information about the exception.
    DbiteException(const std::string & what);

    /// Dtor.
    virtual ~DbiteException() throw();
};

} // namespace pacpus

#endif // DEF_PACPUS_DBITEEXCEPTION_H