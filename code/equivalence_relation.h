
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

private:
    // From a list of classes, creates a relation map.
    static std::unordered_map<T, std::vector<EquivClass>::size_type> InitRelation(const std::vector<EquivClass>& classes);

    std::vector<EquivClass> classes;
    std::unordered_map<T, std::vector<EquivClass>::size_type> relation;
};

// Implementation

template<typename T>
EquivalenceRelation<T>::EquivalenceRelation(std::vector<EquivClass> classes)
        : classes(std::move(classes)), relation(EquivalenceRelation<T>::InitRelation(classes)) {};

template<typename T>
std::unordered_map<T, std::vector<EquivalenceRelation<T>::EquivClass>::size_type>
EquivalenceRelation<T>::InitRelation(const std::vector<EquivClass>& classes) {
    std::unordered_map<T, std::vector<EquivClass>::size_type> relation;
    for (std::vector<EquivClass>::size_type i = 0; i < classes.size(); ++i) {
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
            classes[x].insert(y);
            relation[y] = relation[x];
        }
    } else {
        if (Exists(y)) {
            classes[y].insert(x);
            relation[x] = relation[y];
        } else {
            const EquivClass new_class {x, y};
            classes.push_back(new_class);
            relation[x] = relation[y] = classes.size() - 1;
        }
    }
};


template<typename T>
void EquivalenceRelation<T>::MergeClasses(const EquivalenceRelation::EquivClass& c1,
                                          const EquivalenceRelation::EquivClass& c2) {
    std::vector<EquivClass>::size_type index1 = relation[*c1.begin()];
    std::vector<EquivClass>::size_type index2 = relation[*c2.begin()];

    if (index1 == index2)
        return;

    if (index1 > index2) {
        MergeClasses(c2, c1);
        return;
    }

    for (std::vector<EquivClass>::size_type i = index2 + 1; i < classes.size(); ++i) {
        for (const T& x : classes[i]) {
            relation[x] -= 1;
        }
    }
    classes.erase(classes.begin() + index2);

    for (const T& x : c2)
        relation[x] = index1;
    std::copy(c2.begin(), c2.end(), std::inserter(classes[index1]));
}


template<typename T>
bool EquivalenceRelation<T>::Exists(const T& x) const {
    return this->relation.find(x) != this->relation.end();
}


template<typename T>
const typename EquivalenceRelation<T>::EquivClass& EquivalenceRelation<T>::GetClass(const T& x) const {
    return this->relation.at(x);
}

template<typename T>
bool EquivalenceRelation<T>::IsEquiv(const T& x, const T& y) const {
    const EquivClass& c = GetClass(x);
    return c.find(y) != c.end();
}
