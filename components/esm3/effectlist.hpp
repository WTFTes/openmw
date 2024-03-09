#ifndef OPENMW_ESM_EFFECTLIST_H
#define OPENMW_ESM_EFFECTLIST_H

#include <cstdint>
#include <vector>

namespace ESM
{
    class ESMReader;
    class ESMWriter;

    /** Defines a spell effect. Shared between SPEL (Spells), ALCH
     (Potions) and ENCH (Item enchantments) records
     */
    struct ENAMstruct
    {
        // Magical effect, hard-coded ID
        int16_t mEffectID;

        // Which skills/attributes are affected (for restore/drain spells
        // etc.)
        signed char mSkill, mAttribute; // -1 if N/A

        // Other spell parameters
        int32_t mRange; // 0 - self, 1 - touch, 2 - target (RangeType enum)
        int32_t mArea, mDuration, mMagnMin, mMagnMax;
    };

    /// EffectList, ENAM subrecord
    struct EffectList
    {
        std::vector<ENAMstruct> mList;

        /// Load one effect, assumes subrecord name was already read
        void add(ESMReader& esm);

        /// Load all effects
        void load(ESMReader& esm);
        void save(ESMWriter& esm) const;
    };

}

#endif
