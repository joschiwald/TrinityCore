/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "ArenaScore.h"
#include "BattlegroundRV.h"
#include "ObjectAccessor.h"
#include "Language.h"
#include "Player.h"
#include "WorldPacket.h"
#include "GameObject.h"

void BattlegroundRV::InitializeObjects()
{
    ObjectGUIDsByType.resize(BG_RV_OBJECT_MAX);

// elevators
    AddGameObject(BG_RV_OBJECT_ELEVATOR_1, BG_RV_OBJECT_TYPE_ELEVATOR_1, 763.536377f, -294.535767f, 0.505383f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY);
    AddGameObject(BG_RV_OBJECT_ELEVATOR_2, BG_RV_OBJECT_TYPE_ELEVATOR_2, 763.506348f, -273.873352f, 0.505383f, 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY);
    // buffs
    AddGameObject(BG_RV_OBJECT_BUFF_1, BG_RV_OBJECT_TYPE_BUFF_1, 735.551819f, -284.794678f, 28.276682f, 0.034906f, 0, 0, 0, RESPAWN_IMMEDIATELY);
    AddGameObject(BG_RV_OBJECT_BUFF_2, BG_RV_OBJECT_TYPE_BUFF_2, 791.224487f, -284.794464f, 28.276682f, 2.600535f, 0, 0, 0, RESPAWN_IMMEDIATELY);
    // fire
    AddGameObject(BG_RV_OBJECT_FIRE_1, BG_RV_OBJECT_TYPE_FIRE_1, 743.543457f, -283.799469f, 28.286655f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY);
    AddGameObject(BG_RV_OBJECT_FIRE_2, BG_RV_OBJECT_TYPE_FIRE_2, 782.971802f, -283.799469f, 28.286655f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY);
    AddGameObject(BG_RV_OBJECT_FIREDOOR_1, BG_RV_OBJECT_TYPE_FIREDOOR_1, 743.711060f, -284.099609f, 27.542587f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY);
    AddGameObject(BG_RV_OBJECT_FIREDOOR_2, BG_RV_OBJECT_TYPE_FIREDOOR_2, 783.221252f, -284.133362f, 27.535686f, 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY);
    // Gear
    AddGameObject(BG_RV_OBJECT_GEAR_1, BG_RV_OBJECT_TYPE_GEAR_1, 763.664551f, -261.872986f, 26.686588f, 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY);
    AddGameObject(BG_RV_OBJECT_GEAR_2, BG_RV_OBJECT_TYPE_GEAR_2, 763.578979f, -306.146149f, 26.665222f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY);
    // Pulley
    AddGameObject(BG_RV_OBJECT_PULLEY_1, BG_RV_OBJECT_TYPE_PULLEY_1, 700.722290f, -283.990662f, 39.517582f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY);
    AddGameObject(BG_RV_OBJECT_PULLEY_2, BG_RV_OBJECT_TYPE_PULLEY_2, 826.303833f, -283.996429f, 39.517582f, 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY);
    // Pilars
    AddGameObject(BG_RV_OBJECT_PILAR_1, BG_RV_OBJECT_TYPE_PILAR_1, 763.632385f, -306.162384f, 25.909504f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY);
    AddGameObject(BG_RV_OBJECT_PILAR_2, BG_RV_OBJECT_TYPE_PILAR_2, 723.644287f, -284.493256f, 24.648525f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY);
    AddGameObject(BG_RV_OBJECT_PILAR_3, BG_RV_OBJECT_TYPE_PILAR_3, 763.611145f, -261.856750f, 25.909504f, 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY);
    AddGameObject(BG_RV_OBJECT_PILAR_4, BG_RV_OBJECT_TYPE_PILAR_4, 802.211609f, -284.493256f, 24.648525f, 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY);
/*
    // Pilars Collision - Fixme: Use the collision pilars - should make u break LoS
        || !AddObject(BG_RV_OBJECT_PILAR_COLLISION_1, BG_RV_OBJECT_TYPE_PILAR_COLLISION_1, 763.632385f, -306.162384f, 30.639660f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_PILAR_COLLISION_2, BG_RV_OBJECT_TYPE_PILAR_COLLISION_2, 723.644287f, -284.493256f, 32.382710f, 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_PILAR_COLLISION_3, BG_RV_OBJECT_TYPE_PILAR_COLLISION_3, 763.611145f, -261.856750f, 30.639660f, 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_PILAR_COLLISION_4, BG_RV_OBJECT_TYPE_PILAR_COLLISION_4, 802.211609f, -284.493256f, 32.382710f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY)
*/
}

void BattlegroundRV::StartingEventOpenDoors()
{
    // Buff respawn
    SpawnGameObject(BG_RV_OBJECT_BUFF_1, 90);
    SpawnvObject(BG_RV_OBJECT_BUFF_2, 90);
    // Elevators
    DoorOpen(BG_RV_OBJECT_ELEVATOR_1);
    DoorOpen(BG_RV_OBJECT_ELEVATOR_2);

    _state = (BG_RV_STATE_OPEN_FENCES);
    _timer = (BG_RV_FIRST_TIMER);
}

void BattlegroundRV::ProcessInProgress(uint32 const& diff)
{
    BattlegroundMap::ProcessInProgress(diff);

    if (_timer <= diff)
    {
        uint32 i;
        switch (_state)
        {
        case BG_RV_STATE_OPEN_FENCES:
            _timer = BG_RV_PILAR_TO_FIRE_TIMER;
            _state = BG_RV_STATE_CLOSE_FIRE;
            break;
        case BG_RV_STATE_CLOSE_FIRE:
            for (i = BG_RV_OBJECT_FIRE_1; i <= BG_RV_OBJECT_FIREDOOR_2; ++i)
                DoorClose(i);

            _timer = BG_RV_FIRE_TO_PILAR_TIMER;
            _state = BG_RV_STATE_OPEN_PILARS;
            break;
        case BG_RV_STATE_OPEN_PILARS:
            for (i = BG_RV_OBJECT_PILAR_1; i <= BG_RV_OBJECT_PULLEY_2; ++i)
                DoorOpen(i);

            _timer = BG_RV_PILAR_TO_FIRE_TIMER;
            _state = BG_RV_STATE_OPEN_FIRE;
            break;
        case BG_RV_STATE_OPEN_FIRE:
            for (i = BG_RV_OBJECT_FIRE_1; i <= BG_RV_OBJECT_FIREDOOR_2; ++i)
                DoorOpen(i);

            _timer = BG_RV_FIRE_TO_PILAR_TIMER;
            _state = BG_RV_STATE_CLOSE_PILARS;
            break;
        case BG_RV_STATE_CLOSE_PILARS:
            for (i = BG_RV_OBJECT_PILAR_1; i <= BG_RV_OBJECT_PULLEY_2; ++i)
                DoorOpen(i);

            _timer = BG_RV_PILAR_TO_FIRE_TIMER;
            _state = BG_RV_STATE_CLOSE_FIRE;
            break;
        }
    }
    else
        _timer -= diff;
}

void BattlegroundRV::HandleAreaTrigger(Player *Source, uint32 Trigger)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    switch (trigger)
    {
        case 5224:
        case 5226:
        // fire was removed in 3.2.0
        case 5473:
        case 5474:
            break;
        default:
            Battleground::HandleAreaTrigger(player, trigger);
            break;
    }
}

void BattlegroundRV::FillInitialWorldStates(WorldPacket &data)
{
    data << uint32(BG_RV_WORLD_STATE) << uint32(1);
}


void BattlegroundRV::TogglePillarCollision()
{
    bool apply = GetPillarCollision();

    // Toggle visual pillars, pulley, gear, and collision based on previous state
    for (uint8 i = BG_RV_OBJECT_PILAR_1; i <= BG_RV_OBJECT_GEAR_2; ++i)
        apply ? DoorOpen(i) : DoorClose(i);

    for (uint8 i = BG_RV_OBJECT_PILAR_2; i <= BG_RV_OBJECT_PULLEY_2; ++i)
        apply ? DoorClose(i) : DoorOpen(i);

    for (uint8 i = BG_RV_OBJECT_PILAR_1; i <= BG_RV_OBJECT_PILAR_COLLISION_4; ++i)
    {
        if (GameObject* gob = GetBgMap()->GetGameObject(BgObjects[i]))
        {
            if (i >= BG_RV_OBJECT_PILAR_COLLISION_1)
            {
                uint32 _state = GO_STATE_READY;
                if (gob->GetGOInfo()->door.startOpen)
                    _state = GO_STATE_ACTIVE;
                gob->SetGoState(apply ? (GOState)_state : (GOState)(!_state));

                if (gob->GetGOInfo()->door.startOpen)
                    gob->EnableCollision(!apply); // Forced collision toggle
            }

            for (BattlegroundPlayerMap::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
                if (Player* player = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(itr->first, 0, HIGHGUID_PLAYER)))
                    gob->SendUpdateToPlayer(player);
        }
    }

    SetPillarCollision(!apply);
}
