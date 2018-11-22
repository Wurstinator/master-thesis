

#include <algorithm>
#include <functional>
#include <cassert>

namespace tollk {


template<typename T>
EquivalenceRelation<T>::EquivalenceRelation(std::vector<EquivClass> classes)
        : classes(std::move(classes)), relation(EquivalenceRelation<T>::InitRelation(this->classes)) {};

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
    ClassIndex index1 = GetClassIndex(*c1.begin());
    ClassIndex index2 = GetClassIndex(*c2.begin());

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
    return IsEquiv(x, x);
}

template <typename T>
std::vector<T> EquivalenceRelation<T>::Domain() const {
    std::vector<T> domain;
    for (const EquivalenceRelation<T>::EquivClass& c : classes)
        std::copy(c.begin(), c.end(), std::back_inserter(domain));
    return domain;
}

template<typename T>
const typename EquivalenceRelation<T>::EquivClass& EquivalenceRelation<T>::GetClass(const T& x) const {
    return classes[GetClassIndex(x)];
}

template <typename T>
typename EquivalenceRelation<T>::ClassIndex EquivalenceRelation<T>::GetClassIndex(const T& x) const {
    return relation.at(x);
}

template<typename T>
bool EquivalenceRelation<T>::IsEquiv(const T& x, const T& y) const {
    const auto iter = relation.find(x);
    if (iter == relation.end())
        return false;
    const EquivClass& c = classes[iter->second];
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
            this->relation[x] = this->classes.size();
        }
    }
    for (const T& x : new_class)
        this->classes[i].erase(x);
    if (!new_class.empty())
        this->classes.push_back(std::move(new_class));
}

template <typename T>
EquivalenceRelation<T> EquivalenceRelation<T>::Intersection(const EquivalenceRelation<T>& lhs, const EquivalenceRelation<T>& rhs) {
    std::vector<EquivalenceRelation<T>::EquivClass> intersected_classes(lhs.Classes().size() * rhs.Classes().size());
    assert(lhs.Domain() == rhs.Domain());
    for (const T& x : lhs.Domain()) {
        const size_t i = lhs.GetClassIndex(x);
        const size_t j = rhs.GetClassIndex(x);
        intersected_classes[i + j*lhs.Classes().size()].insert(x);
    }
    intersected_classes.erase(std::remove_if(intersected_classes.begin(), intersected_classes.end(), std::mem_fn(&EquivClass::empty)), intersected_classes.end());
    return EquivalenceRelation<T>(intersected_classes);
}


template <typename T>
bool operator==(const EquivalenceRelation<T>& lhs, const EquivalenceRelation<T>& rhs) {
    // Check that lhs and rhs have the same domain.
    const std::vector<T> lhs_domain = lhs.Domain();
    const std::vector<T> rhs_domain = rhs.Domain();
    if (lhs_domain.size() != rhs_domain.size())
        return false;
    if (std::any_of(lhs_domain.begin(), lhs_domain.end(), [&rhs](const T& element) {return !rhs.Exists(element);}))
        return false;

    // Check that the equivalence classes are the same.
    for (const T& a : lhs_domain) {
        for (const T& b : lhs_domain) {
            const bool lhs_equiv = lhs.IsEquiv(a, b);
            const bool rhs_equiv = rhs.IsEquiv(a, b);
            if ((lhs_equiv && !rhs_equiv) || (!lhs_equiv && rhs_equiv))
                return false;
        }
    }
    return true;
}



}