/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
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

#include "ScriptMgr.h"
#include "OutdoorPvPNA.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "OutdoorPvPMgr.h"
#include "WorldPacket.h"

OPvPCapturePointNA::OPvPCapturePointNA(OutdoorPvP* pvp) : OPvPCapturePoint(pvp),
    _capturable(true),
    _guardsAlive(0),
    _wyvernStateNorth(0),
    _wyvernStateSouth(0),
    _wyvernStateEast(0),
    _wyvernStateWest(0),
    _halaaState(HALAA_N),
    _respawnTimer(NA_RESPAWN_TIME),
    _guardCheckTimer(NA_GUARD_CHECK_TIME)
{
    SetCapturePointData(182210, 530, -1572.57f, 7945.3f, -22.475f, 2.05949f, 0.0f, 0.0f, 0.857167f, 0.515038f);
}

void OPvPCapturePointNA::FillInitialWorldStates(WorldPacket& data)
{
    data << NA_UI_HORDE_GUARDS_SHOW << uint32(m_team == TEAM_HORDE);
    data << NA_UI_ALLIANCE_GUARDS_SHOW << uint32(m_team == TEAM_ALLIANCE);

    data << NA_UI_GUARDS_MAX << NA_GUARDS_MAX;
    data << NA_UI_GUARDS_LEFT << uint32(_guardsAlive);

    data << NA_MAP_WYVERN_NORTH_NEU_H << uint32((_wyvernStateNorth & WYVERN_NEU_HORDE) != 0);
    data << NA_MAP_WYVERN_NORTH_NEU_A << uint32((_wyvernStateNorth & WYVERN_NEU_ALLIANCE) != 0);
    data << NA_MAP_WYVERN_NORTH_H << uint32((_wyvernStateNorth & WYVERN_HORDE) != 0);
    data << NA_MAP_WYVERN_NORTH_A << uint32((_wyvernStateNorth & WYVERN_ALLIANCE) != 0);

    data << NA_MAP_WYVERN_SOUTH_NEU_H << uint32((_wyvernStateSouth & WYVERN_NEU_HORDE) != 0);
    data << NA_MAP_WYVERN_SOUTH_NEU_A << uint32((_wyvernStateSouth & WYVERN_NEU_ALLIANCE) != 0);
    data << NA_MAP_WYVERN_SOUTH_H << uint32((_wyvernStateSouth & WYVERN_HORDE) != 0);
    data << NA_MAP_WYVERN_SOUTH_A << uint32((_wyvernStateSouth & WYVERN_ALLIANCE) != 0);

    data << NA_MAP_WYVERN_WEST_NEU_H << uint32((_wyvernStateWest & WYVERN_NEU_HORDE) != 0);
    data << NA_MAP_WYVERN_WEST_NEU_A << uint32((_wyvernStateWest & WYVERN_NEU_ALLIANCE) != 0);
    data << NA_MAP_WYVERN_WEST_H << uint32((_wyvernStateWest & WYVERN_HORDE) != 0);
    data << NA_MAP_WYVERN_WEST_A << uint32((_wyvernStateWest & WYVERN_ALLIANCE) != 0);

    data << NA_MAP_WYVERN_EAST_NEU_H << uint32((_wyvernStateEast & WYVERN_NEU_HORDE) != 0);
    data << NA_MAP_WYVERN_EAST_NEU_A << uint32((_wyvernStateEast & WYVERN_NEU_ALLIANCE) != 0);
    data << NA_MAP_WYVERN_EAST_H << uint32((_wyvernStateEast & WYVERN_HORDE) != 0);
    data << NA_MAP_WYVERN_EAST_A << uint32((_wyvernStateEast & WYVERN_ALLIANCE) != 0);

    data << NA_MAP_HALAA_NEUTRAL << uint32((_halaaState & HALAA_N) != 0);
    data << NA_MAP_HALAA_NEU_A << uint32((_halaaState & HALAA_N_A) != 0);
    data << NA_MAP_HALAA_NEU_H << uint32((_halaaState & HALAA_N_H) != 0);
    data << NA_MAP_HALAA_HORDE << uint32((_halaaState & HALAA_H) != 0);
    data << NA_MAP_HALAA_ALLIANCE << uint32((_halaaState & HALAA_A) != 0);
}

bool OPvPCapturePointNA::HandleCustomSpell(Player* player, uint32 spellId, GameObject* /*go*/)
{
    uint32 index;

    switch (spellId)
    {
        case NA_SPELL_FLY_NORTH:
            index = NA_ROOST_N;
            break;
        case NA_SPELL_FLY_SOUTH:
            index = NA_ROOST_S;
            break;
        case NA_SPELL_FLY_WEST:
            index = NA_ROOST_W;
            break;
        case NA_SPELL_FLY_EAST:
            index = NA_ROOST_E;
            break;
        default:
            return false;
    }

    player->ActivateTaxiPathTo(FlightPathNodes[index]);
    player->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP);
    player->UpdatePvP(true, true);

    player->AddItem(24538, 10);
    return true;
}

int32 OPvPCapturePointNA::HandleOpenGo(Player* player, ObjectGuid guid)
{
    int32 retval = OPvPCapturePoint::HandleOpenGo(player, guid);
    if (retval >= 0)
    {
        go_type const* gos = nullptr;
        if (m_team == TEAM_ALLIANCE)
            gos = AllianceControlGOs;
        else if (m_team == TEAM_HORDE)
            gos = HordeControlGOs;
        else
            return -1;

        int32 del = -1;
        int32 del2 = -1;
        int32 add = -1;
        int32 add2 = -1;

        switch (retval)
        {
            case NA_DESTROYED_ROOST_S:
                del = NA_DESTROYED_ROOST_S;
                add = NA_ROOST_S;
                add2 = NA_BOMB_WAGON_S;
                if (m_team == TEAM_HORDE)
                    _wyvernStateSouth = WYVERN_ALLIANCE;
                else
                    _wyvernStateSouth = WYVERN_HORDE;
                UpdateWyvernRoostWorldState(NA_ROOST_S);
                break;
            case NA_DESTROYED_ROOST_N:
                del = NA_DESTROYED_ROOST_N;
                add = NA_ROOST_N;
                add2 = NA_BOMB_WAGON_N;
                if (m_team == TEAM_HORDE)
                    _wyvernStateNorth = WYVERN_ALLIANCE;
                else
                    _wyvernStateNorth = WYVERN_HORDE;
                UpdateWyvernRoostWorldState(NA_ROOST_N);
                break;
            case NA_DESTROYED_ROOST_W:
                del = NA_DESTROYED_ROOST_W;
                add = NA_ROOST_W;
                add2 = NA_BOMB_WAGON_W;
                if (m_team == TEAM_HORDE)
                    _wyvernStateWest = WYVERN_ALLIANCE;
                else
                    _wyvernStateWest = WYVERN_HORDE;
                UpdateWyvernRoostWorldState(NA_ROOST_W);
                break;
            case NA_DESTROYED_ROOST_E:
                del = NA_DESTROYED_ROOST_E;
                add = NA_ROOST_E;
                add2 = NA_BOMB_WAGON_E;
                if (m_team == TEAM_HORDE)
                    _wyvernStateEast = WYVERN_ALLIANCE;
                else
                    _wyvernStateEast = WYVERN_HORDE;
                UpdateWyvernRoostWorldState(NA_ROOST_E);
                break;
            case NA_BOMB_WAGON_S:
                del = NA_BOMB_WAGON_S;
                del2 = NA_ROOST_S;
                add = NA_DESTROYED_ROOST_S;
                if (m_team == TEAM_HORDE)
                    _wyvernStateSouth = WYVERN_NEU_ALLIANCE;
                else
                    _wyvernStateSouth = WYVERN_NEU_HORDE;
                UpdateWyvernRoostWorldState(NA_ROOST_S);
                break;
            case NA_BOMB_WAGON_N:
                del = NA_BOMB_WAGON_N;
                del2 = NA_ROOST_N;
                add = NA_DESTROYED_ROOST_N;
                if (m_team == TEAM_HORDE)
                    _wyvernStateNorth = WYVERN_NEU_ALLIANCE;
                else
                    _wyvernStateNorth = WYVERN_NEU_HORDE;
                UpdateWyvernRoostWorldState(NA_ROOST_N);
                break;
            case NA_BOMB_WAGON_W:
                del = NA_BOMB_WAGON_W;
                del2 = NA_ROOST_W;
                add = NA_DESTROYED_ROOST_W;
                if (m_team == TEAM_HORDE)
                    _wyvernStateWest = WYVERN_NEU_ALLIANCE;
                else
                    _wyvernStateWest = WYVERN_NEU_HORDE;
                UpdateWyvernRoostWorldState(NA_ROOST_W);
                break;
            case NA_BOMB_WAGON_E:
                del = NA_BOMB_WAGON_E;
                del2 = NA_ROOST_E;
                add = NA_DESTROYED_ROOST_E;
                if (m_team == TEAM_HORDE)
                    _wyvernStateEast = WYVERN_NEU_ALLIANCE;
                else
                    _wyvernStateEast = WYVERN_NEU_HORDE;
                UpdateWyvernRoostWorldState(NA_ROOST_E);
                break;
            default:
                return -1;
        }

        if (del > -1)
            DelObject(del);

        if (del2 > -1)
            DelObject(del2);

        if (add > -1)
            AddObject(add, gos[add].entry, gos[add].map, gos[add].x, gos[add].y, gos[add].z, gos[add].o, gos[add].rot0, gos[add].rot1, gos[add].rot2, gos[add].rot3);

        if (add2 > -1)
            AddObject(add2, gos[add2].entry, gos[add2].map, gos[add2].x, gos[add2].y, gos[add2].z, gos[add2].o, gos[add2].rot0, gos[add2].rot1, gos[add2].rot2, gos[add2].rot3);
    }

    return retval;
}

bool OPvPCapturePointNA::Update(uint32 diff)
{
    // let the controlling faction advance in phase
    bool capturable = false;
    if (m_team == TEAM_ALLIANCE && m_activePlayers[TEAM_ALLIANCE].size() > m_activePlayers[TEAM_HORDE].size())
        capturable = true;
    else if (m_team == TEAM_HORDE && m_activePlayers[TEAM_ALLIANCE].size() < m_activePlayers[TEAM_HORDE].size())
        capturable = true;

    if (_guardCheckTimer < diff)
    {
        _guardCheckTimer = NA_GUARD_CHECK_TIME;
        uint32 count = GetAliveGuardsCount();
        if (count != _guardsAlive)
        {
            _guardsAlive = count;
            if (!_guardsAlive)
                _capturable = true;
            // update the guard count for the players in zone
            m_PvP->SendUpdateWorldState(NA_UI_GUARDS_LEFT, _guardsAlive);
        }
    }
    else
        _guardCheckTimer -= diff;

    if (_capturable || capturable)
    {
        if (_respawnTimer < diff)
        {
            // if the guards have been killed, then the challenger has one hour to take over halaa.
            // in case they fail to do it, the guards are respawned, and they have to start again.
            ChangeTeam(m_team);
            _respawnTimer = NA_RESPAWN_TIME;
        }
        else
            _respawnTimer -= diff;

        return OPvPCapturePoint::Update(diff);
    }
    return false;
}

void OPvPCapturePointNA::ChangeState()
{
    uint32 artkit = 21;
    switch (m_State)
    {
        case OBJECTIVESTATE_NEUTRAL:
            _halaaState = HALAA_N;
            break;
        case OBJECTIVESTATE_ALLIANCE:
            _halaaState = HALAA_A;
            artkit = 2;
            break;
        case OBJECTIVESTATE_HORDE:
            _halaaState = HALAA_H;
            artkit = 1;
            break;
        case OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE:
            _halaaState = HALAA_N_A;
            break;
        case OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE:
            _halaaState = HALAA_N_H;
            break;
        case OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE:
            _halaaState = HALAA_N_A;
            artkit = 2;
            break;
        case OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE:
            _halaaState = HALAA_N_H;
            artkit = 1;
            break;
    }

    if (GameObject* flag = HashMapHolder<GameObject>::Find(m_capturePointGUID))
        flag->SetGoArtKit(artkit);

    UpdateHalaaWorldState();
}

void OPvPCapturePointNA::ChangeTeam(TeamId oldTeam)
{
    sObjectMgr->RemoveGraveYardLink(NA_HALAA_GRAVEYARD, NA_HALAA_GRAVEYARD_ZONE, oldTeam == TEAM_ALLIANCE ? ALLIANCE : HORDE, false);
    sObjectMgr->AddGraveYardLink(NA_HALAA_GRAVEYARD, NA_HALAA_GRAVEYARD_ZONE, m_team == TEAM_ALLIANCE ? ALLIANCE : HORDE, false);

    DeleteSpawns();
    SpawnGOsForTeam(m_team);
    SpawnNPCsForTeam(m_team);
    _guardsAlive = NA_GUARDS_MAX;
    _capturable = false;
    UpdateHalaaWorldState();

    if (m_team != TEAM_NEUTRAL)
        m_PvP->TeamApplyBuff(m_team, NA_CAPTURE_BUFF);

    if (m_team == TEAM_ALLIANCE)
    {
        _wyvernStateSouth = WYVERN_NEU_HORDE;
        _wyvernStateNorth = WYVERN_NEU_HORDE;
        _wyvernStateEast = WYVERN_NEU_HORDE;
        _wyvernStateWest = WYVERN_NEU_HORDE;
        m_PvP->SendUpdateWorldState(NA_UI_HORDE_GUARDS_SHOW, 0);
        m_PvP->SendUpdateWorldState(NA_UI_ALLIANCE_GUARDS_SHOW, 1);
        m_PvP->SendUpdateWorldState(NA_UI_GUARDS_LEFT, _guardsAlive);
        m_PvP->SendDefenseMessage(NA_HALAA_GRAVEYARD_ZONE, TEXT_HALAA_TAKEN_ALLIANCE);
    }
    else
    {
        _wyvernStateSouth = WYVERN_NEU_ALLIANCE;
        _wyvernStateNorth = WYVERN_NEU_ALLIANCE;
        _wyvernStateEast = WYVERN_NEU_ALLIANCE;
        _wyvernStateWest = WYVERN_NEU_ALLIANCE;
        m_PvP->SendUpdateWorldState(NA_UI_HORDE_GUARDS_SHOW, 1);
        m_PvP->SendUpdateWorldState(NA_UI_ALLIANCE_GUARDS_SHOW, 0);
        m_PvP->SendUpdateWorldState(NA_UI_GUARDS_LEFT, _guardsAlive);
        m_PvP->SendDefenseMessage(NA_HALAA_GRAVEYARD_ZONE, TEXT_HALAA_TAKEN_HORDE);
    }
    UpdateWyvernRoostWorldState(NA_ROOST_S);
    UpdateWyvernRoostWorldState(NA_ROOST_N);
    UpdateWyvernRoostWorldState(NA_ROOST_W);
    UpdateWyvernRoostWorldState(NA_ROOST_E);
}

uint32 OPvPCapturePointNA::GetAliveGuardsCount()
{
    uint32 count = 0;
    for (std::map<uint32, ObjectGuid>::iterator itr = m_Creatures.begin(); itr != m_Creatures.end(); ++itr)
    {
        switch (itr->first)
        {
            case NA_NPC_GUARD_01:
            case NA_NPC_GUARD_02:
            case NA_NPC_GUARD_03:
            case NA_NPC_GUARD_04:
            case NA_NPC_GUARD_05:
            case NA_NPC_GUARD_06:
            case NA_NPC_GUARD_07:
            case NA_NPC_GUARD_08:
            case NA_NPC_GUARD_09:
            case NA_NPC_GUARD_10:
            case NA_NPC_GUARD_11:
            case NA_NPC_GUARD_12:
            case NA_NPC_GUARD_13:
            case NA_NPC_GUARD_14:
            case NA_NPC_GUARD_15:
                if (Creature const* creature = HashMapHolder<Creature>::Find(itr->second))
                    if (creature->IsAlive())
                        ++count;
                break;
            default:
                break;
        }
    }
    return count;
}

void OPvPCapturePointNA::DeleteSpawns()
{
    for (auto i = m_Objects.begin(); i != m_Objects.end(); ++i)
        DelObject(i->first);
    for (auto i = m_Creatures.begin(); i != m_Creatures.end(); ++i)
        DelCreature(i->first);
}

void OPvPCapturePointNA::SpawnNPCsForTeam(TeamId teamId)
{
    creature_type const* creatures = nullptr;
    if (teamId == TEAM_ALLIANCE)
        creatures = AllianceControlNPCs;
    else if (teamId == TEAM_HORDE)
        creatures = HordeControlNPCs;
    else
        return;

    for (uint8 i = 0; i < NA_CONTROL_NPC_NUM; ++i)
        AddCreature(i, creatures[i].entry, creatures[i].map, creatures[i].x, creatures[i].y, creatures[i].z, creatures[i].o, teamId, 1000000);
}

void OPvPCapturePointNA::SpawnGOsForTeam(TeamId teamId)
{
    go_type const* gos = nullptr;
    if (teamId == TEAM_ALLIANCE)
        gos = AllianceControlGOs;
    else if (teamId == TEAM_HORDE)
        gos = HordeControlGOs;
    else
        return;

    for (uint8 i = 0; i < NA_CONTROL_GO_NUM; ++i)
    {
        if (i == NA_ROOST_S ||
            i == NA_ROOST_W ||
            i == NA_ROOST_N ||
            i == NA_ROOST_E ||
            i == NA_BOMB_WAGON_S ||
            i == NA_BOMB_WAGON_W ||
            i == NA_BOMB_WAGON_N ||
            i == NA_BOMB_WAGON_E)
            continue;   // roosts and bomb wagons are spawned when someone uses the matching destroyed roost
        AddObject(i, gos[i].entry, gos[i].map, gos[i].x, gos[i].y, gos[i].z, gos[i].o, gos[i].rot0, gos[i].rot1, gos[i].rot2, gos[i].rot3);
    }
}

void OPvPCapturePointNA::UpdateHalaaWorldState()
{
    m_PvP->SendUpdateWorldState(NA_MAP_HALAA_NEUTRAL, uint32((_halaaState & HALAA_N) != 0));
    m_PvP->SendUpdateWorldState(NA_MAP_HALAA_NEU_A, uint32((_halaaState & HALAA_N_A) != 0));
    m_PvP->SendUpdateWorldState(NA_MAP_HALAA_NEU_H, uint32((_halaaState & HALAA_N_H) != 0));
    m_PvP->SendUpdateWorldState(NA_MAP_HALAA_HORDE, uint32((_halaaState & HALAA_H) != 0));
    m_PvP->SendUpdateWorldState(NA_MAP_HALAA_ALLIANCE, uint32((_halaaState & HALAA_A) != 0));
}

void OPvPCapturePointNA::UpdateWyvernRoostWorldState(uint32 roost)
{
    switch (roost)
    {
        case NA_ROOST_S:
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_SOUTH_NEU_H, uint32((_wyvernStateSouth & WYVERN_NEU_HORDE) != 0));
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_SOUTH_NEU_A, uint32((_wyvernStateSouth & WYVERN_NEU_ALLIANCE) != 0));
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_SOUTH_H, uint32((_wyvernStateSouth & WYVERN_HORDE) != 0));
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_SOUTH_A, uint32((_wyvernStateSouth & WYVERN_ALLIANCE) != 0));
            break;
        case NA_ROOST_N:
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_NORTH_NEU_H, uint32((_wyvernStateNorth & WYVERN_NEU_HORDE) != 0));
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_NORTH_NEU_A, uint32((_wyvernStateNorth & WYVERN_NEU_ALLIANCE) != 0));
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_NORTH_H, uint32((_wyvernStateNorth & WYVERN_HORDE) != 0));
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_NORTH_A, uint32((_wyvernStateNorth & WYVERN_ALLIANCE) != 0));
            break;
        case NA_ROOST_W:
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_WEST_NEU_H, uint32((_wyvernStateWest & WYVERN_NEU_HORDE) != 0));
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_WEST_NEU_A, uint32((_wyvernStateWest & WYVERN_NEU_ALLIANCE) != 0));
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_WEST_H, uint32((_wyvernStateWest & WYVERN_HORDE) != 0));
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_WEST_A, uint32((_wyvernStateWest & WYVERN_ALLIANCE) != 0));
            break;
        case NA_ROOST_E:
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_EAST_NEU_H, uint32((_wyvernStateEast & WYVERN_NEU_HORDE) != 0));
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_EAST_NEU_A, uint32((_wyvernStateEast & WYVERN_NEU_ALLIANCE) != 0));
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_EAST_H, uint32((_wyvernStateEast & WYVERN_HORDE) != 0));
            m_PvP->SendUpdateWorldState(NA_MAP_WYVERN_EAST_A, uint32((_wyvernStateEast & WYVERN_ALLIANCE) != 0));
            break;
        default:
            break;
    }
}

OutdoorPvPNA::OutdoorPvPNA()
{
    m_TypeId = OUTDOOR_PVP_NA;
    _capturePoint = nullptr;
}

bool OutdoorPvPNA::SetupOutdoorPvP()
{
    // add the zones affected by the pvp buff
    RegisterZone(NA_BUFF_ZONE);

    // halaa
    _capturePoint = new OPvPCapturePointNA(this);

    AddCapturePoint(_capturePoint);
    return true;
}

void OutdoorPvPNA::HandlePlayerEnterZone(Player* player, uint32 zone)
{
    // add buffs
    if (player->GetTeamId() == _capturePoint->GetTeamId())
        player->CastSpell(player, NA_CAPTURE_BUFF, true);
    OutdoorPvP::HandlePlayerEnterZone(player, zone);
}

void OutdoorPvPNA::HandlePlayerLeaveZone(Player* player, uint32 zone)
{
    // remove buffs
    player->RemoveAurasDueToSpell(NA_CAPTURE_BUFF);
    OutdoorPvP::HandlePlayerLeaveZone(player, zone);
}

bool OutdoorPvPNA::Update(uint32 diff)
{
    return _capturePoint->Update(diff);
}

void OutdoorPvPNA::FillInitialWorldStates(WorldPacket &data)
{
    _capturePoint->FillInitialWorldStates(data);
}

void OutdoorPvPNA::SendRemoveWorldStates(Player* player)
{
    player->SendUpdateWorldState(NA_UI_HORDE_GUARDS_SHOW, 0);
    player->SendUpdateWorldState(NA_UI_ALLIANCE_GUARDS_SHOW, 0);
    player->SendUpdateWorldState(NA_UI_GUARDS_MAX, 0);
    player->SendUpdateWorldState(NA_UI_GUARDS_LEFT, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_NORTH_NEU_H, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_NORTH_NEU_A, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_NORTH_H, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_NORTH_A, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_SOUTH_NEU_H, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_SOUTH_NEU_A, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_SOUTH_H, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_SOUTH_A, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_WEST_NEU_H, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_WEST_NEU_A, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_WEST_H, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_WEST_A, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_EAST_NEU_H, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_EAST_NEU_A, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_EAST_H, 0);
    player->SendUpdateWorldState(NA_MAP_WYVERN_EAST_A, 0);
    player->SendUpdateWorldState(NA_MAP_HALAA_NEUTRAL, 0);
    player->SendUpdateWorldState(NA_MAP_HALAA_NEU_A, 0);
    player->SendUpdateWorldState(NA_MAP_HALAA_NEU_H, 0);
    player->SendUpdateWorldState(NA_MAP_HALAA_HORDE, 0);
    player->SendUpdateWorldState(NA_MAP_HALAA_ALLIANCE, 0);
}

void OutdoorPvPNA::HandleKillImpl(Player* player, Unit* killed)
{
    if (killed->GetTypeId() == TYPEID_PLAYER && player->GetTeam() != killed->ToPlayer()->GetTeam())
    {
        player->KilledMonsterCredit(NA_CREDIT_MARKER);
        if (player->GetTeamId() == TEAM_ALLIANCE)
            player->CastSpell(player, NA_KILL_TOKEN_ALLIANCE, true);
        else
            player->CastSpell(player, NA_KILL_TOKEN_HORDE, true);
    }
}

class OutdoorPvP_nagrand : public OutdoorPvPScript
{
    public:
        OutdoorPvP_nagrand() : OutdoorPvPScript("outdoorpvp_na") { }

        OutdoorPvP* GetOutdoorPvP() const override
        {
            return new OutdoorPvPNA();
        }
};

void AddSC_outdoorpvp_na()
{
    new OutdoorPvP_nagrand();
}
