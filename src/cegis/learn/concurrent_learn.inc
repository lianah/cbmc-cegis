template<class learner1t, class learner2t>
concurrent_learnt<learner1t, learner2t>::concurrent_learnt(learner1t &learner1,
    learner2t &learner2, const serialisert serialiser,
    const deserialisert deserialiser) :
    learner1(learner1), learner2(learner2), serialiser(serialiser), deserialiser(
        deserialiser)
{
}

template<class learner1t, class learner2t>
concurrent_learnt<learner1t, learner2t>::~concurrent_learnt()
{
}
