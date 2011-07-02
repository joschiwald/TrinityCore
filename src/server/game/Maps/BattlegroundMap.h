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

#ifndef TRINITY_BATTLEGROUND_MAP_H
#define TRINITY_BATTLEGROUND_MAP_H

#include "BattlegroundTemplate.h"

enum BattlegroundStartTimeIntervals
{
    BG_START_DELAY_2M               = 120000,               // ms (2 minutes)
    BG_START_DELAY_1M               = 60000,                // ms (1 minute)
    BG_START_DELAY_30S              = 30000,                // ms (30 seconds)
    BG_START_DELAY_15S              = 15000,                // ms (15 seconds) Used only in arena
    BG_START_DELAY_NONE             = 0,                    // ms
};

enum BattlegroundStartingEventsIds
{
    BG_STARTING_EVENT_FIRST,
    BG_STARTING_EVENT_SECOND,
    BG_STARTING_EVENT_THIRD,
    BG_STARTING_EVENT_FOURTH,
    BG_STARTING_EVENT_COUNT
};

enum BattlegroundStatus
{
    STATUS_NONE,                                // first status, should mean bg is not instance
    STATUS_WAIT_QUEUE,                          // means bg is empty and waiting for queue
    STATUS_WAIT_JOIN,                           // this means, that BG has already started and it is waiting for more players
    STATUS_IN_PROGRESS,                         // means bg is running
    STATUS_WAIT_LEAVE                           // means some faction has won BG and it is ending
};

enum BattlegroundWinner
{
    WINNER_HORDE            = 0,
    WINNER_ALLIANCE         = 1,
    WINNER_NONE             = 2
};

enum BattlegroundTimeIntervals
{
    RESURRECTION_INTERVAL           = 30000,                // ms
    INVITATION_REMIND_TIME          = 20000,                // ms
    INVITE_ACCEPT_WAIT_TIME         = 40000,                // ms
    TIME_TO_AUTOREMOVE              = 120000,               // ms
    MAX_OFFLINE_TIME                = 300,                  // secs
    RESPAWN_ONE_DAY                 = 86400,                // secs
    RESPAWN_IMMEDIATELY             = 0,                    // secs
    BUFF_RESPAWN_TIME               = 180,                  // secs
};

/*
// handle the queue types and bg types separately to enable joining queue for different sized arenas at the same time
enum BattlegroundQueueTypeId
{
    BATTLEGROUND_QUEUE_NONE     = 0,
    BATTLEGROUND_QUEUE_AV       = 1,
    BATTLEGROUND_QUEUE_WS       = 2,
    BATTLEGROUND_QUEUE_AB       = 3,
    BATTLEGROUND_QUEUE_EY       = 4,
    BATTLEGROUND_QUEUE_SA       = 5,
    BATTLEGROUND_QUEUE_IC       = 6,
    BATTLEGROUND_QUEUE_RB       = 7,
    BATTLEGROUND_QUEUE_2v2      = 8,
    BATTLEGROUND_QUEUE_3v3      = 9,
    BATTLEGROUND_QUEUE_5v5      = 10,
    MAX_BATTLEGROUND_QUEUE_TYPES
};
*/

struct BattlegroundScore;

class BattlegroundMap : public Map
{
    friend class WorldSession;
    friend class MapManager;

    public:
    protected:
        // Typedefs here
        typedef std::map<uint32, BattlegroundScore*> BattlegroundScoreMap;

    protected:
        uint32 GetMaxPlayers() const { return _template.MaxPlayersPerTeam * 2; }
        uint32 GetMinPlayers() const { return _template.MinPlayersPerTeam * 2; }
        uint32 GetMinLevel() const { return _template.MinLevel; }
        uint32 GetMaxLevel() const { return _template.MaxLevel; }

        uint32 GetStatus() const { return _status; }

    protected:
        /* Methods called by upper level code */
        BattlegroundMap(uint32 id, time_t expiry, uint32 instanceId, Map* parent, uint8 spawnMode);
        ~BattlegroundMap();

        bool AddPlayerToMap(Player* player) override;
        void RemovePlayerFromMap(Player*, bool) override;
        void Update(uint32 const diff) override;

        bool CanEnter(Player* player);
        void SetUnload();

        // Packet builders
        virtual void BuildPvPLogDataPacket(WorldPacket& data);

        /* Methods and attributes accessed by subclasses */
        // Initialization
        virtual void InitializeObjects() { }   // Resize ObjectGUIDsByType and spawn objects
        virtual void InitializeTextIds() { }   // Initializes text IDs that are used in the battleground at any possible phase.
        virtual void InitializePreparationDelayTimes(); // Initializes preparation delay timers.
        virtual void FillInitialWorldStates(WorldPacket& data) { }

        virtual void InstallBattleground() { }  // Calls all overridable InitializeXX() methods
        virtual void StartBattleground() { }    // Initializes EndTimer and other bg-specific variables.
        virtual uint32 GetWinningTeam() const { return WINNER_NONE; }  // Contains rules on which team to pick as winner
        virtual void EndBattleground(uint32 winner) { }  // Handles out rewards etc
        virtual void DestroyBattleground() { }  // Contains battleground specific cleanup method calls.

        virtual bool UpdatePlayerScore(Player* player, uint32 type, uint32 value, bool addHonor = true);
        void UpdateWorldState(uint32 type, uint32 value);

        // Entity management - GameObject
        GameObject* AddGameObject(uint32 type, uint32 entry, float x, float y, float z, float o, float r0, float r1, float r2, float r3, uint32 respawnTime = 0);   // Adds GO's to the map but doesn't necessarily spawn them
        GameObject* AddGameObject(uint32 type, uint32 entry, Position* pos, float r0, float r1, float r2, float r3, uint32 respawnTime = 0);   // Adds GO's to the map but doesn't necessarily spawn them
        GameObject* GetGameObject(uint32 type);
        void SpawnGameObject(uint32 type, uint32 respawntime);  // Spawns an already added gameobject
        bool DeleteGameObject(uint32 type); // Deletes an object with specified type designation 

        // Entity management - Creature
        Creature* AddCreature(uint32 entry, uint32 type, uint32 teamval, float x, float y, float z, float o, uint32 respawntime = 0); // Adds and spawns creatures to map
        Creature* AddCreature(uint32 entry, uint32 type, uint32 teamval, Position* pos, uint32 respawntime = 0); // Adds and spawns creatures to map
        Creature* GetCreature(uint32 type);
        bool DeleteCreature(uint32 type);

        std::vector<uint64> ObjectGUIDsByType;      // Stores object GUIDs per enum-defined arbitrary type

        // Hooks called after Map methods
        virtual void OnPlayerJoin(Player* player);  // Initialize battleground specific variables.
        virtual void OnPlayerExit(Player* player);  // Remove battleground specific auras etc.

        // Misc. hooks
        virtual void OnPlayerKill(Player* victim, Player* killer);

        uint32 EndTimer;        // Battleground specific time limit. Must be overwritten in subclass.
        uint32 PreparationPhaseTextIds[BG_STARTING_EVENT_COUNT]; // Must be initialized for each battleground
        uint32 PreparationDelayTimers[BG_STARTING_EVENT_COUNT];

        BattlegroundScoreMap PlayerScores;                  // Player scores
        int32 TeamScores[BG_TEAMS_COUNT];                   // Team scores - can this even be negative?

    private:
        // Private initializers, non overridable
        void InitVisibilityDistance() final;
        void InitializePreparationDelayTimer();

        // Private processing methods
        void ProcessPreparation(uint32 diff);
        void ProcessInProgress(uint32 diff);
        void ProcessEnded(uint32 diff);

        void RemoveAllPlayers();

        // Packet senders
        void SendPacketToAll(WorldPacket* data);
        void SendPacketToTeam(WorldPacket* data, uint32 team, Player* exclude);
        void SendMessageToAll(int32 entry, ChatMsg type);
        void SendPlayerJoinedPacket(Player* player);
        void SendPlayerLeftPacket(Player* player);

        bool AreTeamsInBalance() const;

        BattlegroundTemplate _template;
        BattlegroundStatus _status;

        uint8 _preparationPhase;
        uint32 _preparationTimer;   // Time between map creation and start of the battleground
        uint32 _prematureCountdownTimer; // If teams are imbalanced the battleground will close after a certain delay
        uint32 _postEndTimer;       // Time between battleground ending and closing of the map

        uint16 _participantCount[BG_TEAMS_COUNT];   // Players actually in the battleground
        uint16 _invitedCount[BG_TEAMS_COUNT];       // Players invited to join the battleground
};

#endif // TRINITY_BATTLEGROUND_MAP_H
