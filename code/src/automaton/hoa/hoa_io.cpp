
#include "hoa_io.h"
#include "cpphoafparser/consumer/hoa_consumer.hh"
#include "cpphoafparser/parser/hoa_parser.hh"
#include "dpa_consumer.h"
#include "npa_consumer.h"
#include "hoa_consumer_debug.h"


namespace tollk {
namespace automaton {
namespace hoa {

DPA DPAFromHOA(std::istream* istream) {
    cpphoafparser::HOAConsumer::ptr consumer(new DPAConsumer());
    try {
        cpphoafparser::HOAParser::parse(*istream, consumer);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return dynamic_cast<DPAConsumer*>(consumer.get())->GetAutomaton();
}

NPA NPAFromHOA(std::istream* istream) {
    cpphoafparser::HOAConsumer::ptr consumer(new NPAConsumer());
    try {
        cpphoafparser::HOAParser::parse(*istream, consumer);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return dynamic_cast<NPAConsumer*>(consumer.get())->GetAutomaton();
}

}
}
}