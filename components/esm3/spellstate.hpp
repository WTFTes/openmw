#ifndef OPENMW_ESM_SPELLSTATE_H
#define OPENMW_ESM_SPELLSTATE_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "components/esm/defs.hpp"
#include "components/esm/refid.hpp"

namespace ESM
{
    class ESMReader;
    class ESMWriter;
    class RefId;

    // NOTE: spell ids must be lower case
    struct SpellState
    {
        struct CorprusStats
        {
            int mWorsenings;
            TimeStamp mNextWorsening;
        };

        struct PermanentSpellEffectInfo
        {
            int mId;
            int mArg;
            float mMagnitude;
        };

        struct SpellParams
        {
            std::map<int, float> mEffectRands; // <effect index, normalised random magnitude>
            std::set<int> mPurgedEffects; // indices of purged effects
        };
        std::vector<ESM::RefId> mSpells;

        // FIXME: obsolete, used only for old saves
        std::map<ESM::RefId, SpellParams> mSpellParams;
        std::map<ESM::RefId, std::vector<PermanentSpellEffectInfo>> mPermanentSpellEffects;
        std::map<ESM::RefId, CorprusStats> mCorprusSpells;

        std::map<ESM::RefId, TimeStamp> mUsedPowers;

        ESM::RefId mSelectedSpell;

        void load(ESMReader& esm);
        void save(ESMWriter& esm) const;
    };

}

#endif
