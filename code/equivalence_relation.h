
#pragma once


#include <unordered_map>
#include <unordered_set>
#include <vector>

// Represents an equivalence relation on an arbitrary type, i.e. a binary relation that is reflexive, symmetric, and
// transitive.
template<typename T>
class EquivalenceRelation {
public:
    using EquivClass = std::unordered_set<T>;

    // Creates a new empty relation.
    EquivalenceRelation() {}

    // Creates a new relation with the given equivalence classes.
    EquivalenceRelation(std::vector<EquivClass> classes);

    // TODO add element

    // TODO remove element

    // Returns the equivalence class of a given element.
    const EquivClass& GetClass(const T& x) const;

    // Checks whether two elements are equivalent.
    bool IsEquiv(const T& x, const T& y) const;

private:
    // From a list of classes, creates a relation map.
    static std::unordered_map<T, EquivClass*> InitRelation(const std::vector<EquivClass>& classes);

    std::vector<EquivClass> classes;
    std::unordered_map<T, EquivClass*> relation;
};

// Implementation

template<typename T>
EquivalenceRelation<T>::EquivalenceRelation(std::vector<EquivClass> classes)
        : classes(std::move(classes)), relation(EquivalenceRelation<T>::InitRelation(classes)) {};

template<typename T>
std::unordered_map<T, typename EquivalenceRelation<T>::EquivClass*>
EquivalenceRelation<T>::InitRelation(const std::vector<EquivClass>& classes) {
    std::unordered_map<T, EquivalenceRelation<T>::EquivClass*> relation;
    for (const EquivClass& clas : classes) {
        for (const T& x : clas) {
            relation[x] = &clas;
        }
    }
    return relation;
}

template<typename T>
const typename EquivalenceRelation<T>::EquivClass& EquivalenceRelation<T>::GetClass(const T& x) const {
    return this->relation[x];
}

template<typename T>
bool EquivalenceRelation<T>::IsEquiv(const T& x, const T& y) const {
    const EquivClass& c = GetClass(x);
    return c.find(y) != c.end();
};
