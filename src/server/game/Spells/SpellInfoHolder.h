/*
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SpellInfoHolder_h__
#define SpellInfoHolder_h__

#include "Define.h"

#include <functional>
#include <unordered_map>

class SpellInfo;

struct SpellInfoLoadHelper
{
    SpellEntry const* Entry = nullptr;

    SpellAuraOptionsEntry const* AuraOptions = nullptr;
    SpellAuraRestrictionsEntry const* AuraRestrictions = nullptr;
    SpellCastingRequirementsEntry const* CastingRequirements = nullptr;
    SpellCategoriesEntry const* Categories = nullptr;
    SpellClassOptionsEntry const* ClassOptions = nullptr;
    SpellCooldownsEntry const* Cooldowns = nullptr;
    std::vector<SpellEffectEntry const*> Effects;
    SpellEquippedItemsEntry const* EquippedItems = nullptr;
    SpellInterruptsEntry const* Interrupts = nullptr;
    SpellLevelsEntry const* Levels = nullptr;
    SpellMiscEntry const* Misc = nullptr;
    std::vector<SpellPowerEntry const*> Powers;
    SpellReagentsEntry const* Reagents = nullptr;
    SpellScalingEntry const* Scaling = nullptr;
    SpellShapeshiftEntry const* Shapeshift = nullptr;
    SpellTargetRestrictionsEntry const* TargetRestrictions = nullptr;
    SpellTotemsEntry const* Totems = nullptr;
    std::vector<SpellXSpellVisualEntry const*> Visuals;
};

struct SpellInfoDifficultyLoadHelper
{
    typedef std::unordered_map<uint32 /*difficultyID*/, SpellInfoLoadHelper> StorageType;

    static void AutoCollect(StorageType& data);
};

struct SpellInfoHolder
{
    SpellInfoHolder() { }

    void Load(SpellInfoDifficultyLoadHelper::StorageType&& data, std::function<SpellInfo*(uint32 /*difficulty*/, SpellInfoLoadHelper&&)> loader);

    bool IsEmpty() const { _data.empty(); }
    SpellInfo const* Get(uint32 difficulty) const;

    template <typename Fn>
    void ForEach(Fn worker);

    typedef std::unordered_map<uint32 /*difficultyID*/, SpellInfo*> StorageType;

private:
    StorageType _data;
    bool _hasDifficultyData = false;
};

template<typename Fn>
void SpellInfoHolder::ForEach(Fn worker)
{
    for (StorageType::value_type const& pair : _data)
        worker(pair);
}

#endif // SpellInfoHolder_h__
