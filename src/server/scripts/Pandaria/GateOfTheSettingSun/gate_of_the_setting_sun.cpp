/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
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
#include "ScriptedCreature.h"
#include "gate_of_the_setting_sun.h"

enum Spells
{
    SPELL_MANTID_MUNITION_EXPLOSION     = 107153,
    SPELL_EXPLOSE_GATE                  = 115456,

    SPELL_BOMB_CAST_VISUAL              = 106729,
    SPELL_BOMB_AURA                     = 106875,
};

struct npc_serpent_spine_defender : public ScriptedAI
{
    npc_serpent_spine_defender(Creature* creature) : ScriptedAI(creature)
    {
    }

    uint32 attackTimer;

    void Reset() override
    {
        attackTimer = urand(1000, 5000);
    }

    void DamageDealt(Unit* /*target*/, uint32& damage, DamageEffectType /*damageType*/) override
    {
        damage = 0;
    }

    void UpdateAI(uint32 diff) override
    {
        if (!me->IsInCombat())
        {
            if (attackTimer <= diff)
            {
                if (Unit* target = me->SelectNearestTarget(5.0f))
                    if (!target->IsFriendlyTo(me))
                        AttackStart(target);
            }
            else
                attackTimer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

struct npc_krikthik_bombarder : public ScriptedAI
{
    npc_krikthik_bombarder(Creature* creature) : ScriptedAI(creature)
    {
        pInstance = creature->GetInstanceScript();
    }

    InstanceScript* pInstance;
    uint32 bombTimer;

    void Reset() override
    {
        me->GetMotionMaster()->MoveRandom(5.0f);
        bombTimer = urand(1000, 7500);
    }

    // Called when spell hits a target
    void SpellHitTarget(Unit* target, SpellInfo const* /*spell*/) override
    {
        if (target->GetEntry() == NPC_BOMB_STALKER)
            me->AddAura(SPELL_BOMB_AURA, target);
    }

    void UpdateAI(uint32 diff) override
    {
        if (bombTimer <= diff)
        {
            if (Unit* stalker = pInstance->GetCreature(pInstance->GetData64(DATA_RANDOM_BOMB_STALKER)))
                if (!stalker->HasAura(SPELL_BOMB_AURA))
                    me->CastSpell(stalker, SPELL_BOMB_CAST_VISUAL, true);

            bombTimer = urand(1000, 5000);
        }
        else bombTimer -= diff;
    }
};

//8359
class AreaTrigger_at_first_door : public AreaTriggerScript
{
    public:
        AreaTrigger_at_first_door() : AreaTriggerScript("at_first_door") {}

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/, bool /*entered*/) override
        {
            if (player->GetInstanceScript())
                player->GetInstanceScript()->SetData(DATA_OPEN_FIRST_DOOR, DONE);

            return false;
        }
};

class go_setting_sun_brasier : public GameObjectScript
{
public:
    go_setting_sun_brasier() : GameObjectScript("go_setting_sun_brasier") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/) override
    {
        if (player->GetInstanceScript())
            player->GetInstanceScript()->SetData(DATA_BRASIER_CLICKED, DONE);

        return false;
    }
};

class go_setting_sun_temp_portal : public GameObjectScript
{
public:
    go_setting_sun_temp_portal() : GameObjectScript("go_setting_sun_temp_portal") { }

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        switch (go->GetEntry())
        {
            case 400001:
                player->NearTeleportTo(1078.96f, 2305.48f, 381.55f, 0.01f);
                break;
            case 400002:
                if (go->GetPositionZ() < 400.0f)
                    player->NearTeleportTo(go->GetPositionX(), go->GetPositionY(), 431.0f, go->GetOrientation());
                else
                    player->NearTeleportTo(go->GetPositionX(), go->GetPositionY(), 388.5f, go->GetOrientation());
                break;
        }

        return false;
    }
};

struct vehicle_artillery_to_wall : public ScriptedAI
{
    vehicle_artillery_to_wall(Creature* creature) : ScriptedAI(creature)
    {
    }

    uint32 launchEventTimer;

    void Reset() override
    {
        launchEventTimer = 0;
    }

    void PassengerBoarded(Unit* /*unit*/, int8 /*seat*/, bool apply) override
    {
        if (apply)
            launchEventTimer = 2500;
    }

    void UpdateAI(uint32 diff) override
    {
        if (!launchEventTimer)
            return;

        if (launchEventTimer <= diff)
        {
            if (me->GetVehicleKit())
            {
                if (Unit* passenger = me->GetVehicleKit()->GetPassenger(0))
                {
                    passenger->ExitVehicle();
                    //passenger->GetMotionMaster()->MoveJump(1100.90f, 2304.58f, 381.23f, 30.0f, 50.0f);
                }
            }

            launchEventTimer = 0;
        }
        else
            launchEventTimer -= diff;
    }
};

void AddSC_gate_setting_sun()
{
    RegisterGateOfTheSettingSunCreatureAI(npc_serpent_spine_defender);
    RegisterGateOfTheSettingSunCreatureAI(npc_krikthik_bombarder);
    new AreaTrigger_at_first_door();
    new go_setting_sun_brasier();
    new go_setting_sun_temp_portal();
    RegisterGateOfTheSettingSunCreatureAI(vehicle_artillery_to_wall);
}