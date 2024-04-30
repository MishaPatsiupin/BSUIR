#include "logger.h"


std::ostream& operator<<(std::ostream& strm, const boost::posix_time::ptime& pt)
{
    if(!pt.is_not_a_date_time())
    {
        boost::posix_time::time_facet* facet = new boost::posix_time::time_facet();
        facet->format("%Y-%m-%d %H:%M"); // Измените формат времени здесь
        strm.imbue(std::locale(strm.getloc(), facet));
        strm << pt;
    }
    return strm;
}

void setup_logger() {
    auto sink_backend = boost::make_shared<sinks::text_file_backend>(
            keywords::file_name = "logs.txt",
            keywords::auto_flush = true  // Enable auto flushing after each log record
    );

    auto sink = boost::make_shared<sinks::synchronous_sink<sinks::text_file_backend>>(sink_backend);

    sink->set_formatter(
            expr::format("[%1%] [%2%] %3%")
            % expr::attr< boost::posix_time::ptime >("TimeStamp")
            % expr::attr< unsigned int >("LineID")
            % expr::smessage
    );

    logging::core::get()->add_sink(sink);

    logging::add_common_attributes();  // Add common attributes such as TimeStamp and LineID

    // Now all logs will be flushed immediately
    BOOST_LOG_TRIVIAL(info) << "information";
}