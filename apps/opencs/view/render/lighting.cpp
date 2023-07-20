#include "lighting.hpp"

#include <string>

#include <osg/Group>
#include <osg/LightSource>
#include <osg/NodeVisitor>
#include <osg/Object>
#include <osg/Switch>
#include <osg/ValueObject>

#include <apps/opencs/model/prefs/category.hpp>
#include <apps/opencs/model/prefs/setting.hpp>

#include <components/misc/constants.hpp>

#include "../../model/prefs/state.hpp"

class DayNightSwitchVisitor : public osg::NodeVisitor
{
public:
    DayNightSwitchVisitor(int index)
        : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN)
        , mIndex(index)
    {
    }

    void apply(osg::Switch& switchNode) override
    {
        constexpr int NoIndex = -1;

        int initialIndex = NoIndex;
        if (!switchNode.getUserValue("initialIndex", initialIndex))
        {
            for (size_t i = 0; i < switchNode.getValueList().size(); ++i)
            {
                if (switchNode.getValueList()[i])
                {
                    initialIndex = i;
                    break;
                }
            }

            if (initialIndex != NoIndex)
                switchNode.setUserValue("initialIndex", initialIndex);
        }

        if (CSMPrefs::get()["Rendering"]["scene-day-night-switch-nodes"].isTrue())
        {
            if (switchNode.getName() == Constants::NightDayLabel)
                switchNode.setSingleChildOn(mIndex);
        }
        else if (initialIndex != NoIndex)
        {
            switchNode.setSingleChildOn(initialIndex);
        }

        traverse(switchNode);
    }

private:
    int mIndex;
};

void CSVRender::Lighting::updateDayNightMode(int index)
{
    if (mRootNode == nullptr)
        return;

    DayNightSwitchVisitor visitor(index);
    mRootNode->accept(visitor);
}
