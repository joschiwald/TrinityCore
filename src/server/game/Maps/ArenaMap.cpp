/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
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

#include "ArenaMap.h"
#include "ArenaScore.h"
#include "ArenaTeam.h"
#include "ArenaTeamMgr.h"
#include "ArenaScore.h"
#include "BattlegroundScore.h"
#include "Player.h"
#include "Log.h"

void ArenaMap::InstallBattleground()
{
    for (uint8 i = 0; i < BG_TEAMS_COUNT; ++i)
        _arenaTeamScores[i] = new ArenaTeamScore();
}

void ArenaMap::DestroyBattleground()
{
    for (uint8 i = 0; i < BG_TEAMS_COUNT; ++i)
        delete _arenaTeamScores[i];
}

void ArenaMap::InitializeTextIds()
{
    PreparationPhaseTextIds[BG_STARTING_EVENT_FIRST]  = LANG_ARENA_ONE_MINUTE;
    PreparationPhaseTextIds[BG_STARTING_EVENT_SECOND] = LANG_ARENA_THIRTY_SECONDS;
    PreparationPhaseTextIds[BG_STARTING_EVENT_THIRD]  = LANG_ARENA_FIFTEEN_SECONDS;
    PreparationPhaseTextIds[BG_STARTING_EVENT_FOURTH] = LANG_ARENA_HAS_BEGUN;
}

void ArenaMap::InitializePreparationDelayTimes()
{
    PreparationDelayTimers[BG_STARTING_EVENT_FIRST]  = BG_START_DELAY_1M;
    PreparationDelayTimers[BG_STARTING_EVENT_SECOND] = BG_START_DELAY_30S;
    PreparationDelayTimers[BG_STARTING_EVENT_THIRD]  = BG_START_DELAY_15S;
    PreparationDelayTimers[BG_STARTING_EVENT_FOURTH] = BG_START_DELAY_NONE;
}

void ArenaMap::StartBattleground()
{
    BattlegroundMap::StartBattleground();
    EndTimer = 47 * MINUTE * IN_MILLISECONDS;
    _playersAlive[BG_TEAM_ALLIANCE] = ParticipantCount[BG_TEAM_ALLIANCE];
    _playersAlive[BG_TEAM_HORDE] = ParticipantCount[BG_TEAM_HORDE];
}

void ArenaMap::EndBattleground(uint32 winner)
{
    BattlegroundMap::EndBattleground(winner);

    uint32 loser = 1 - winner;

    ArenaTeam* winnerTeam = _arenaTeams[winner];
    ArenaTeam* loserTeam = _arenaTeams[loser];

    if (_rated && winner != WINNER_NONE)
    {
        uint32 loserTeamRating = loserTeam->GetRating();
        uint32 loserMMR = loserTeam->GetAverageMMR(GetGroupForTeam(loser));
        uint32 winnerTeamRating = winnerTeam->GetRating();
        uint32 winnerMMR = winnerTeam->GetAverageMMR(GetGroupForTeam(winner));

        uint32 winnerChange = winnerTeam->WonAgainst(loserMMR);
        uint32 loserChange = loserTeam->WonAgainst(winnerMMR);
        sLog->outArena("Winner rating: %u, Loser rating: %u, Winner MMR: %u, Loser MMR: %u, Winner change: %d, Loser change: %d",
            winnerTeamRating, loserTeamRating, winnerMMR, loserMMR, winnerChange, loserChange);

        _arenaTeamScores[winner]->Assign(winnerChange, winnerMMR, winnerTeam->GetName());
        _arenaTeamScores[loser]->Assign(loserChange, loserMMR, loserTeam->GetName());
    }

    // TODO: if WINNER_NONE - remove rating for both parties?
}

Group* ArenaMap::GetGroupForTeam(uint32 team) const
{
    for (MapRefManager::iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
        if (Player* player = itr->GetSource())
            if (player->GetBGTeam() == team)
                return player->GetGroup();
}

void ArenaMap::BuildPvPLogDataPacket(WorldPacket& data)
{
    data.Initialize(MSG_PVP_LOG_DATA, 1 + 1 + 4 + 40 * GetPlayerScoresSize());
    data << uint8(1);                           // type (battleground = 0 / arena = 1)

    for (uint8 i = 0; i < BG_TEAMS_COUNT; ++i)
        _arenaTeamScores[i]->BuildRatingInfoBlock(data);

    for (uint8 i = 0; i < BG_TEAMS_COUNT; ++i)
        _arenaTeamScores[i]->BuildTeamInfoBlock(data);

    if (GetStatus() == STATUS_WAIT_LEAVE)
    {
        data << uint8(1);                       // bg ended
        data << uint8(GetWinner());             // who win
    }
    else
        data << uint8(0);                       // bg not ended

    data << uint32(GetPlayerScoresSize());
    for (auto const& score : PlayerScores)
        score.second->AppendToPacket(data);
}

void ArenaMap::OnPlayerJoin(Player* player)
{
    BattlegroundMap::OnPlayerJoin(player);
    PlayerScores[player->GetGUIDLow()] = new ArenaScore(player->GetGUID(), player->GetBGTeam());
    UpdateArenaWorldState();
}

void ArenaMap::OnPlayerExit(Player* player)
{
    BattlegroundMap::OnPlayerExit(player);

    if (_status == STATUS_WAIT_LEAVE)
        return;

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

void ArenaMap::OnPlayerKill(Player* victim, Player* killer)
{
    BattlegroundMap::OnPlayerKill(victim, killer);

    if (_status != STATUS_IN_PROGRESS)
        return;

    --_playersAlive[victim->GetBGTeam()];

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

void ArenaMap::UpdateArenaWorldState()
{
    UpdateWorldState(WORLD_STATE_ARENA_TEAM_H, _playersAlive[TEAM_HORDE]);
    UpdateWorldState(WORLD_STATE_ARENA_TEAM_A, _playersAlive[TEAM_ALLIANCE]);
}

void ArenaMap::CheckArenaWinConditions()
{
    if (!_playersAlive[TEAM_ALLIANCE] && _playersAlive[TEAM_HORDE])
        EndBattleground(TEAM_HORDE);
    else if (!_playersAlive[TEAM_HORDE] && _playersAlive[TEAM_ALLIANCE])
        EndBattleground(TEAM_ALLIANCE);
}
