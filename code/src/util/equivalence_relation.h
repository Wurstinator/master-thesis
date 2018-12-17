
#pragma once

#include <unordered_map>
#include <set>
#include <vector>

namespace tollk {


// Represents an equivalence relation on an arbitrary type, i.e. a binary relation that is reflexive, symmetric, and
// transitive.
template<typename T>
class EquivalenceRelation {
public:
    using EquivClass = std::set<T>;
    using ClassIndex = typename std::vector<EquivClass>::size_type;

    // Creates a new empty relation.
    EquivalenceRelation() = default;

    // Creates a new relation with the given equivalence classes.
    explicit EquivalenceRelation(std::vector<EquivClass> classes);

    // Adds a new connection between to elements to the relation. If neither of them is present already, a new class
    // is created. If only one of them is present already, the other one is added to its class. If both exist in the
    // relation, their classes are merged. (note that the two elements can be the same to create a new singleton class)
    void AddConnection(const T& x, const T& y);

    // Removes an element from the relation. If its equivalence class contained no other state, that class is deleted.
    void Remove(const T& x);

    // Removes all elements from the relation that are not in a given range.
    template <typename RngT>
    void RestrictDomain(RngT&& new_domain);

    // Merges two given equivalence classes.
    void MergeClasses(const EquivClass& c1, const EquivClass& c2);

    // Returns whether some element is already present in the relation in some form.
    bool Exists(const T& x) const;

    // Returns a list of all elements that are part of any class.
    std::vector<T> Domain() const;

    // Returns the equivalence class of a given element.
    const EquivClass& GetClass(const T& x) const;

    // Returns the index in the classes array of the equivalence class of a given element.
    ClassIndex GetClassIndex(const T& x) const;

    // Checks whether two elements are equivalent.
    bool IsEquiv(const T& x, const T& y) const;

    // Returns the vector of classes.
    const std::vector<EquivClass>& Classes() const;

    // Splits a class C at index i into two new classes: C \ X and C ∩ X. O(|C|) operation.
    void SplitClass(typename std::vector<EquivClass>::size_type i, const EquivClass& X);

    // Builds the intersection of two equivalence classes.
    static EquivalenceRelation<T> Intersection(const EquivalenceRelation<T>& lhs, const EquivalenceRelation<T>& rhs);

    template <typename S>
    friend bool operator==(const EquivalenceRelation<S>& lhs, const EquivalenceRelation<S>& rhs);

private:
    using RelationMap = std::unordered_map<T, ClassIndex>;

    // From a list of classes, creates a relation map.
    static RelationMap InitRelation(const std::vector<EquivClass>& classes);

    std::vector<EquivClass> classes;
    RelationMap relation;
};

template <typename T>
bool operator==(const EquivalenceRelation<T>& lhs, const EquivalenceRelation<T>& rhs);

}  // namespace tollk


#include "equivalence_relation.impl.h"