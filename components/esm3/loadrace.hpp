#ifndef OPENMW_ESM_RACE_H
#define OPENMW_ESM_RACE_H

#include <array>
#include <string>

#include "components/esm/defs.hpp"
#include "components/esm/refid.hpp"
#include "spelllist.hpp"

namespace ESM
{

    class ESMReader;
    class ESMWriter;

    /*
     * Race definition
     */

    struct Race
    {
        constexpr static RecNameInts sRecordId = REC_RACE;

        /// Return a string descriptor for this record type. Currently used for debugging / error logs only.
        static std::string_view getRecordType() { return "Race"; }

        struct SkillBonus
        {
            int mSkill; // SkillEnum
            int mBonus;
        };

        struct MaleFemale
        {
            int mMale, mFemale;

            int getValue(bool male) const;
        };

        struct MaleFemaleF
        {
            float mMale, mFemale;

            float getValue(bool male) const;
        };

        enum Flags
        {
            Playable = 0x01,
            Beast = 0x02
        };

        struct RADTstruct
        {
            // List of skills that get a bonus
            std::array<SkillBonus, 7> mBonus;

            // Attribute values for male/female
            std::array<MaleFemale, 8> mAttributeValues;

            // The actual eye level height (in game units) is (probably) given
            // as 'height' times 128. This has not been tested yet.
            MaleFemaleF mHeight, mWeight;

            int mFlags; // 0x1 - playable, 0x2 - beast race

        }; // Size = 140 bytes

        RADTstruct mData;

        unsigned int mRecordFlags;
        std::string mName, mDescription;
        RefId mId;
        SpellList mPowers;

        void load(ESMReader& esm, bool& isDeleted);
        void save(ESMWriter& esm, bool isDeleted = false) const;

        void blank();
        ///< Set record to default state (does not touch the ID/index).
    };

}
#endif
