#ifndef CHATTY__ARCHIVE_HXX
#define CHATTY__ARCHIVE_HXX

#include <boost/serialization/split_member.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/export.hpp>
#include <boost/date_time/posix_time/time_serialize.hpp>
#include <boost/serialization/extended_type_info.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

typedef boost::archive::text_oarchive oarchive;
typedef boost::archive::text_iarchive iarchive;

#endif /* CHATTY__ARCHIVE_HXX */