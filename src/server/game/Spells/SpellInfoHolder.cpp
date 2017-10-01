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

#include "DB2Stores.h"
#include "SharedDefines.h"
#include "SpellInfoHolder.h"

void SpellInfoDifficultyLoadHelper::AutoCollect(SpellInfoDifficultyLoadHelper::StorageType& data)
{
    std::vector<DifficultyEntry const*> difficultyEntries;
    for (DifficultyEntry const* difficultyEntry : sDifficultyStore)
        difficultyEntries.push_back(difficultyEntry);
    std::sort(difficultyEntries.begin(), difficultyEntries.end(), [](DifficultyEntry const* first, DifficultyEntry const* second)
    {
        return first->OrderIndex < second->OrderIndex;
    });

    auto overrideFn = [](SpellInfoLoadHelper& helper, SpellInfoLoadHelper const& overrideHelper)
    {
#define OverrideEntry(Entry) \
        if (!helper.##Entry) \
            helper.##Entry = overrideHelper.##Entry

        OverrideEntry(Entry);

        OverrideEntry(AuraOptions);
        OverrideEntry(AuraRestrictions);
        OverrideEntry(CastingRequirements);
        OverrideEntry(Categories);
        OverrideEntry(ClassOptions);
        OverrideEntry(Cooldowns);

        if (overrideHelper.Effects.size() > helper.Effects.size())
            helper.Effects.resize(overrideHelper.Effects.size());
        for (uint8 effIndex = EFFECT_0; effIndex < overrideHelper.Effects.size(); ++effIndex)
            OverrideEntry(Effects[effIndex]);

        OverrideEntry(EquippedItems);
        OverrideEntry(Interrupts);
        OverrideEntry(Levels);
        OverrideEntry(Misc);

        if (overrideHelper.Powers.size() > helper.Powers.size())
            helper.Powers.resize(overrideHelper.Powers.size());
        for (uint8 powerIndex = 0; powerIndex < overrideHelper.Powers.size(); ++powerIndex)
            OverrideEntry(Powers[powerIndex]);

        OverrideEntry(Reagents);
        OverrideEntry(Scaling);
        OverrideEntry(Shapeshift);
        OverrideEntry(TargetRestrictions);
        OverrideEntry(Totems);

        if (helper.Visuals.empty())
            helper.Visuals = overrideHelper.Visuals;

#undef OverrideEntry(Entry)
    };

    for (DifficultyEntry const* difficultyEntry : difficultyEntries)
    {
        auto itr = data.find(difficultyEntry->ID);
        if (itr != data.end())
        {
            DifficultyEntry const* fallbackDifficultyEntry = sDifficultyStore.LookupEntry(difficultyEntry->FallbackDifficultyID);
            while (fallbackDifficultyEntry)
            {
                auto fallbackItr = data.find(fallbackDifficultyEntry->ID);
                if (fallbackItr != data.end())
                    overrideFn(itr->second, fallbackItr->second);

                fallbackDifficultyEntry = sDifficultyStore.LookupEntry(fallbackDifficultyEntry->FallbackDifficultyID);
            }

            auto defaultItr = data.find(DIFFICULTY_NONE);
            if (defaultItr != data.end())
                overrideFn(itr->second, defaultItr->second);
        }
    }
}

void SpellInfoHolder::Load(SpellInfoDifficultyLoadHelper::StorageType&& data, std::function<SpellInfo*(uint32 /*difficulty*/, SpellInfoLoadHelper&&)> loader)
{
    for (auto&& pair : data)
    {
        if (pair.first != DIFFICULTY_NONE)
            _hasDifficultyData = true;
        _data[pair.first] = loader(pair.first, std::move(pair.second));
    }
}

SpellInfo const* SpellInfoHolder::Get(uint32 difficulty) const
{
    if (_data.empty())
        return nullptr;

    if (_hasDifficultyData)
    {
        DifficultyEntry const* difficultyEntry = sDifficultyStore.LookupEntry(difficulty);
        while (difficultyEntry)
        {
            auto itr = _data.find(difficultyEntry->ID);
            if (itr != _data.end())
                return itr->second;

            difficultyEntry = sDifficultyStore.LookupEntry(difficultyEntry->FallbackDifficultyID);
        }
    }

    auto itr = _data.find(DIFFICULTY_NONE);
    if (itr != _data.end())
        return itr->second;

    return nullptr;
}
