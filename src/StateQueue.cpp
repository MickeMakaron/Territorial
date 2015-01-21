#include "StateQueue.hpp"


StateQueue::StateQueue(StatePtr defaultState)
: mDefaultState(std::move(defaultState))
{

}

void StateQueue::update()
{
    if(!mStateQueue.empty())
    {
        EntityState* pState = mStateQueue.front().get();
        pState->update();

        if(pState->isDone())
        {
            mStateQueue.pop();

            if(!mStateQueue.empty())
                mStateQueue.front()->initialize();
        }
    }
    else
        mDefaultState->update();
}

void StateQueue::setState(StatePtr state)
{
    // Clear queue.
    std::queue<StatePtr> empty;
    mStateQueue.swap(empty);

    state->initialize();
    mStateQueue.push(std::move(state));
}

void StateQueue::pushState(StatePtr state)
{
    if(mStateQueue.empty())
        state->initialize();

    mStateQueue.push(std::move(state));
}

bool StateQueue::isEmpty() const
{
    return mStateQueue.empty();
}

const StateQueue::StatePtr& StateQueue::getState() const
{
    if(isEmpty())
        return mDefaultState;
    else
        return mStateQueue.front();
}
