

namespace tollk {
namespace automaton {

template<typename LabelT>
void LabelledAutomaton<LabelT>::AddState(state_t q) {
    FiniteAutomaton::AddState(q);
    this->labels[q] = Label();
}

template<typename LabelT>
void LabelledAutomaton<LabelT>::RemoveState(state_t q) {
    FiniteAutomaton::RemoveState(q);
    this->labels.erase(this->labels.find(q));
}

template<typename LabelT>
const typename LabelledAutomaton<LabelT>::Label& LabelledAutomaton<LabelT>::GetLabel(state_t q) const {
    return this->labels.find(q)->second;
}

template<typename LabelT>
void LabelledAutomaton<LabelT>::SetLabel(state_t q, Label label) {
    this->labels[q] = label;
}

template<typename LabelT>
EquivalenceRelation <state_t> LabelledAutomaton<LabelT>::LabelEquivalence() const {
    EquivalenceRelation<state_t> result;
    // For each label, memorize some representative state.
    std::map<LabelT, state_t> representatives;

    for (state_t q : States()) {
        // Check if the label has an assigned representative already.
        const auto iter = representatives.find(GetLabel(q));
        if (iter == representatives.end()) {
            // If a label without a representative was found, create a new equivalence class...
            representatives[GetLabel(q)] = q;
            result.AddConnection(q, q);
        } else {
            // ..otherwise simply add q to the existing equivalence class.
            result.AddConnection(q, iter->second);
        }
    }
    return result;
}


template<typename LabelT>
std::unordered_set<typename LabelledAutomaton<LabelT>::Label> LabelledAutomaton<LabelT>::AllLabels() const {
    std::unordered_set<typename LabelledAutomaton<LabelT>::Label> result;
    for (const std::pair<const state_t, Label>& kv_pair : this->labels)
        result.insert(kv_pair.second);
    return result;
}


template<typename LabelT>
DeterministicLabelledAutomaton<LabelT>::DeterministicLabelledAutomaton(const DeterministicAutomaton& automaton) :
        DeterministicAutomaton(automaton) {}

template<typename LabelT>
DeterministicLabelledAutomaton<LabelT>::DeterministicLabelledAutomaton(DeterministicAutomaton&& automaton) :
        DeterministicAutomaton(std::move(automaton)) {}

template<typename LabelT>
DeterministicLabelledAutomaton <LabelT> DeterministicLabelledAutomaton<LabelT>::FromNondeterministic(
        const NondeterministicLabelledAutomaton <LabelT>& automaton) {
    DeterministicLabelledAutomaton<LabelT> det(DeterministicAutomaton::FromTransitionAutomaton(automaton));
    for (state_t q : det.States())
        det.SetLabel(q, automaton.GetLabel(q));
    return det;
}

template<typename LabelT>
void DeterministicLabelledAutomaton<LabelT>::AddState(state_t q) {
    DeterministicAutomaton::AddState(q);
    this->labels[q] = LabelT();
}

template<typename LabelT>
void DeterministicLabelledAutomaton<LabelT>::RemoveState(state_t q) {
    DeterministicAutomaton::RemoveState(q);
    this->labels.erase(this->labels.find(q));
}


template<typename LabelT>
NondeterministicLabelledAutomaton<LabelT>::NondeterministicLabelledAutomaton(const NondeterministicAutomaton& automaton) :
        NondeterministicAutomaton(automaton) {}

template<typename LabelT>
NondeterministicLabelledAutomaton<LabelT>::NondeterministicLabelledAutomaton(NondeterministicAutomaton&& automaton) :
        NondeterministicAutomaton(std::move(automaton)) {}

template<typename LabelT>
NondeterministicLabelledAutomaton <LabelT>
NondeterministicLabelledAutomaton<LabelT>::FromDeterministic(const DeterministicLabelledAutomaton <LabelT>& automaton) {
    NondeterministicLabelledAutomaton<LabelT> ndet(NondeterministicAutomaton::FromTransitionAutomaton(automaton));
    for (state_t q : ndet.States())
        ndet.SetLabel(q, automaton.GetLabel(q));
    return ndet;
}

template<typename LabelT>
void NondeterministicLabelledAutomaton<LabelT>::AddState(state_t q) {
    NondeterministicAutomaton::AddState(q);
    this->labels[q] = LabelT();
}

template<typename LabelT>
void NondeterministicLabelledAutomaton<LabelT>::RemoveState(state_t q) {
    NondeterministicAutomaton::RemoveState(q);
    this->labels.erase(this->labels.find(q));
}


}  // namespace automaton
}  // namespace tollk