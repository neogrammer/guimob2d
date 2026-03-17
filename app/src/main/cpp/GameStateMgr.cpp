//
// Created by jlhar on 3/15/2026.
//

#include "GameStateMgr.h"
#include "game_states/SplashState.h"
#include "game_states/TitleState.h"
#include "game_states/PlayState.h"
#include "Logging.h"
#include "Game.h"

#define topstate mStateStack.top().lock()

GameStateMgr::GameStateMgr(Game& game)
 : mGameStateMap{}, mHelperStack{}, mStateStack{}, mClearStopState{}, mClearStopStateInWait{}, mStateInWait{}, mStackNextState{}
 , mGame{&game}
{
  mGameStateMap.emplace(gstate::Splash, std::make_shared<SplashState>(*this));
  mGameStateMap.emplace(gstate::Title, std::make_shared<TitleState>(*this));
  mGameStateMap.emplace(gstate::Play, std::make_shared<PlayState>(*this));


  mStateStack.push(mGameStateMap[gstate::Splash]);
  if (!test(mStateStack.top())) {
      log("Something wrong with SplashState!");
  }

  topstate->Enter();
  mStateInWait = {};
  mClearStopState = topstate;
  mClearStopStateInWait = {};
}

GameStateMgr::~GameStateMgr() {

}


void GameStateMgr::readyUpState(gstate state, bool stackIt, bool updateClearStop, gstate nextClearState)
{


    if (mGameStateMap.find(state) == mGameStateMap.end() || state == gstate::None)
    {
        log("Readying Up a non-existent state!");
        mStateInWait.reset();
        mClearStopStateInWait.reset();

        return;
    }

    if (topstate == mGameStateMap.at(state))
    {
        log("Readying up state that is already the current state!");
        return;
    }

    if (mStateInWait.lock() != nullptr)
    {
        log("State in waiting currently, is being kicked out the line");
    }

    mStateInWait.reset();
    mStateInWait = mGameStateMap.at(state);
    mStackNextState = stackIt;

    if (mClearStopStateInWait.lock())
    {
        mClearStopStateInWait.reset();
    }

    if (updateClearStop)
    {
        if (nextClearState != gstate::None) {
            if (mGameStateMap.find(nextClearState) == mGameStateMap.end()) {
                log("nextClearState is not in the game state map");

            }
            else
            {
                bool found = false;
                // check the stack that we even have that state
                while (!mStateStack.empty())
                {
                    if (topstate == mGameStateMap.at(nextClearState))
                    {
                        found = true;
                        break;
                    }
                    else
                    {
                        mHelperStack.push(mStateStack.top());
                        mStateStack.pop();
                    }

                }
                if (nextClearState == state)
                {
                    found = true;
                }

                while (!mHelperStack.empty())
                {
                    mStateStack.push(mHelperStack.top());
                    mHelperStack.pop();
                }

                if (!found)
                {
                    log("Trying to ready up a state that is not in the stack or next!");
                    mClearStopStateInWait.reset();
                }
                else
                {
                    // found
                    mClearStopStateInWait = mGameStateMap[nextClearState];
                }
            }
        }
        else {
            if (stackIt == false)
            {
                while (!mStateStack.empty()) {
                    if (mClearStopState.lock() && mClearStopState.lock() == topstate) {
                        // set mClearStopStateInWait to one below the current topState
                        mHelperStack.push(topstate);
                        mStateStack.pop();
                        if (mStateStack.empty())
                        {
                            mClearStopStateInWait = mHelperStack.top().lock();
                        }
                        else {
                            mClearStopStateInWait = topstate;
                        }
                        mStateStack.push(mHelperStack.top().lock());
                        mHelperStack.pop();
                        break;
                    }
                    mHelperStack.push(topstate);
                    mStateStack.pop();
                }
                while (!mHelperStack.empty()) {
                    mStateStack.push(mHelperStack.top().lock());
                    mHelperStack.pop();
                }

            }
        }
    }
    else
    {
        // dont update it so it clears to the right state when not stacking
    }
}

bool GameStateMgr::isStateWaiting()
{
    if (test(mStateInWait))
    {
        return true;
    }
    return false;
}

void GameStateMgr::Update(float deltaTime)
{
    // check if a state is waiting first, then pop the top states until reaching clearStopState and then push the new state
    //  and update the mStateInWait, mClearStopState and mClearStopStateInWait and of course the stack mStateStack
    if (topstate == nullptr)
    {
        log("No topstate!");
        return;
    }

    if (mClearStopState.lock() == nullptr)
    {
        mClearStopState = topstate;
    }

    if (isStateWaiting())
    {
        if (!mStackNextState) {
            while (topstate != mClearStopState.lock() && !mStateStack.empty()) {
                topstate->Leave();
                mStateStack.pop();
            }
        }
        if(mStateStack.empty())
        {
            log("No states in the stack, adding back the splash screen");
            mStateStack.push(mGameStateMap[gstate::Splash]);
            mClearStopState = topstate;
            mClearStopStateInWait.reset();
            mStateInWait.reset();
            topstate->Enter();
        }
        else {
            mStateStack.push(mStateInWait);
            topstate->Enter();
            mStateInWait.reset();

            if (isClearStopStateWaiting()) {
                mClearStopState = mClearStopStateInWait;
                mClearStopStateInWait.reset();
            }
        }
        mStackNextState = false;
    }

    while (!mStateStack.empty())
    {
        mHelperStack.push(mStateStack.top());
        mStateStack.pop();
    }
    while (!mHelperStack.empty())
    {
        mStateStack.push(mHelperStack.top());
        mHelperStack.pop();
        topstate->Update(deltaTime);
    }

}

bool GameStateMgr::existsClearStopState() {
    if (mClearStopState.lock() == nullptr)
    {
        return false;
    }
    return true;
}

bool GameStateMgr::isClearStopStateWaiting() {
    if (mClearStopStateInWait.lock() == nullptr)
    {
        return false;
    }
    return true;
}

void GameStateMgr::Render()
{
    if (topstate == nullptr)
    {
        log("No topstate!");
        return;
    }
    while (!mStateStack.empty())
    {
        mHelperStack.push(mStateStack.top());
        mStateStack.pop();
    }
    while (!mHelperStack.empty())
    {
        mStateStack.push(mHelperStack.top());
        mHelperStack.pop();
        topstate->Render();
    }
}

void GameStateMgr::Input()
{
    if (topstate == nullptr)
    {
        log("No topstate!");
        return;
    }
    topstate->Input();
}

Game& GameStateMgr::getGame() {
    return *mGame;
}
