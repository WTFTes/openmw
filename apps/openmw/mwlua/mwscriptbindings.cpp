#include "mwscriptbindings.hpp"

#include <components/lua/luastate.hpp>

#include "../mwbase/world.hpp"
#include "../mwscript/scriptmanagerimp.hpp"

#include "object.hpp"

namespace MWLua
{
    struct MWScriptRef
    {
        ESM::RefId mId;
        sol::optional<GObject> mObj;

        MWScript::Locals& getLocals()
        {
            if (mObj)
                return mObj->ptr().getRefData().getLocals();
            else
                return MWBase::Environment::get().getScriptManager()->getGlobalScripts().getLocals(mId);
        }
    };
    struct MWScriptVariables
    {
        MWScriptRef mRef;
    };
}

namespace sol
{
    template <>
    struct is_automagical<MWLua::MWScriptRef> : std::false_type
    {
    };
    template <>
    struct is_automagical<MWLua::MWScriptVariables> : std::false_type
    {
    };
}

namespace MWLua
{

    sol::table initMWScriptBindings(const Context& context)
    {
        sol::table api(context.mLua->sol(), sol::create);

        api["getGlobalScript"]
            = [](std::string_view recordId, sol::optional<GObject> player) -> sol::optional<MWScriptRef> {
            if (player.has_value() && player->ptr() != MWBase::Environment::get().getWorld()->getPlayerPtr())
                throw std::runtime_error("Second argument must either be a player or be missing");
            auto scriptId = ESM::RefId::deserializeText(recordId);
            if (MWBase::Environment::get().getScriptManager()->getGlobalScripts().getScriptIfPresent(scriptId))
                return MWScriptRef{ scriptId, sol::nullopt };
            else
                return sol::nullopt;
        };
        api["getLocalScript"] = [](const GObject& obj, sol::optional<GObject> player) -> sol::optional<MWScriptRef> {
            if (player.has_value() && player->ptr() != MWBase::Environment::get().getWorld()->getPlayerPtr())
                throw std::runtime_error("Second argument must either be a player or be missing");
            auto scriptId = obj.ptr().getRefData().getLocals().getScriptId();
            if (scriptId.empty())
                return sol::nullopt;
            return MWScriptRef{ scriptId, obj };
        };

        // In multiplayer it will be possible to have several instances (per player) of a single script,
        // so we will likely add functions returning Lua table of scripts.
        // api["getGlobalScripts"] = [](std::string_view recordId) -> list of scripts
        // api["getLocalScripts"] = [](const GObject& obj) -> list of scripts

        sol::usertype<MWScriptRef> mwscript = context.mLua->sol().new_usertype<MWScriptRef>("MWScript");
        sol::usertype<MWScriptVariables> mwscriptVars
            = context.mLua->sol().new_usertype<MWScriptVariables>("MWScriptVariables");
        mwscript[sol::meta_function::to_string]
            = [](const MWScriptRef& s) { return std::string("MWScript{") + s.mId.toDebugString() + "}"; };
        mwscript["recordId"] = sol::readonly_property([](const MWScriptRef& s) { return s.mId.serializeText(); });
        mwscript["variables"] = sol::readonly_property([](const MWScriptRef& s) { return MWScriptVariables{ s }; });
        mwscript["object"] = sol::readonly_property([](const MWScriptRef& s) -> sol::optional<GObject> {
            if (s.mObj)
                return s.mObj;
            const MWScript::GlobalScriptDesc* script
                = MWBase::Environment::get().getScriptManager()->getGlobalScripts().getScriptIfPresent(s.mId);
            if (!script)
                throw std::runtime_error("Invalid MWScriptRef");
            const MWWorld::Ptr* ptr = script->getPtrIfPresent();
            if (ptr && !ptr->isEmpty())
                return GObject(*ptr);
            else
                return sol::nullopt;
        });
        mwscript["player"] = sol::readonly_property(
            [](const MWScriptRef&) { return GObject(MWBase::Environment::get().getWorld()->getPlayerPtr()); });
        mwscriptVars[sol::meta_function::index] = [](MWScriptVariables& s, std::string_view var) {
            return s.mRef.getLocals().getVarAsDouble(s.mRef.mId, Misc::StringUtils::lowerCase(var));
        };
        mwscriptVars[sol::meta_function::new_index] = [](MWScriptVariables& s, std::string_view var, double val) {
            MWScript::Locals& locals = s.mRef.getLocals();
            if (!locals.setVar(s.mRef.mId, Misc::StringUtils::lowerCase(var), val))
                throw std::runtime_error(
                    "No variable \"" + std::string(var) + "\" in mwscript " + s.mRef.mId.toDebugString());
        };

        return LuaUtil::makeReadOnly(api);
    }

}
