/****************************************************************************
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2009      Valentin Milea
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2011      Zynga Inc.
 CopyRight (c) 2013-2016 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef CC_2D_ACTIONMANAGER_H
#define CC_2D_ACTIONMANAGER_H

#include <cocos/2d/CCActionManagerData.h>
#include <cocos/base/CCRef.h>
#include <cocos/base/CCVector.h>
#include <cocos/platform/CCPlatformDefine.h>
#include <cocos/platform/CCPlatformMacros.h>

#include <cstddef>
#include <list>
#include <vector>

NS_CC_BEGIN

class Action;
class Node;

/**
 * @addtogroup actions
 * @{
 */

/** @class ActionManager
 @brief ActionManager is a singleton that manages all the actions.
 Normally you won't need to use this singleton directly. 99% of the cases you will use the Node interface,
 which uses this singleton.
 But there are some cases where you might need to use this singleton.
 Examples:
 - When you want to run an action where the target is different from a Node.
 - When you want to pause / resume the actions.

 @since v0.8
 */
class CC_DLL ActionManager : public Ref
{
    std::list<Action*> _actions_to_process;
    ActionManagerData _actions;

public:
    ActionManager() = default;
    ActionManager(const ActionManager&) = delete;
    ActionManager& operator=(const ActionManager&) = delete;
    ActionManager(ActionManager&&) noexcept = delete;
    ActionManager& operator=(ActionManager&&) noexcept = delete;
    ~ActionManager();

    // actions

    /** Adds an action with a target.
     If the target is already present, then the action will be added to the existing target.
     If the target is not present, a new instance of this target will be created either paused or not, and the action will be added to the newly created target.
     When the target is paused, the queued actions won't be 'ticked'.
     *
     * @param action    A certain action.
     * @param target    The target which need to be added an action.
     * @param paused    Is the target paused or not.
     */
    void addAction(Action* action, Node* target, bool paused);

    /** Removes all actions from all the targets.
     */
    void removeAllActions();

    /** Removes all actions from a certain target.
     All the actions that belongs to the target will be removed.
     *
     * @param target    A certain target.
     */
    void removeAllActionsFromTarget(Node* target);

    /** Removes an action given an action reference.
     *
     * @param action    A certain target.
     */
    void removeAction(Action* action);

    /** Removes all actions given its tag and the target.
     *
     * @param tag       The actions' tag.
     * @param target    A certain target.
     * @js NA
     */
    void removeAllActionsByTag(int tag, Node* target);

    /** Removes all actions matching at least one bit in flags and the target.
     *
     * @param flags     The flag field to match the actions' flags based on bitwise AND.
     * @param target    A certain target.
     * @js NA
     */
    void removeActionsByFlags(unsigned int flags, Node* target);

    /** Gets an action given its tag an a target.
     *
     * @param tag       The action's tag.
     * @param target    A certain target.
     * @return  The Action the with the given tag.
     */
    Action* getActionByTag(int tag, Node* target) const;

    /** Returns the numbers of actions that are running in a certain target.
     * Composable actions are counted as 1 action. Example:
     * - If you are running 1 Sequence of 7 actions, it will return 1.
     * - If you are running 7 Sequences of 2 actions, it will return 7.
     *
     * @param target    A certain target.
     * @return  The numbers of actions that are running in a certain target.
     * @js NA
     */
    std::size_t getNumberOfRunningActionsInTarget(Node* target) const;

    /**
     * Added by Seb.Flory & Pierre.Marxen
     * Returns all actions currently running in a certain target.
     *
     * @param target    A certain target.
     * @return  A vector of actions that are running in a certain target.
     */
    std::vector<Action*> getRunningActionsInTarget(Node* target) const;

    /** @deprecated Use getNumberOfRunningActionsInTarget() instead.
     */
    CC_DEPRECATED_ATTRIBUTE std::size_t numberOfRunningActionsInTarget(Node* target) const { return getNumberOfRunningActionsInTarget(target); }

    /** Pauses the target: all running actions and newly added actions will be paused.
     *
     * @param target    A certain target.
     */
    void pauseTarget(Node* target);

    /** Resumes the target. All queued actions will be resumed.
     *
     * @param target    A certain target.
     */
    void resumeTarget(Node* target);

    /** Pauses all running actions, returning a list of targets whose actions were paused.
     *
     * @return  A list of targets whose actions were paused.
     */
    Vector<Node*> pauseAllRunningActions();

    /** Resume a set of targets (convenience function to reverse a pauseAllRunningActions call).
     *
     * @param targetsToResume   A set of targets need to be resumed.
     */
    void resumeTargets(const Vector<Node*>& targetsToResume);

    /** Main loop of ActionManager.
     * @param dt    In seconds.
     */
    void update(float dt);
};

// end of actions group
/// @}

NS_CC_END

#endif // CC_2D_ACTIONMANAGER_H
