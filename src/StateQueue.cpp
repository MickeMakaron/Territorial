/****************************************************************
****************************************************************
*
* Territorial - 2D RTS game with dynamic territorial borders.
* Copyright (C) 2015 Mikael Hernvall (mikael.hernvall@gmail.com)
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
****************************************************************
****************************************************************/

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
