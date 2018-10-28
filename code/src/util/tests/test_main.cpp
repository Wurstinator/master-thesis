
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "../equivalence_relation.h"

tollk::EquivalenceRelation<unsigned char> TestEquivRelation1() {
    tollk::EquivalenceRelation<unsigned char>::EquivClass c1 {0};
    tollk::EquivalenceRelation<unsigned char>::EquivClass c2 {1, 3};
    tollk::EquivalenceRelation<unsigned char>::EquivClass c3 {6, 7, 8};
    std::vector<tollk::EquivalenceRelation<unsigned char>::EquivClass> classes {c1, c2, c3};
    tollk::EquivalenceRelation<unsigned char> relation(classes);
    return relation;
}

tollk::EquivalenceRelation<int> TestEquivRelation2() {
    tollk::EquivalenceRelation<int> relation;
    relation.AddConnection(0, 0);
    relation.AddConnection(1, 3);
    relation.AddConnection(1, 4);
    return relation;
}