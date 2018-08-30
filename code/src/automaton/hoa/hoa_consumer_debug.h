
#pragma once

#include <iostream>
#include "cpphoafparser/consumer/hoa_consumer.hh"

namespace tollk {
namespace automaton {
namespace hoa {

// This HOAConsumer can be used to gain additional information about a HOA file that is read. Each function call
// prints a detailed message about its call.
class HOAConsumerDebug : public cpphoafparser::HOAConsumer {
 public:
    template<typename T>
    void print(T x) {
        std::cout << x;
    }

    template<typename T>
    void print(const std::vector<T>& vec) {
        std::cout << "[";
        if (!vec.empty()) {
            for (auto iter = vec.cbegin(); std::next(iter) != vec.cend(); ++iter) {
                print(*iter);
                std::cout << ", ";
            }
            std::cout << vec.back();
        }
        std::cout << "]";
    }

    template<typename T>
    void print(T* ptr) {
        if (ptr == nullptr)
            std::cout << "NULL";
        else
            print(*ptr);
    }

    template<typename T>
    void print(const T* ptr) {
        if (ptr == nullptr)
            std::cout << "NULL";
        else
            print(*ptr);
    }

    bool parserResolvesAliases() override {
        std::cout << "HOAConsumerDebug::parserResolvesAliases()" << std::endl;
        return false;
    }

    void notifyHeaderStart(const std::string& version) override {
        std::cout << "HOAConsumerDebug::notifyHeaderStart(";
        print(version);
        std::cout << ")" << std::endl;
    }

    void setNumberOfStates(unsigned int numberOfStates) override {
        std::cout << "HOAConsumerDebug::setNumberOfStates(";
        print(numberOfStates);
        std::cout << ")" << std::endl;
    }

    void addStartStates(const int_list& stateConjunction) override {
        std::cout << "HOAConsumerDebug::addStartStates(";
        print(stateConjunction);
        std::cout << ")" << std::endl;
    }

    void addAlias(const std::string& name, label_expr::ptr labelExpr) override {
        std::cout << "HOAConsumerDebug::addAlias(";
        print(name);
        std::cout << ", ";
        print(labelExpr.get());
        std::cout << ")" << std::endl;
    }

    void setAPs(const std::vector<std::string>& aps) override {
        std::cout << "HOAConsumerDebug::setAPs(";
        print(aps);
        std::cout << ")" << std::endl;
    }

    void setAcceptanceCondition(unsigned int numberOfSets, acceptance_expr::ptr accExpr) override {
        std::cout << "HOAConsumerDebug::setAcceptanceCondition(";
        print(numberOfSets);
        std::cout << ")" << std::endl;
        print(accExpr.get());
        std::cout << ")" << std::endl;
    }

    void
    provideAcceptanceName(const std::string& name, const std::vector<cpphoafparser::IntOrString>& extraInfo) override {
        std::cout << "HOAConsumerDebug::provideAcceptanceName(";
        print(name);
        std::cout << ", ";
        print(extraInfo);
        std::cout << ")" << std::endl;
    }

    void setName(const std::string& name) override {
        std::cout << "HOAConsumerDebug::setName(";
        print(name);
        std::cout << ")" << std::endl;
    }

    void setTool(const std::string& name, std::shared_ptr<std::string> version) override {
        std::cout << "HOAConsumerDebug::setTool(";
        print(name);
        std::cout << ", ";
        print(version.get());
        std::cout << ")" << std::endl;
    }

    void addProperties(const std::vector<std::string>& properties) override {
        std::cout << "HOAConsumerDebug::addProperties(";
        print(properties);
        std::cout << ")" << std::endl;
    };

    void addMiscHeader(const std::string& name, const std::vector<cpphoafparser::IntOrString>& content) override {
        std::cout << "HOAConsumerDebug::addMiscHeader(";
        print(name);
        std::cout << "";
        print(content);
        std::cout << ")" << std::endl;
    }

    void notifyBodyStart() override {
        std::cout << "HOAConsumerDebug::notifyBodyStart()" << std::endl;
    }

    void addState(unsigned int id, std::shared_ptr<std::string> info, label_expr::ptr labelExpr,
                  std::shared_ptr<int_list> accSignature) override {
        std::cout << "HOAConsumerDebug::addState(";
        print(id);
        std::cout << ", ";
        print(info.get());
        std::cout << ", ";
        print(labelExpr.get());
        std::cout << ", ";
        print(*accSignature);
        std::cout << ")" << std::endl;
    }

    void addEdgeImplicit(unsigned int stateId, const int_list& conjSuccessors,
                         std::shared_ptr<int_list> accSignature) override {
        std::cout << "HOAConsumerDebug::addEdgeImplicit(";
        print(stateId);
        std::cout << ", ";
        print(conjSuccessors);
        std::cout << ", ";
        print(accSignature.get());
        std::cout << ")" << std::endl;
    }

    void addEdgeWithLabel(unsigned int stateId, label_expr::ptr labelExpr, const int_list& conjSuccessors,
                          std::shared_ptr<int_list> accSignature) override {
        std::cout << "HOAConsumerDebug::addEdgeWithLabel(";
        print(stateId);
        std::cout << ", ";
        print(labelExpr.get());
        std::cout << ", ";
        print(conjSuccessors);
        std::cout << ", ";
        print(accSignature.get());
        std::cout << ")" << std::endl;
    }

    void notifyEndOfState(unsigned int stateId) override {
        std::cout << "HOAConsumerDebug::notifyEndOfState(";
        print(stateId);
        std::cout << ")" << std::endl;
    }

    void notifyEnd() override {
        std::cout << "HOAConsumerDebug::notifyEnd()" << std::endl;
    }

    void notifyAbort() override {
        std::cout << "HOAConsumerDebug::notifyAbort()" << std::endl;
    }

    void notifyWarning(const std::string& warning) override {
        std::cout << "HOAConsumerDebug::notifyWarning(";
        print(warning);
        std::cout << ")" << std::endl;
    }
};

}
}
}