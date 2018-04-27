
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

    // Creates a new empty relation.
    EquivalenceRelation() = default;

    // Creates a new relation with the given equivalence classes.
    explicit EquivalenceRelation(std::vector<EquivClass> classes);

    // Adds a new connection between to elements to the relation. If neither of them is present already, a new class
    // is created. If only one of them is present already, the other one is added to its class. If both exist in the
    // relation, their classes are merged. (note that the two elements can be the same to create a new singleton class)
    void AddConnection(const T& x, const T& y);

    // TODO remove element

    // Merges two given equivalence classes.
    void MergeClasses(const EquivClass& c1, const EquivClass& c2);

    // Returns whether some element is already present in the relation in some form.
    bool Exists(const T& x) const;

    // Returns the equivalence class of a given element.
    const EquivClass& GetClass(const T& x) const;

    // Checks whether two elements are equivalent.
    bool IsEquiv(const T& x, const T& y) const;

    // Returns the vector of classes.
    const std::vector<EquivClass>& Classes() const;

    // Splits a class C at index i into two new classes: C \ X and C ∩ X. O(|C|) operation.
    void SplitClass(typename std::vector<EquivClass>::size_type i, const EquivClass& X);

private:
    using RelationMap = std::unordered_map<T, typename std::vector<EquivClass>::size_type>;

    // From a list of classes, creates a relation map.
    static RelationMap InitRelation(const std::vector<EquivClass>& classes);

    std::vector<EquivClass> classes;
    RelationMap relation;
};

// Implementation

template<typename T>
EquivalenceRelation<T>::EquivalenceRelation(std::vector<EquivClass> classes)
        : classes(std::move(classes)), relation(EquivalenceRelation<T>::InitRelation(classes)) {};

template<typename T>
typename EquivalenceRelation<T>::RelationMap
EquivalenceRelation<T>::InitRelation(const std::vector<EquivClass>& classes) {
    RelationMap relation;
    for (typename std::vector<EquivClass>::size_type i = 0; i < classes.size(); ++i) {
        for (const T& x : classes[i]) {
            relation[x] = i;
        }
    }
    return relation;
}

template<typename T>
void EquivalenceRelation<T>::AddConnection(const T& x, const T& y) {
    if (Exists(x)) {
        if (Exists(y)) {
            MergeClasses(GetClass(x), GetClass(y));
        } else {
            classes[relation[x]].insert(y);
            relation[y] = relation[x];
        }
    } else {
        if (Exists(y)) {
            classes[relation[y]].insert(x);
            relation[x] = relation[y];
        } else {
            const EquivClass new_class{x, y};
            classes.push_back(new_class);
            relation[x] = relation[y] = classes.size() - 1;
        }
    }
};


template<typename T>
void EquivalenceRelation<T>::MergeClasses(const EquivalenceRelation::EquivClass& c1,
                                          const EquivalenceRelation::EquivClass& c2) {
    typename std::vector<EquivClass>::size_type index1 = relation[*c1.begin()];
    typename std::vector<EquivClass>::size_type index2 = relation[*c2.begin()];

    if (index1 == index2)
        return;

    if (index1 > index2) {
        MergeClasses(c2, c1);
        return;
    }

    for (auto i = index2 + 1; i < classes.size(); ++i) {
        for (const T& x : classes[i]) {
            relation[x] -= 1;
        }
    }

    for (const T& x : c2)
        relation[x] = index1;
    std::copy(c2.begin(), c2.end(), std::inserter(classes[index1], classes[index1].begin()));

    classes.erase(classes.begin() + index2);
}


template<typename T>
bool EquivalenceRelation<T>::Exists(const T& x) const {
    return this->relation.find(x) != this->relation.end();
}


template<typename T>
const typename EquivalenceRelation<T>::EquivClass& EquivalenceRelation<T>::GetClass(const T& x) const {
    return classes[relation.at(x)];
}

template<typename T>
bool EquivalenceRelation<T>::IsEquiv(const T& x, const T& y) const {
    const EquivClass& c = GetClass(x);
    return c.find(y) != c.end();
}

template<typename T>
const std::vector<typename EquivalenceRelation<T>::EquivClass>& EquivalenceRelation<T>::Classes() const {
    return classes;
}

template<typename T>
void EquivalenceRelation<T>::SplitClass(typename std::vector<EquivalenceRelation<T>::EquivClass>::size_type i, const EquivalenceRelation::EquivClass& X) {
    EquivClass new_class;
    for (const T& x : this->classes[i]) {
        if (X.find(x) != X.end()) {
            new_class.insert(x);
            this->classes[i].erase(x);
            this->relation[x] = this->classes.size();
        }
    }
    if (!new_class.empty())
        this->classes.push_back(std::move(new_class));
}


}  // namespace tollk