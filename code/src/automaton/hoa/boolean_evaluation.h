
#pragma once

#include "../transition_automaton.h"
#include "cpphoafparser/ast/boolean_expression.hh"


namespace tollk {
namespace automaton {
namespace hoa {

struct SymbolInterpreter {
    symbol_t sym;

    bool operator()(const cpphoafparser::AtomLabel& label) const {
        return !!((sym >> label.getAPIndex()) & 1);
    }
};

// Evaluates the truth value of a given expression with provided values for the atoms.
template<typename T, typename F>
bool EvaluateHOABooleanExpr(const cpphoafparser::BooleanExpression<T>& expression, const F& values_function) {
    if (expression.isAtom())
        return values_function(expression.getAtom());
    if (expression.isTRUE())
        return true;
    if (expression.isFALSE())
        return false;
    if (expression.isAND())
        return EvaluateHOABooleanExpr(*expression.getLeft(), values_function) &&
               EvaluateHOABooleanExpr(*expression.getRight(), values_function);
    if (expression.isOR())
        return EvaluateHOABooleanExpr(*expression.getLeft(), values_function) ||
               EvaluateHOABooleanExpr(*expression.getRight(), values_function);
    if (expression.isNOT())
        return !EvaluateHOABooleanExpr(*expression.getLeft(), values_function);
    throw std::logic_error("Expression has invalid format.");
}

}
}
}