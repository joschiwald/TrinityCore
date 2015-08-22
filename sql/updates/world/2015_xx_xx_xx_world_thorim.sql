/*
 * Thorim
 */

-- todo: immunity runic colossus, rune giant grip

DELETE FROM `creature_text` WHERE `entry` IN (32865,32872,32873,33196);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextID`, `comment`) VALUES
-- Thorim
(32865, 0,0,'Interlopers! You mortals who dare to interfere with my sport will pay... Wait--you...',14,0,100,0,0,15733,33145,'Thorim SAY_AGGRO_1'),
(32865, 1,0,'I remember you... In the mountains... But you... what is this? Where am--',14,0,100,0,0,15734,33270,'Thorim SAY_AGGRO_2'),
(32865, 2,0,'Behold the power of the storms and despair!',14,0,100,0,0,15735,0,'Thorim SAY_SPECIAL_1'),
(32865, 3,0,'Do not hold back! Destroy them!',14,0,100,0,0,15736,34241,'Thorim SAY_SPECIAL_2'),
(32865, 4,0,'Have you begun to regret your intrusion?',14,0,100,0,0,15737,0,'Thorim SAY_SPECIAL_3'),
(32865, 5,0,'Impertinent whelps, you dare challenge me atop my pedestal? I will crush you myself!',14,0,100,0,0,15738,33148,'Thorim SAY_JUMPDOWN'),
(32865, 6,0,'Can''t you at least put up a fight!?',14,0,100,0,0,15739,34239,'Thorim SAY_SLAY_1'),
(32865, 6,1,'Pathetic.',14,0,100,0,0,15740,35768,'Thorim SAY_SLAY_2'),
(32865, 7,0,'My patience has reached its limit!',14,0,100,0,0,15741,33365,'Thorim SAY_BERSERK'),
(32865, 8,0,'Failures! Weaklings!',14,0,100,0,0,15742,33274,'Thorim SAY_WIPE'),
(32865, 9,0,'Stay your arms! I yield!',14,0,100,0,0,15743,33948,'Thorim SAY_DEATH'),
(32865,10,0,'I feel as though I am awakening from a nightmare, but the shadows in this place yet linger.',14,0,100,0,0,15744,33949,'Thorim SAY_END_NORMAL_1'),
(32865,11,0,'Sif... was Sif here? Impossible--she died by my brother''s hand. A dark nightmare indeed....',14,0,100,0,0,15745,33950,'Thorim SAY_END_NORMAL_2'),
(32865,12,0,'I need time to reflect.... I will aid your cause if you should require it. I owe you at least that much. Farewell.',14,0,100,0,0,15746,33951,'Thorim SAY_END_NORMAL_3'),
(32865,13,0,'You! Fiend! You are not my beloved! Be gone!',14,0,100,0,0,15747,33952,'Thorim SAY_END_HARD_1'),
(32865,14,0,'Behold the hand behind all the evil that has befallen Ulduar, left my kingdom in ruins, corrupted my brother, and slain my wife.',14,0,100,0,0,15748,33953,'Thorim SAY_END_HARD_2'),
(32865,15,0,'And now it falls to you, champions, to avenge us all. The task before you is great, but I will lend you my aid as I am able. You must prevail.',14,0,100,0,0,15749,33954,'Thorim SAY_END_HARD_3'),
-- Runic Colossus
(32872, 0,0,'%s surrounds itself with a crackling Runic Barrier!',41,0,100,0,0,0,33267,'Runic Colossus'),
-- Ancient Rune Giant
(32873, 0,0,'%s fortifies nearby allies with runic might!',41,0,100,0,0,0,33523,'Ancient Rune Giant'),
-- Sif
(33196, 0,0,'Thorim, my lord!  Why else would these invaders have come into your sanctum but to slay you?  They must be stopped!',14,0,100,0,0,15668,33325,'Sif - Start'),
(33196, 1,0,'These pathetic mortals are harmless, beneath my station.  Dispose of them!',14,0,100,0,0,15669,33368,'Sif - Despawn'),
(33196, 2,0,'Impossible!  Lord Thorim, I will bring your foes a frigid death!',14,0,100,0,0,15670,33369,'Sif - Event');

UPDATE `gameobject_template` SET `AIName`='SmartGameObjectAI' WHERE `entry`=194264;
DELETE FROM `smart_scripts` WHERE `entryorguid`=194264 AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(194264,1,0,0,70,0,100,0,2,0,0,0,9,0,0,0,0,0,0,14,34155,0,0,0,0,0,0, 'Lever - On activate - Activate Dark Iron Portcullis');

-- Thorim Trap Bunny SAI
SET @ENTRY := 33054;
UPDATE `creature_template` SET `AIName`='SmartAI', `flags_extra`=2 WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,10,0,100,0,0,12,17000,17000,11,62241,0,0,0,0,0,1,0,0,0,0,0,0,0,'Thorim Trap Bunny - Within 0-12 Range Out of Combat LoS - Cast Paralytic Field');

SET @ENTRY := 33725;
UPDATE `creature_template` SET `AIName`='SmartAI', `flags_extra`=2 WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,10,0,100,0,0,12,17000,17000,11,63540,0,0,0,0,0,1,0,0,0,0,0,0,0,'Thorim Trap Bunny - Within 0-12 Range Out of Combat LoS - Cast Paralytic Field');


DELETE FROM `spell_script_names` WHERE `spell_id` IN (62577,62603,62576,62602,62580,62604,62016,62057,62058,62042,62184,62466,64767,64909,62184,62331,62418,61934);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(62577,'spell_thorim_blizzard'),
(62603,'spell_thorim_blizzard'),
(62576,'spell_thorim_blizzard_effect'),
(62602,'spell_thorim_blizzard_effect'),
(62580,'spell_thorim_frostbolt_volley'),
(62604,'spell_thorim_frostbolt_volley'),
(62016,'spell_thorim_charge_orb'),
(62057,'spell_thorim_runic_smash'),
(62058,'spell_thorim_runic_smash'),
(62042,'spell_thorim_stormhammer'),
(62466,'spell_thorim_lightning_charge'),
(64767,'spell_thorim_stormhammer_sif'),
(64909,'spell_thorim_stormhammer_boomerang'),
(62184,'spell_thorim_activate_lightning_orb_periodic'),
(62331,'spell_iron_ring_guard_impale'),
(62418,'spell_iron_ring_guard_impale'),
(61934,'spell_thorim_arena_leap');

DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=62042;
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = -62320;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(-62320, -62398, 0, 'Aura of Celerity - Remove Visual');

-- Add dummy effect to kill credit so it actually selects targets
UPDATE `spell_dbc` SET `EffectImplicitTargetA1`=18, `EffectImplicitTargetB1`=16, `EffectRadiusIndex1`=28, `SchoolMask`=0, `Effect1`=3 WHERE `Id`=64980;

DELETE FROM `spelldifficulty_dbc` WHERE `id` IN (62131,62315,62316,62333,62334,62335,62327,62328,62321,62331,62332,62613,62583,62577,62597,62580);
INSERT INTO `spelldifficulty_dbc` (`id`, `spellid0`, `spellid1`) VALUES
(62131,62131,64390), -- Chain Lightning
(62315,62315,62415), -- Acid Breath
(62316,62316,62417), -- Sweep
(62333,62333,62441), -- Renew
(62334,62334,62442), -- Greater Heal
(62335,62335,62443), -- Holy Smite
(62327,62327,62445), -- Runic Lightning
(62328,62328,62446), -- Runic Mending
(62321,62321,62529), -- Runic Shield
(62331,62331,62418), -- Impale
(62332,62332,62420), -- Shield Smash
(62613,62613,62614), -- Charge
(62583,62583,62601), -- Frostbolt
(62577,62577,62603), -- Blizzard
(62597,62597,62605), -- Frost Nova
(62580,62580,62604); -- Frostbolt Volley

UPDATE `creature_template` SET `ScriptName`='boss_thorim' WHERE `entry`=32865;
UPDATE `creature_template` SET `ScriptName`='npc_sif' WHERE `entry`=33196;
UPDATE `creature_template` SET `ScriptName`='npc_thorim_pre_phase' WHERE `entry` IN (32885,32883,32908,32907,32882,32886);
UPDATE `creature_template` SET `ScriptName`='npc_thorim_arena_phase' WHERE `entry` IN (32876,32904,32878,32877,32874,32875,33110);
UPDATE `creature_template` SET `ScriptName`='npc_runic_colossus' WHERE `entry`=32872;
UPDATE `creature_template` SET `ScriptName`='npc_ancient_rune_giant' WHERE `entry`=32873;
UPDATE `creature_template` SET `difficulty_entry_1`=33150 WHERE `entry`=32908; -- Swapped Difficulty entry npcs
UPDATE `creature_template` SET `difficulty_entry_1`=33151 WHERE `entry`=32907; -- Caused swapped displayIDs in 25n
UPDATE `creature_template` SET `InhabitType`=4, `flags_extra`=128, `ScriptName`='' WHERE `entry` IN (33140,33141);
UPDATE `creature_template` SET `InhabitType`=4, `flags_extra`=128 WHERE `entry` IN (33378,32879);
UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry` IN (32892);
UPDATE `creature_template` SET `faction`=1692 WHERE `entry` IN (32885,32883,33152,33153,32908,33150,32907,33151);
UPDATE `creature_template` SET `faction`=1693 WHERE `entry` IN (32882,33154);
UPDATE `creature_template` SET `speed_walk`=2.5/2.5, `speed_run`=14.0/7.0, `BaseAttackTime`=1500, `RangeAttackTime`=1500 WHERE `entry` IN (32865,33147);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry` IN (62577,62603,62016,62976,63238,64098,62466,62565,62942,64767,62560,61964,61934);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13,1,62577,0,0,31,0,3,32892,0,0,0,'','Thorim - Blizzard'),
(13,1,62603,0,0,31,0,3,32892,0,0,0,'','Thorim - Blizzard'),
(13,1,62016,0,0,31,0,3,33378,0,0,0,'','Thorim - Charge Orb'),
(13,1,62976,0,0,31,0,3,33378,0,0,0,'','Thorim - Lightning Pillar'),
(13,1,62976,0,0,33,0,1,0,0,1,0,'','Thorim - Lightning Pillar'),
(13,1,63238,0,0,31,0,3,33378,0,0,0,'','Thorim - Lightning Pillar'),
(13,1,63238,0,0,33,0,1,0,0,1,0,'','Thorim - Lightning Pillar'),
(13,1,64098,0,0,31,0,3,32865,0,0,0,'','Thorim - Lightning Bolt'),
(13,4,62466,0,0,31,0,3,32780,0,0,0,'','Thorim - Lightning Charge'),
(13,3,62565,0,0,31,0,3,32865,0,0,0,'','Thorim - Touch of Dominion'),
(13,7,62942,0,0,31,0,3,32874,0,0,0,'','Thorim - Runic Fortification'),
(13,7,62942,0,1,31,0,3,32875,0,0,0,'','Thorim - Runic Fortification'),
(13,7,62942,0,2,31,0,3,33110,0,0,0,'','Thorim - Runic Fortification'),
(13,1,64767,0,0,31,0,3,33196,0,0,0,'','Thorim - Stormhammer'),
(13,7,62560,0,0,31,0,3,32876,0,0,0,'','Thorim - Berserk'),
(13,7,62560,0,1,31,0,3,32877,0,0,0,'','Thorim - Berserk'),
(13,7,62560,0,2,31,0,3,32878,0,0,0,'','Thorim - Berserk'),
(13,7,62560,0,3,31,0,3,32904,0,0,0,'','Thorim - Berserk'),
(13,1,61964,0,0,31,0,3,32882,0,0,0,'','Thorim - Circle of Healing'),
(13,1,61964,0,1,31,0,3,32883,0,0,0,'','Thorim - Circle of Healing'),
(13,1,61964,0,2,31,0,3,32885,0,0,0,'','Thorim - Circle of Healing'),
(13,1,61964,0,3,31,0,3,32886,0,0,0,'','Thorim - Circle of Healing'),
(13,1,61964,0,4,31,0,3,32907,0,0,0,'','Thorim - Circle of Healing'),
(13,1,61964,0,5,31,0,3,32908,0,0,0,'','Thorim - Circle of Healing'),
(13,1,61934,0,0,31,0,3,32892,0,0,0,'condition_thorim_arena_leap','Thorim - Leap');

DELETE FROM `disables` WHERE `sourceType`=4 AND `entry` IN (10289,10314,10304,10313,10440,10457,10800,10801);
INSERT INTO `disables` (`sourceType`,`entry`,`flags`,`params_0`,`params_1`,`comment`) VALUES
(4,10800,0,0,0,'Siffed (10 player) - Duplicate'),
(4,10801,0,0,0,'Siffed (25 player) - Duplicate');

DELETE FROM `disables` WHERE `sourceType`=0 AND `entry` IN (62042,64767,45537);
INSERT INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES
(0, 62042, 64, '', '', 'Stormhammer - Ignore LOS'),
(0, 64767, 64, '', '', 'Stormhammer - Ignore LOS'),
(0, 45537, 64, '', '', 'Lightning Beam Channel - Ignore LOS');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10289,10314,10305,10309,10440,10457);
INSERT INTO `achievement_criteria_data` (`criteria_id`,`type`,`value1`,`value2`,`ScriptName`) VALUES
(10289,12,0,0,''), -- Siffed (10 player)
(10314,12,1,0,''), -- Siffed (25 player)

(10305,11,0,0,'achievement_dont_stand_in_the_lightning'), -- Don't Stand in the Lightning (10 player)
(10305,12,0,0,''),                                        -- Don't Stand in the Lightning (10 player)
(10309,11,0,0,'achievement_dont_stand_in_the_lightning'), -- Don't Stand in the Lightning (25 player)
(10309,12,1,0,''),                                        -- Don't Stand in the Lightning (25 player)

(10440,11,0,0,'achievement_lose_your_illusion'), -- Lose Your Illusion (10 player)
(10440,12,0,0,''),                               -- Lose Your Illusion (10 player)
(10457,11,0,0,'achievement_lose_your_illusion'), -- Lose Your Illusion (25 player)
(10457,12,1,0,'');                               -- Lose Your Illusion (25 player)

UPDATE `gameobject_template` SET `faction`=94, `flags`=16, `size`=3 WHERE `entry` IN (194312,194313,194314,194315);

DELETE FROM `creature` WHERE `id` IN (32882,32908,32907,32885,32883,32886); -- PreAdds
DELETE FROM `creature` WHERE `id` IN (32874,32875,33110); -- Colossus, Giant Adds

-- Fix Thorim Controller multi spawns
SET @CGUID:=136446;
DELETE FROM `creature` WHERE `id`=32879;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@CGUID, 32879, 603, 3, 1, 2134.774, -262.3073, 428.6936, 1.343904, 7200, 0, 0); -- 32879 (Area: 0) (Auras: 62184 - 62184)

DELETE FROM `gameobject_loot_template` WHERE `Entry` IN (26955,26956);
INSERT INTO `gameobject_loot_template` (`Entry`,`Item`,`Reference`,`Chance`,`QuestRequired`,`LootMode`,`GroupId`,`MinCount`,`MaxCount`,`Comment`) VALUES
(26955,1,34372,100,0,1,0,1,1,'Thorim25 HM - normal mode loot'),
(26955,2,12033,100,0,1,0,1,2,'Thorim25 HM - T8.5 tokens'),
(26955,3,34154,10,0,1,0,1,1,'Thorim25 HM - Random Ulduar craft recipe'),
(26955,45038,0,18,0,1,0,1,1,'Thorim25 HM - Val''anyr fragment'),
(26955,45087,0,10,0,1,0,1,1,'Thorim25 HM - Runed Orb'),
(26955,45470,0,0,0,1,1,1,1,'Thorim25 HM - Wisdom''s Hold'),
(26955,45471,0,0,0,1,1,1,1,'Thorim25 HM - Fate''s Clutch'),
(26955,45472,0,0,0,1,1,1,1,'Thorim25 HM - Warhelm of the Champion'),
(26955,45473,0,0,0,1,1,1,1,'Thorim25 HM - Embrace of the Gladiator'),
(26955,45474,0,0,0,1,1,1,1,'Thorim25 HM - Pauldrons of the Combatant'),
(26955,45570,0,0,0,1,1,1,1,'Thorim25 HM - Skyforge Crossbow'),
(26955,45817,0,100,1,1,0,1,1,'Thorim25 HM - Thorim''s Sigil'),
(26955,47241,0,100,0,1,0,1,1,'Thorim25 HM - Emblem of Triumph'),

(26956,1,34372,100,0,1,0,1,1,'Thorim25 - normal mode loot'),
(26956,2,12033,100,0,1,0,1,2,'Thorim25 - T8.5 tokens'),
(26956,3,34154,10,0,1,0,1,1,'Thorim25 - Random Ulduar craft recipe'),
(26956,45038,0,8,0,1,0,1,1,'Thorim25 - Val''anyr fragment'),
(26956,45087,0,10,0,1,0,1,1,'Thorim25 - Runed Orb'),
(26956,47241,0,100,0,1,0,1,1,'Thorim25 - Emblem of Triumph');

-- Pathing for Thorim Event Bunny Entry: 32892 'TDB FORMAT'
SET @NPC := 136515;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=2125.243,`position_y`=-279.0981,`position_z`=419.6088 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH, 1, 2151.18, -291.233, 425.000,0,0,0,0,100,0),
(@PATH, 2, 2150.276, -290.805, 425.000,0,0,0,0,100,0),
(@PATH, 3, 2149.361, -290.401, 425.000,0,0,0,0,100,0),
(@PATH, 4, 2148.436, -290.022, 425.000,0,0,0,0,100,0),
(@PATH, 5, 2147.501, -289.668, 425.000,0,0,0,0,100,0),
(@PATH, 6, 2146.556, -289.34, 425.000,0,0,0,0,100,0),
(@PATH, 7, 2145.603, -289.037, 425.000,0,0,0,0,100,0),
(@PATH, 8, 2144.642, -288.76, 425.000,0,0,0,0,100,0),
(@PATH, 9, 2143.674, -288.509, 425.000,0,0,0,0,100,0),
(@PATH, 10, 2142.7, -288.284, 425.000,0,0,0,0,100,0),
(@PATH, 11, 2141.72, -288.086, 425.000,0,0,0,0,100,0),
(@PATH, 12, 2140.735, -287.915, 425.000,0,0,0,0,100,0),
(@PATH, 13, 2139.745, -287.77, 425.000,0,0,0,0,100,0),
(@PATH, 14, 2138.752, -287.652, 425.000,0,0,0,0,100,0),
(@PATH, 15, 2137.757, -287.56, 425.000,0,0,0,0,100,0),
(@PATH, 16, 2136.759, -287.496, 425.000,0,0,0,0,100,0),
(@PATH, 17, 2135.759, -287.459, 425.000,0,0,0,0,100,0),
(@PATH, 18, 2134.76, -287.448, 425.000,0,0,0,0,100,0),
(@PATH, 19, 2133.76, -287.465, 425.000,0,0,0,0,100,0),
(@PATH, 20, 2132.761, -287.509, 425.000,0,0,0,0,100,0),
(@PATH, 21, 2131.763, -287.579, 425.000,0,0,0,0,100,0),
(@PATH, 22, 2130.768, -287.677, 425.000,0,0,0,0,100,0),
(@PATH, 23, 2129.776, -287.801, 425.000,0,0,0,0,100,0),
(@PATH, 24, 2128.787, -287.953, 425.000,0,0,0,0,100,0),
(@PATH, 25, 2127.803, -288.13, 425.000,0,0,0,0,100,0),
(@PATH, 26, 2126.825, -288.335, 425.000,0,0,0,0,100,0),
(@PATH, 27, 2125.852, -288.566, 425.000,0,0,0,0,100,0),
(@PATH, 28, 2124.885, -288.823, 425.000,0,0,0,0,100,0),
(@PATH, 29, 2123.926, -289.106, 425.000,0,0,0,0,100,0),
(@PATH, 30, 2122.975, -289.415, 425.000,0,0,0,0,100,0),
(@PATH, 31, 2122.033, -289.749, 425.000,0,0,0,0,100,0),
(@PATH, 32, 2121.1, -290.109, 425.000,0,0,0,0,100,0),
(@PATH, 33, 2120.177, -290.494, 425.000,0,0,0,0,100,0),
(@PATH, 34, 2119.264, -290.903, 425.000,0,0,0,0,100,0),
(@PATH, 35, 2118.574, -291.233, 425.000,0,0,0,0,100,0),
(@PATH, 36, 2117.717, -290.718, 425.000,0,0,0,0,100,0),
(@PATH, 37, 2116.876, -290.177, 425.000,0,0,0,0,100,0),
(@PATH, 38, 2116.052, -289.611, 425.000,0,0,0,0,100,0),
(@PATH, 39, 2115.246, -289.019, 425.000,0,0,0,0,100,0),
(@PATH, 40, 2114.458, -288.403, 425.000,0,0,0,0,100,0),
(@PATH, 41, 2113.69, -287.763, 425.000,0,0,0,0,100,0),
(@PATH, 42, 2112.942, -287.099, 425.000,0,0,0,0,100,0),
(@PATH, 43, 2112.215, -286.413, 425.000,0,0,0,0,100,0),
(@PATH, 44, 2111.509, -285.705, 425.000,0,0,0,0,100,0),
(@PATH, 45, 2110.825, -284.975, 425.000,0,0,0,0,100,0),
(@PATH, 46, 2110.164, -284.225, 425.000,0,0,0,0,100,0),
(@PATH, 47, 2109.526, -283.455, 425.000,0,0,0,0,100,0),
(@PATH, 48, 2108.913, -282.665, 425.000,0,0,0,0,100,0),
(@PATH, 49, 2108.323, -281.857, 425.000,0,0,0,0,100,0),
(@PATH, 50, 2107.76, -281.031, 425.000,0,0,0,0,100,0),
(@PATH, 51, 2107.221, -280.189, 425.000,0,0,0,0,100,0),
(@PATH, 52, 2106.709, -279.33, 425.000,0,0,0,0,100,0),
(@PATH, 53, 2106.224, -278.456, 425.000,0,0,0,0,100,0),
(@PATH, 54, 2105.765, -277.567, 425.000,0,0,0,0,100,0),
(@PATH, 55, 2105.335, -276.664, 425.000,0,0,0,0,100,0),
(@PATH, 56, 2104.932, -275.749, 425.000,0,0,0,0,100,0),
(@PATH, 57, 2104.558, -274.822, 425.000,0,0,0,0,100,0),
(@PATH, 58, 2104.212, -273.884, 425.000,0,0,0,0,100,0),
(@PATH, 59, 2103.895, -272.935, 425.000,0,0,0,0,100,0),
(@PATH, 60, 2103.608, -271.977, 425.000,0,0,0,0,100,0),
(@PATH, 61, 2103.35, -271.011, 425.000,0,0,0,0,100,0),
(@PATH, 62, 2103.122, -270.038, 425.000,0,0,0,0,100,0),
(@PATH, 63, 2102.924, -269.057, 425.000,0,0,0,0,100,0),
(@PATH, 64, 2102.757, -268.072, 425.000,0,0,0,0,100,0),
(@PATH, 65, 2102.62, -267.081, 425.000,0,0,0,0,100,0),
(@PATH, 66, 2102.513, -266.087, 425.000,0,0,0,0,100,0),
(@PATH, 67, 2102.437, -265.09, 425.000,0,0,0,0,100,0),
(@PATH, 68, 2102.392, -264.091, 425.000,0,0,0,0,100,0),
(@PATH, 69, 2102.377, -263.091, 425.000,0,0,0,0,100,0),
(@PATH, 70, 2102.393, -262.091, 425.000,0,0,0,0,100,0),
(@PATH, 71, 2102.44, -261.092, 425.000,0,0,0,0,100,0),
(@PATH, 72, 2102.518, -260.095, 425.000,0,0,0,0,100,0),
(@PATH, 73, 2102.626, -259.101, 425.000,0,0,0,0,100,0),
(@PATH, 74, 2102.765, -258.111, 425.000,0,0,0,0,100,0),
(@PATH, 75, 2102.934, -257.125, 425.000,0,0,0,0,100,0),
(@PATH, 76, 2103.134, -256.146, 425.000,0,0,0,0,100,0),
(@PATH, 77, 2103.363, -255.172, 425.000,0,0,0,0,100,0),
(@PATH, 78, 2103.623, -254.207, 425.000,0,0,0,0,100,0),
(@PATH, 79, 2103.912, -253.249, 425.000,0,0,0,0,100,0),
(@PATH, 80, 2104.23, -252.301, 425.000,0,0,0,0,100,0),
(@PATH, 81, 2104.577, -251.364, 425.000,0,0,0,0,100,0),
(@PATH, 82, 2104.953, -250.437, 425.000,0,0,0,0,100,0),
(@PATH, 83, 2105.357, -249.522, 425.000,0,0,0,0,100,0),
(@PATH, 84, 2105.79, -248.621, 425.000,0,0,0,0,100,0),
(@PATH, 85, 2106.249, -247.733, 425.000,0,0,0,0,100,0),
(@PATH, 86, 2106.736, -246.859, 425.000,0,0,0,0,100,0),
(@PATH, 87, 2107.25, -246.001, 425.000,0,0,0,0,100,0),
(@PATH, 88, 2107.789, -245.159, 425.000,0,0,0,0,100,0),
(@PATH, 89, 2108.355, -244.335, 425.000,0,0,0,0,100,0),
(@PATH, 90, 2108.945, -243.527, 425.000,0,0,0,0,100,0),
(@PATH, 91, 2109.56, -242.739, 425.000,0,0,0,0,100,0),
(@PATH, 92, 2110.199, -241.97, 425.000,0,0,0,0,100,0),
(@PATH, 93, 2110.861, -241.221, 425.000,0,0,0,0,100,0),
(@PATH, 94, 2111.546, -240.492, 425.000,0,0,0,0,100,0),
(@PATH, 95, 2112.253, -239.785, 425.000,0,0,0,0,100,0),
(@PATH, 96, 2112.982, -239.1, 425.000,0,0,0,0,100,0),
(@PATH, 97, 2113.731, -238.438, 425.000,0,0,0,0,100,0),
(@PATH, 98, 2114.501, -237.799, 425.000,0,0,0,0,100,0),
(@PATH, 99, 2115.289, -237.184, 425.000,0,0,0,0,100,0),
(@PATH, 100, 2116.096, -236.594, 425.000,0,0,0,0,100,0),
(@PATH, 101, 2116.921, -236.029, 425.000,0,0,0,0,100,0),
(@PATH, 102, 2117.763, -235.489, 425.000,0,0,0,0,100,0),
(@PATH, 103, 2118.621, -234.976, 425.000,0,0,0,0,100,0),
(@PATH, 104, 2119.495, -234.489, 425.000,0,0,0,0,100,0),
(@PATH, 105, 2120.383, -234.029, 425.000,0,0,0,0,100,0),
(@PATH, 106, 2121.284, -233.597, 425.000,0,0,0,0,100,0),
(@PATH, 107, 2122.199, -233.193, 425.000,0,0,0,0,100,0),
(@PATH, 108, 2123.126, -232.817, 425.000,0,0,0,0,100,0),
(@PATH, 109, 2124.063, -232.47, 425.000,0,0,0,0,100,0),
(@PATH, 110, 2125.011, -232.152, 425.000,0,0,0,0,100,0),
(@PATH, 111, 2125.969, -231.863, 425.000,0,0,0,0,100,0),
(@PATH, 112, 2126.934, -231.603, 425.000,0,0,0,0,100,0),
(@PATH, 113, 2127.908, -231.374, 425.000,0,0,0,0,100,0),
(@PATH, 114, 2128.888, -231.175, 425.000,0,0,0,0,100,0),
(@PATH, 115, 2129.873, -231.006, 425.000,0,0,0,0,100,0),
(@PATH, 116, 2130.863, -230.867, 425.000,0,0,0,0,100,0),
(@PATH, 117, 2131.858, -230.759, 425.000,0,0,0,0,100,0),
(@PATH, 118, 2132.855, -230.681, 425.000,0,0,0,0,100,0),
(@PATH, 119, 2133.853, -230.634, 425.000,0,0,0,0,100,0),
(@PATH, 120, 2134.853, -230.618, 425.000,0,0,0,0,100,0),
(@PATH, 121, 2134.877, -230.618, 425.000,0,0,0,0,100,0),
(@PATH, 122, 2135.877, -230.633, 425.000,0,0,0,0,100,0),
(@PATH, 123, 2136.876, -230.68, 425.000,0,0,0,0,100,0),
(@PATH, 124, 2137.873, -230.756, 425.000,0,0,0,0,100,0),
(@PATH, 125, 2138.867, -230.864, 425.000,0,0,0,0,100,0),
(@PATH, 126, 2139.857, -231.002, 425.000,0,0,0,0,100,0),
(@PATH, 127, 2140.843, -231.17, 425.000,0,0,0,0,100,0),
(@PATH, 128, 2141.823, -231.369, 425.000,0,0,0,0,100,0),
(@PATH, 129, 2142.796, -231.598, 425.000,0,0,0,0,100,0),
(@PATH, 130, 2143.762, -231.856, 425.000,0,0,0,0,100,0),
(@PATH, 131, 2144.72, -232.144, 425.000,0,0,0,0,100,0),
(@PATH, 132, 2145.668, -232.462, 425.000,0,0,0,0,100,0),
(@PATH, 133, 2146.606, -232.808, 425.000,0,0,0,0,100,0),
(@PATH, 134, 2147.533, -233.184, 425.000,0,0,0,0,100,0),
(@PATH, 135, 2148.448, -233.587, 425.000,0,0,0,0,100,0),
(@PATH, 136, 2149.35, -234.019, 425.000,0,0,0,0,100,0),
(@PATH, 137, 2150.238, -234.478, 425.000,0,0,0,0,100,0),
(@PATH, 138, 2151.112, -234.964, 425.000,0,0,0,0,100,0),
(@PATH, 139, 2151.971, -235.476, 425.000,0,0,0,0,100,0),
(@PATH, 140, 2152.813, -236.015, 425.000,0,0,0,0,100,0),
(@PATH, 141, 2153.638, -236.58, 425.000,0,0,0,0,100,0),
(@PATH, 142, 2154.446, -237.17, 425.000,0,0,0,0,100,0),
(@PATH, 143, 2155.235, -237.784, 425.000,0,0,0,0,100,0),
(@PATH, 144, 2156.005, -238.422, 425.000,0,0,0,0,100,0),
(@PATH, 145, 2156.754, -239.084, 425.000,0,0,0,0,100,0),
(@PATH, 146, 2157.483, -239.769, 425.000,0,0,0,0,100,0),
(@PATH, 147, 2158.191, -240.475, 425.000,0,0,0,0,100,0),
(@PATH, 148, 2158.877, -241.203, 425.000,0,0,0,0,100,0),
(@PATH, 149, 2159.539, -241.952, 425.000,0,0,0,0,100,0),
(@PATH, 150, 2160.179, -242.72, 425.000,0,0,0,0,100,0),
(@PATH, 151, 2160.795, -243.508, 425.000,0,0,0,0,100,0),
(@PATH, 152, 2161.386, -244.315, 425.000,0,0,0,0,100,0),
(@PATH, 153, 2161.951, -245.14, 425.000,0,0,0,0,100,0),
(@PATH, 154, 2162.492, -245.981, 425.000,0,0,0,0,100,0),
(@PATH, 155, 2163.006, -246.839, 425.000,0,0,0,0,100,0),
(@PATH, 156, 2163.493, -247.712, 425.000,0,0,0,0,100,0),
(@PATH, 157, 2163.954, -248.599, 425.000,0,0,0,0,100,0),
(@PATH, 158, 2164.387, -249.501, 425.000,0,0,0,0,100,0),
(@PATH, 159, 2164.792, -250.415, 425.000,0,0,0,0,100,0),
(@PATH, 160, 2165.168, -251.341, 425.000,0,0,0,0,100,0),
(@PATH, 161, 2165.516, -252.279, 425.000,0,0,0,0,100,0),
(@PATH, 162, 2165.835, -253.227, 425.000,0,0,0,0,100,0),
(@PATH, 163, 2166.125, -254.184, 425.000,0,0,0,0,100,0),
(@PATH, 164, 2166.385, -255.149, 425.000,0,0,0,0,100,0),
(@PATH, 165, 2166.615, -256.122, 425.000,0,0,0,0,100,0),
(@PATH, 166, 2166.815, -257.102, 425.000,0,0,0,0,100,0),
(@PATH, 167, 2166.985, -258.087, 425.000,0,0,0,0,100,0),
(@PATH, 168, 2167.125, -259.078, 425.000,0,0,0,0,100,0),
(@PATH, 169, 2167.234, -260.072, 425.000,0,0,0,0,100,0),
(@PATH, 170, 2167.312, -261.068, 425.000,0,0,0,0,100,0),
(@PATH, 171, 2167.36, -262.067, 425.000,0,0,0,0,100,0),
(@PATH, 172, 2167.377, -263.067, 425.000,0,0,0,0,100,0),
(@PATH, 173, 2167.363, -264.067, 425.000,0,0,0,0,100,0),
(@PATH, 174, 2167.319, -265.066, 425.000,0,0,0,0,100,0),
(@PATH, 175, 2167.243, -266.063, 425.000,0,0,0,0,100,0),
(@PATH, 176, 2167.137, -267.057, 425.000,0,0,0,0,100,0),
(@PATH, 177, 2167.001, -268.048, 425.000,0,0,0,0,100,0),
(@PATH, 178, 2166.834, -269.034, 425.000,0,0,0,0,100,0),
(@PATH, 179, 2166.637, -270.014, 425.000,0,0,0,0,100,0),
(@PATH, 180, 2166.41, -270.988, 425.000,0,0,0,0,100,0),
(@PATH, 181, 2166.153, -271.954, 425.000,0,0,0,0,100,0),
(@PATH, 182, 2165.866, -272.912, 425.000,0,0,0,0,100,0),
(@PATH, 183, 2165.55, -273.861, 425.000,0,0,0,0,100,0),
(@PATH, 184, 2165.205, -274.8, 425.000,0,0,0,0,100,0),
(@PATH, 185, 2164.831, -275.727, 425.000,0,0,0,0,100,0),
(@PATH, 186, 2164.429, -276.643, 425.000,0,0,0,0,100,0),
(@PATH, 187, 2163.999, -277.546, 425.000,0,0,0,0,100,0),
(@PATH, 188, 2163.541, -278.435, 425.000,0,0,0,0,100,0),
(@PATH, 189, 2163.057, -279.309, 425.000,0,0,0,0,100,0),
(@PATH, 190, 2162.545, -280.168, 425.000,0,0,0,0,100,0),
(@PATH, 191, 2162.008, -281.012, 425.000,0,0,0,0,100,0),
(@PATH, 192, 2161.444, -281.838, 425.000,0,0,0,0,100,0),
(@PATH, 193, 2160.856, -282.646, 425.000,0,0,0,0,100,0),
(@PATH, 194, 2160.243, -283.436, 425.000,0,0,0,0,100,0),
(@PATH, 195, 2159.606, -284.207, 425.000,0,0,0,0,100,0),
(@PATH, 196, 2158.945, -284.958, 425.000,0,0,0,0,100,0),
(@PATH, 197, 2158.262, -285.688, 425.000,0,0,0,0,100,0),
(@PATH, 198, 2157.556, -286.397, 425.000,0,0,0,0,100,0),
(@PATH, 199, 2156.83, -287.083, 425.000,0,0,0,0,100,0),
(@PATH, 200, 2156.082, -287.747, 425.000,0,0,0,0,100,0),
(@PATH, 201, 2155.314, -288.388, 425.000,0,0,0,0,100,0),
(@PATH, 202, 2154.527, -289.005, 425.000,0,0,0,0,100,0),
(@PATH, 203, 2153.721, -289.597, 425.000,0,0,0,0,100,0),
(@PATH, 204, 2152.898, -290.164, 425.000,0,0,0,0,100,0),
(@PATH, 205, 2152.057, -290.706, 425.000,0,0,0,0,100,0),
(@PATH, 206, 2151.2, -291.221, 425.000,0,0,0,0,100,0);
-- 0x1C2F304B60201F0000137E0000FF7768 .go 2125.243 -279.0981 419.6088
