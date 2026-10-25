/*
	asf.inc - ASF (Additional Server Functions) for open.mp
	Version 1.0.0
	Author: noiseXYZ - https://github.com/noiseXYZ/openmp-asf

	The YSF functions open.mp does not have natively, rebuilt on Pawn.RakNet.
	No YSF plugin: YSF patches SA-MP server memory and cannot load on open.mp.
	Function names and parameters match YSF.inc, so YSF code compiles as is.

	Include after <open.mp>, <Pawn.RakNet> and <pause> (if used):
		#include <open.mp>
		#include <Pawn.RakNet>
		#include <asf>

	Config (define before the include):
		ASF_MAX_OVERRIDES		per-player skin/team/style/name/pos overrides in total (1024)
		ASF_MAX_REMOVED			RemoveBuildingForPlayer calls remembered in total (8192)
		ASF_MAX_PICKUP_OVR		SetPickupForPlayer overrides in total (512)
		ASF_AFK_ACCURACY		ms without sync before a player counts as paused (1500)
		ASF_NO_EXCLUSIVE_BROADCAST	leave out SetExclusiveBroadcast (no OnOutgoingRPC hook)

	Per-player view
		SetPlayerTeamForPlayer(playerid, teamplayerid, teamid)
		GetPlayerTeamForPlayer(playerid, teamplayerid)
		ResetPlayerTeamForPlayer(playerid, teamplayerid)                      (ASF extra)
		SetPlayerSkinForPlayer(playerid, skinplayerid, skin)
		GetPlayerSkinForPlayer(playerid, skinplayerid)
		ResetPlayerSkinForPlayer(playerid, skinplayerid)                      (ASF extra)
		SetPlayerNameForPlayer(playerid, nameplayerid, const playername[])
		GetPlayerNameForPlayer(playerid, nameplayerid, playername[], size)
		ResetPlayerNameForPlayer(playerid, nameplayerid)                      (ASF extra)
		SetPlayerFightStyleForPlayer(playerid, styleplayerid, style)
		GetPlayerFightStyleForPlayer(playerid, styleplayerid)
		ResetPlayerFightStyleForPlayer(playerid, styleplayerid)               (ASF extra)
		SetPlayerPosForPlayer(playerid, posplayerid, Float:x, Float:y, Float:z, bool:forcesync = true)
		ResetPlayerPosForPlayer(playerid, posplayerid)                        (ASF extra)
		SetPlayerRotationQuatForPlayer(playerid, quatplayerid, Float:w, Float:x, Float:y, Float:z, bool:forcesync = true)
		ResetPlayerRotationQuatForPlayer(playerid, quatplayerid)              (ASF extra)
		ApplyAnimationForPlayer(playerid, animplayerid, const animlib[], const animname[], Float:fDelta, loop, lockx, locky, freeze, time)
		SetPlayerChatBubbleForPlayer(forplayerid, playerid, const text[], color, Float:drawdistance, expiretime)
		ResetPlayerMarkerForPlayer(playerid, resetplayerid)
		ShowPlayerForPlayer(forplayerid, playerid, bool:setskin = false)
		HidePlayerForPlayer(forplayerid, playerid)
		IsPlayerHiddenForPlayer(forplayerid, playerid)                        (ASF extra)
		AddPlayerForPlayer(forplayerid, playerid, isnpc = 0)
		RemovePlayerForPlayer(forplayerid, playerid)
		SpawnForWorld(playerid)
		SendPlayerDeath(playerid, forplayerid = -1)
		BroadcastDeath(playerid)
		SendBulletData(senderid, forplayerid = -1, weaponid, hittype, hitid, Float:fHitOriginX, Float:fHitOriginY, Float:fHitOriginZ, Float:fHitTargetX, Float:fHitTargetY, Float:fHitTargetZ, Float:fCenterOfHitX, Float:fCenterOfHitY, Float:fCenterOfHitZ)

	Sync
		SetPlayerDisabledKeysSync(playerid, keys, updown = 0, leftright = 0)
		GetPlayerDisabledKeysSync(playerid, &keys, &updown = 0, &leftright = 0)
		GetPlayerLastSyncedVehicleID(playerid)
		GetPlayerLastSyncedTrailerID(playerid)
		GetPlayerSyncWeapon(playerid)
		SetPlayerSyncKeys / SetPlayerSyncPos / SetPlayerSyncPosition / SetPlayerSyncVelocity / SetPlayerSyncRotationQuat
		SetPlayerSyncCameraFrontVector / SetPlayerSyncCameraPos / SetPlayerSyncCameraMode
		SetPlayerSyncWeapon / SetPlayerSyncWeaponState / SetPlayerSyncSpecialAction / SetPlayerSyncHealth / SetPlayerSyncArmour
		SetPlayerSyncVehicleId / SetPlayerSyncVehicleSeat / SetPlayerSyncVehiclePosition / SetPlayerSyncVehicleVelocity
		SetPlayerSyncVehicleRotQuat / SetPlayerSyncVehicleHealth / SetPlayerSyncTrainSpeed
		UpdatePlayerSyncData(playerid, type = -1, bool:setstate = false)
			The SetPlayerSync* functions change the player's last sync (like YSF); UpdatePlayerSyncData sends it
			to everyone who streams the player. The player's next real sync replaces it. setstate is ignored.

	Objects / vehicles / pickups
		HideObjectForPlayer(playerid, objectid) / ShowObjectForPlayer(playerid, objectid)
		HideNewObjectsForPlayer(playerid, bool:toggle) / IsObjectHiddenForPlayer(playerid, objectid) / NewObjectsHiddenForPlayer(playerid)
			Global objects only (streamer objects are player objects: use streamer's own per-player items).
			Showing restores model, materials, position and rotation, not an unfinished MoveObject.
		HideVehicle(vehicleid) / ShowVehicle(vehicleid) / IsVehicleHidden(vehicleid)
		SetPickupForPlayer(playerid, id, model, type, Float:X, Float:Y, Float:Z)
		IsBuildingRemovedForPlayer(playerid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRadius)

	Network / server
		SetExclusiveBroadcast(toggle) / BroadcastToPlayer(playerid, toggle = 1)
		TogglePlayerScoresPingsUpdate(playerid, bool:toggle) / TogglePlayerFakePing(playerid, bool:toggle) / SetPlayerFakePing(playerid, ping)
		SendRPC(playerid, rpcid, {Float,_}:...) / SendData(playerid, {Float,_}:...)    (BS_* type + value pairs, playerid -1 = everyone)
		LoadFilterScript(const scriptname[]) / UnLoadFilterScript(const scriptname[])

	Pause (only without pause.inc, which already has IsPlayerPaused / GetPlayerPausedTime)
		IsPlayerPaused(playerid) / GetPlayerPausedTime(playerid) (ms) / YSF_SetAFKAccuracy(time_ms) / YSF_GetAFKAccuracy()

	Callbacks
		OnPlayerPauseStateChange(playerid, pausestate)                       (without pause.inc)
		OnPlayerStatsAndWeaponsUpdate(playerid)                              return 0 = drop the update
		OnOutcomeScmEvent(playerid, issuerid, E_SCM_EVENT_TYPE:eventid, vehicleid, arg1, arg2)   return 0 = block

	Not possible from Pawn (need server internals): SetMaxPlayers, SetMaxNPCs, SetPlayerConnectMode, SetTimeoutTime,
	ToggleCloseConnectionFix, SetSyncBounds, ChangeRCONCommandName, GetServerSettings, YSF_SetTickRate,
	SetVehicleBeenOccupied / OccupiedTick / RespawnTick / Dead, Get/SetPlayerNameInServerQuery, OnServerQueryInfo,
	OnServerMessage, OnRemoteRCONPacket, Enable/DisableConsoleMSGsForPlayer, GetFilterScriptCount / Name,
	CallFunctionInScript, GetNPCCommandLine, SetPlayerVersion, GetLocalIP, GetColCount / GetColSphere*,
	OnPlayerClientGameInit / SendPlayerClientGameInit, ffind / frename / dfind / dcreate / drename.
*/

#if defined _asf_included
	#endinput
#endif
#define _asf_included

#define ASF_VERSION				"1.0.0"
#define ASF_AUTHOR				"noiseXYZ"

#if defined _YSF_included
	#error asf.inc replaces YSF.inc - include only one of them
#endif
#if !defined PAWNRAKNET_INC_
	#error asf.inc needs Pawn.RakNet: #include <Pawn.RakNet> before <asf>
#endif

#if !defined ASF_MAX_OVERRIDES
	#define ASF_MAX_OVERRIDES		(1024)
#endif
#if !defined ASF_MAX_REMOVED
	#define ASF_MAX_REMOVED			(8192)
#endif
#if !defined ASF_MAX_PICKUP_OVR
	#define ASF_MAX_PICKUP_OVR		(512)
#endif
#if !defined ASF_AFK_ACCURACY
	#define ASF_AFK_ACCURACY		(1500)
#endif

#define ASF_PBITS				((MAX_PLAYERS + 31) / 32)
#define ASF_OBITS				((MAX_OBJECTS + 31) / 32)
#define ASF_VBITS				((MAX_VEHICLES + 31) / 32)

#define Asf_BitGet(%0,%1)		((%0[(%1) >>> 5] & (1 << ((%1) & 31))) != 0)
#define Asf_BitSet(%0,%1)		(%0[(%1) >>> 5] |= (1 << ((%1) & 31)))
#define Asf_BitClr(%0,%1)		(%0[(%1) >>> 5] &= ~(1 << ((%1) & 31)))

// SendRPC / SendData value types (same as YSF.inc)
enum
{
	BS_BOOL,
	BS_CHAR,
	BS_UNSIGNEDCHAR,
	BS_SHORT,
	BS_UNSIGNEDSHORT,
	BS_INT,
	BS_UNSIGNEDINT,
	BS_FLOAT,
	BS_STRING
}

enum E_SCM_EVENT_TYPE
{
	SCM_EVENT_PAINTJOB = 1,
	SCM_EVENT_MOD = 2,
	SCM_EVENT_RESPRAY = 3,
	SCM_EVENT_MOD_SHOP = 4
}

// UpdatePlayerSyncData types
#define ASF_SYNC_AUTO			(-1)
#define ASF_SYNC_ONFOOT			(0)
#define ASF_SYNC_INCAR			(1)
#define ASF_SYNC_PASSENGER		(2)
#define ASF_SYNC_AIM			(3)

forward OnPlayerStatsAndWeaponsUpdate(playerid);
forward OnOutcomeScmEvent(playerid, issuerid, E_SCM_EVENT_TYPE:eventid, vehicleid, arg1, arg2);
#if !defined pause_included
	forward OnPlayerPauseStateChange(playerid, pausestate);
#endif

// packet / RPC ids (consts, not defines: the handler macros stringify the name)
const Asf_P_CAR = 200;
const Asf_P_AIM = 203;
const Asf_P_WEAPONS = 204;
const Asf_P_STATS = 205;
const Asf_P_BULLET = 206;
const Asf_P_FOOT = 207;
const Asf_P_UNOCC = 209;
const Asf_P_TRAILER = 210;
const Asf_P_PASS = 211;
const Asf_P_SPEC = 212;

const Asf_R_NAME = 11;
const Asf_R_ADD = 32;
const Asf_R_OCREATE = 44;
const Asf_R_OPOS = 45;
const Asf_R_OROT = 46;
const Asf_R_ODESTROY = 47;
const Asf_R_BUBBLE = 59;
const Asf_R_PDESTROY = 63;
const Asf_R_TEAM = 69;
const Asf_R_COLOR = 72;
const Asf_R_OATTACH = 75;
const Asf_R_OMAT = 84;
const Asf_R_ANIM = 86;
const Asf_R_STYLE = 89;
const Asf_R_PCREATE = 95;
const Asf_R_SCM = 96;
const Asf_R_OMOVE = 99;
const Asf_R_OSTOP = 122;
const Asf_R_JOIN = 137;
const Asf_R_QUIT = 138;
const Asf_R_VHEALTH = 147;
const Asf_R_SKIN = 153;
const Asf_R_SCORES = 155;
const Asf_R_VPOS = 159;
const Asf_R_VANGLE = 160;
const Asf_R_REMOVE = 163;
const Asf_R_VADD = 164;
const Asf_R_VREMOVE = 165;
const Asf_R_DEATH = 166;

#define ASF_O_SKIN				(1)
#define ASF_O_TEAM				(2)
#define ASF_O_STYLE				(4)
#define ASF_O_NAME				(8)
#define ASF_O_POS				(16)
#define ASF_O_QUAT				(32)

enum E_ASF_OVR
{
	bool:aoUsed,
	aoFor,
	aoWho,
	aoFlags,
	aoSkin,
	aoTeam,
	aoStyle,
	aoName[MAX_PLAYER_NAME + 1],
	Float:aoPos[3],
	Float:aoQuat[4]
}

enum E_ASF_PLAYER
{
	apDisKeys,
	apDisUD,
	apDisLR,
	apLastVeh,
	apLastTrailer,
	apAddFmt,				// 0 unknown, 1 normal, 2 DL (seen in the server's own RPC 32)
	bool:apNoScores,
	bool:apFakePingOn,
	apFakePing,
	bool:apHasFoot,
	bool:apHasCar,
	bool:apHasPass,
	bool:apHasAim,
	apLastTick,
	bool:apPaused,
	apPausedAt
}

enum E_ASF_RB
{
	arPlayer,				// playerid + 1, 0 = free
	arModel,
	Float:arX,
	Float:arY,
	Float:arZ,
	Float:arR
}

enum E_ASF_PICKUP
{
	akPlayer,				// playerid + 1, 0 = free
	akId,
	akModel,
	akType,
	Float:akPos[3]
}

static bool:gAsfSelf;
static gAsfHidePl[MAX_PLAYERS][ASF_PBITS];
static gAsfOvrPl[MAX_PLAYERS][ASF_PBITS];
static gAsfOvr[ASF_MAX_OVERRIDES][E_ASF_OVR];
static gAsfPl[MAX_PLAYERS][E_ASF_PLAYER];
static gAsfFoot[MAX_PLAYERS][PR_OnFootSync];
static gAsfCar[MAX_PLAYERS][PR_InCarSync];
static gAsfPass[MAX_PLAYERS][PR_PassengerSync];
static gAsfAim[MAX_PLAYERS][PR_AimSync];
static gAsfHideObj[MAX_PLAYERS][ASF_OBITS];
static bool:gAsfHideNewObj[MAX_PLAYERS];
static BitStream:gAsfObjBS[MAX_OBJECTS];
static BitStream:gAsfObjMat[MAX_OBJECTS][16];
static gAsfHideVeh[ASF_VBITS];
static BitStream:gAsfVehBS[MAX_VEHICLES];
static gAsfRB[ASF_MAX_REMOVED][E_ASF_RB];
static gAsfPk[ASF_MAX_PICKUP_OVR][E_ASF_PICKUP];
static gAsfFakePingCount;
static gAsfScoreBuf[MAX_PLAYERS][3];
#if !defined ASF_NO_EXCLUSIVE_BROADCAST
	static bool:gAsfExclusive;
	static gAsfBcast[ASF_PBITS];
#endif
#if !defined pause_included
	static gAsfAfkAcc = ASF_AFK_ACCURACY;
	static gAsfPauseTimer;
#endif

/* ---------------------------------------------------------------- helpers */

static stock Asf_Send(BitStream:bs, playerid, rpcid, bool:del = true)
{
	gAsfSelf = true;
	if(rpcid == -1) PR_SendPacket(bs, playerid, PR_HIGH_PRIORITY, PR_UNRELIABLE_SEQUENCED);
	else PR_SendRPC(bs, playerid, rpcid);
	gAsfSelf = false;
	if(del) BS_Delete(bs);
}

static stock bool:Asf_IsDL(playerid)
{
	if(gAsfPl[playerid][apAddFmt]) return (gAsfPl[playerid][apAddFmt] == 2);
	new v[32];
	GetPlayerVersion(playerid, v);
	return (strfind(v, "DL") != -1);
}

static stock Asf_OvrFind(forid, who, bool:create)
{
	if(forid < 0 || forid >= MAX_PLAYERS || who < 0 || who >= MAX_PLAYERS) return -1;
	if(Asf_BitGet(gAsfOvrPl[forid], who))
	{
		for(new i = 0; i < ASF_MAX_OVERRIDES; i++)
		{
			if(gAsfOvr[i][aoUsed] && gAsfOvr[i][aoFor] == forid && gAsfOvr[i][aoWho] == who) return i;
		}
	}
	if(!create) return -1;
	for(new i = 0; i < ASF_MAX_OVERRIDES; i++)
	{
		if(gAsfOvr[i][aoUsed]) continue;
		gAsfOvr[i][aoUsed] = true;
		gAsfOvr[i][aoFor] = forid;
		gAsfOvr[i][aoWho] = who;
		gAsfOvr[i][aoFlags] = 0;
		Asf_BitSet(gAsfOvrPl[forid], who);
		return i;
	}
	return -1;
}

// slot of (forid, who) only when it has this override flag
static stock Asf_Ovr(forid, who, flag)
{
	new slot = Asf_OvrFind(forid, who, false);
	if(slot != -1 && !(gAsfOvr[slot][aoFlags] & flag)) return -1;
	return slot;
}

static stock Asf_OvrDrop(slot, flag)
{
	gAsfOvr[slot][aoFlags] &= ~flag;
	if(gAsfOvr[slot][aoFlags]) return;
	gAsfOvr[slot][aoUsed] = false;
	Asf_BitClr(gAsfOvrPl[gAsfOvr[slot][aoFor]], gAsfOvr[slot][aoWho]);
}

static stock Asf_FootOvr(forid, who, data[PR_OnFootSync])
{
	new slot = Asf_OvrFind(forid, who, false);
	if(slot == -1) return;
	if(gAsfOvr[slot][aoFlags] & ASF_O_POS)
	{
		data[PR_position][0] = gAsfOvr[slot][aoPos][0];
		data[PR_position][1] = gAsfOvr[slot][aoPos][1];
		data[PR_position][2] = gAsfOvr[slot][aoPos][2];
	}
	if(gAsfOvr[slot][aoFlags] & ASF_O_QUAT)
	{
		for(new i = 0; i < 4; i++) data[PR_quaternion][i] = gAsfOvr[slot][aoQuat][i];
	}
}

static stock Asf_RealSkin(who, forid)
{
	new slot = Asf_Ovr(forid, who, ASF_O_SKIN);
	if(slot != -1) return gAsfOvr[slot][aoSkin];
	return GetPlayerSkin(who);
}

static stock Asf_RealTeam(who, forid)
{
	new slot = Asf_Ovr(forid, who, ASF_O_TEAM);
	if(slot != -1) return gAsfOvr[slot][aoTeam];
	return GetPlayerTeam(who);
}

static stock Asf_RealStyle(who, forid)
{
	new slot = Asf_Ovr(forid, who, ASF_O_STYLE);
	if(slot != -1) return gAsfOvr[slot][aoStyle];
	return _:GetPlayerFightingStyle(who);
}

// RPC 32: stream the player in for one viewer
static stock Asf_SendAdd(who, to)
{
	new Float:x, Float:y, Float:z, Float:a, slot = Asf_Ovr(to, who, ASF_O_POS);
	GetPlayerPos(who, x, y, z);
	GetPlayerFacingAngle(who, a);
	if(slot != -1)
	{
		x = gAsfOvr[slot][aoPos][0];
		y = gAsfOvr[slot][aoPos][1];
		z = gAsfOvr[slot][aoPos][2];
	}
	new team = Asf_RealTeam(who, to), skin = Asf_RealSkin(who, to), style = Asf_RealStyle(who, to);
	new custom = (Asf_Ovr(to, who, ASF_O_SKIN) != -1) ? 0 : GetPlayerCustomSkin(who);
	new BitStream:bs = BS_New();
	BS_WriteValue(bs, PR_UINT16, who, PR_UINT8, team, PR_UINT32, skin);
	if(Asf_IsDL(to)) BS_WriteValue(bs, PR_UINT32, custom);
	BS_WriteValue(bs, PR_FLOAT, x, PR_FLOAT, y, PR_FLOAT, z, PR_FLOAT, a, PR_UINT32, GetPlayerColor(who), PR_UINT8, style);
	for(new i = 0; i < 11; i++) BS_WriteValue(bs, PR_UINT16, GetPlayerSkillLevel(who, WEAPONSKILL:i));
	Asf_Send(bs, to, Asf_R_ADD);
}

static stock Asf_SendSkin(who, to, skin)
{
	new BitStream:bs = BS_New();
	BS_WriteValue(bs, PR_UINT32, who, PR_UINT32, skin);
	if(Asf_IsDL(to)) BS_WriteValue(bs, PR_UINT32, 0);
	Asf_Send(bs, to, Asf_R_SKIN);
}

static stock Asf_SendTeam(who, to, team)
{
	new BitStream:bs = BS_New();
	BS_WriteValue(bs, PR_UINT16, who, PR_UINT8, team);
	Asf_Send(bs, to, Asf_R_TEAM);
}

static stock Asf_SendStyle(who, to, style)
{
	new BitStream:bs = BS_New();
	BS_WriteValue(bs, PR_UINT16, who, PR_UINT8, style);
	Asf_Send(bs, to, Asf_R_STYLE);
}

static stock Asf_SendName(who, to, const name[])
{
	new buf[MAX_PLAYER_NAME + 1];
	strcat(buf, name);
	new BitStream:bs = BS_New();
	BS_WriteValue(bs, PR_UINT16, who, PR_STRING8, buf, PR_UINT8, 1);
	Asf_Send(bs, to, Asf_R_NAME);
}

// one sync packet of `who` to `to`, built from the stored last sync
static stock Asf_SendSync(who, to, type)
{
	new BitStream:bs = BS_New();
	switch(type)
	{
		case ASF_SYNC_ONFOOT:
		{
			if(!gAsfPl[who][apHasFoot]) { BS_Delete(bs); return 0; }
			new data[PR_OnFootSync];
			data = gAsfFoot[who];
			Asf_FootOvr(to, who, data);
			BS_WriteValue(bs, PR_UINT8, Asf_P_FOOT, PR_UINT16, who);
			BS_WriteOnFootSync(bs, data, true);
		}
		case ASF_SYNC_INCAR:
		{
			if(!gAsfPl[who][apHasCar]) { BS_Delete(bs); return 0; }
			new data[PR_InCarSync];
			data = gAsfCar[who];
			BS_WriteValue(bs, PR_UINT8, Asf_P_CAR, PR_UINT16, who);
			BS_WriteInCarSync(bs, data, true);
		}
		case ASF_SYNC_PASSENGER:
		{
			if(!gAsfPl[who][apHasPass]) { BS_Delete(bs); return 0; }
			new data[PR_PassengerSync];
			data = gAsfPass[who];
			BS_WriteValue(bs, PR_UINT8, Asf_P_PASS, PR_UINT16, who);
			BS_WritePassengerSync(bs, data);
		}
		case ASF_SYNC_AIM:
		{
			if(!gAsfPl[who][apHasAim]) { BS_Delete(bs); return 0; }
			new data[PR_AimSync];
			data = gAsfAim[who];
			BS_WriteValue(bs, PR_UINT8, Asf_P_AIM, PR_UINT16, who);
			BS_WriteAimSync(bs, data);
		}
		default:
		{
			BS_Delete(bs);
			return 0;
		}
	}
	Asf_Send(bs, to, -1);
	return 1;
}

static stock Asf_Touch(playerid)
{
	#if !defined pause_included
		gAsfPl[playerid][apLastTick] = GetTickCount();
		if(gAsfPl[playerid][apPaused])
		{
			gAsfPl[playerid][apPaused] = false;
			if(funcidx("OnPlayerPauseStateChange") != -1) CallLocalFunction("OnPlayerPauseStateChange", "ii", playerid, 0);
		}
	#else
		#pragma unused playerid
	#endif
}

static stock Asf_ClearObj(objectid)
{
	if(gAsfObjBS[objectid] != BitStream:0) BS_Delete(gAsfObjBS[objectid]);
	gAsfObjBS[objectid] = BitStream:0;
	for(new i = 0; i < 16; i++)
	{
		if(gAsfObjMat[objectid][i] != BitStream:0) BS_Delete(gAsfObjMat[objectid][i]);
		gAsfObjMat[objectid][i] = BitStream:0;
	}
}

static stock Asf_Reset(playerid)
{
	for(new i = 0; i < ASF_PBITS; i++)
	{
		gAsfHidePl[playerid][i] = 0;
		gAsfOvrPl[playerid][i] = 0;
	}
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		Asf_BitClr(gAsfHidePl[i], playerid);
		Asf_BitClr(gAsfOvrPl[i], playerid);
	}
	for(new i = 0; i < ASF_MAX_OVERRIDES; i++)
	{
		if(gAsfOvr[i][aoUsed] && (gAsfOvr[i][aoFor] == playerid || gAsfOvr[i][aoWho] == playerid)) gAsfOvr[i][aoUsed] = false;
	}
	for(new i = 0; i < ASF_OBITS; i++) gAsfHideObj[playerid][i] = 0;
	gAsfHideNewObj[playerid] = false;
	for(new i = 0; i < ASF_MAX_REMOVED; i++)
	{
		if(gAsfRB[i][arPlayer] == playerid + 1) gAsfRB[i][arPlayer] = 0;
	}
	for(new i = 0; i < ASF_MAX_PICKUP_OVR; i++)
	{
		if(gAsfPk[i][akPlayer] == playerid + 1) gAsfPk[i][akPlayer] = 0;
	}
	if(gAsfPl[playerid][apFakePingOn]) gAsfFakePingCount--;
	gAsfPl[playerid][apDisKeys] = 0;
	gAsfPl[playerid][apDisUD] = 0;
	gAsfPl[playerid][apDisLR] = 0;
	gAsfPl[playerid][apLastVeh] = INVALID_VEHICLE_ID;
	gAsfPl[playerid][apLastTrailer] = INVALID_VEHICLE_ID;
	gAsfPl[playerid][apAddFmt] = 0;
	gAsfPl[playerid][apNoScores] = false;
	gAsfPl[playerid][apFakePingOn] = false;
	gAsfPl[playerid][apFakePing] = 0;
	gAsfPl[playerid][apHasFoot] = false;
	gAsfPl[playerid][apHasCar] = false;
	gAsfPl[playerid][apHasPass] = false;
	gAsfPl[playerid][apHasAim] = false;
	gAsfPl[playerid][apLastTick] = 0;
	gAsfPl[playerid][apPaused] = false;
	gAsfPl[playerid][apPausedAt] = 0;
	#if !defined ASF_NO_EXCLUSIVE_BROADCAST
		Asf_BitClr(gAsfBcast, playerid);
	#endif
}

// drop a sync packet of a hidden player (or one in a hidden vehicle)
static stock Asf_DropSync(playerid, BitStream:bs, bool:veh)
{
	new who, vehicleid;
	BS_IgnoreBits(bs, 8);
	BS_ReadValue(bs, PR_UINT16, who);
	if(who < MAX_PLAYERS && Asf_BitGet(gAsfHidePl[playerid], who)) return 0;
	if(veh)
	{
		BS_ReadValue(bs, PR_UINT16, vehicleid);
		if(vehicleid > 0 && vehicleid < MAX_VEHICLES && Asf_BitGet(gAsfHideVeh, vehicleid)) return 0;
	}
	return 1;
}

static stock Asf_ObjBlocked(playerid, BitStream:bs)
{
	new id;
	BS_ReadValue(bs, PR_UINT16, id);
	return (id < MAX_OBJECTS && Asf_BitGet(gAsfHideObj[playerid], id));
}

/* ---------------------------------------------------------------- callbacks */

public OnPlayerConnect(playerid)
{
	Asf_Reset(playerid);
	#if !defined pause_included
		if(!gAsfPauseTimer) gAsfPauseTimer = SetTimer("Asf_PauseTick", 250, true);
	#endif
	#if defined Asf_OnPlayerConnect
		return Asf_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect Asf_OnPlayerConnect
#if defined Asf_OnPlayerConnect
	forward Asf_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
	#if defined Asf_OnPlayerDisconnect
		Asf_OnPlayerDisconnect(playerid, reason);
	#endif
	Asf_Reset(playerid);
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect Asf_OnPlayerDisconnect
#if defined Asf_OnPlayerDisconnect
	forward Asf_OnPlayerDisconnect(playerid, reason);
#endif

#if !defined ASF_NO_EXCLUSIVE_BROADCAST
	public OnOutgoingRPC(playerid, rpcid, BitStream:bs)
	{
		if(gAsfExclusive && !gAsfSelf && playerid >= 0 && playerid < MAX_PLAYERS && !Asf_BitGet(gAsfBcast, playerid)) return 0;
		#if defined Asf_OnOutgoingRPC
			return Asf_OnOutgoingRPC(playerid, rpcid, bs);
		#else
			return 1;
		#endif
	}
	#if defined _ALS_OnOutgoingRPC
		#undef OnOutgoingRPC
	#else
		#define _ALS_OnOutgoingRPC
	#endif
	#define OnOutgoingRPC Asf_OnOutgoingRPC
	#if defined Asf_OnOutgoingRPC
		forward Asf_OnOutgoingRPC(playerid, rpcid, BitStream:bs);
	#endif
#endif

#if !defined pause_included
	forward Asf_PauseTick();
	public Asf_PauseTick()
	{
		new now = GetTickCount(), PLAYER_STATE:st;
		for(new i = 0; i < MAX_PLAYERS; i++)
		{
			if(!IsPlayerConnected(i) || IsPlayerNPC(i) || gAsfPl[i][apPaused] || !gAsfPl[i][apLastTick]) continue;
			st = GetPlayerState(i);
			if(st == PLAYER_STATE_NONE || st == PLAYER_STATE_WASTED) continue;
			if(now - gAsfPl[i][apLastTick] < gAsfAfkAcc) continue;
			gAsfPl[i][apPaused] = true;
			gAsfPl[i][apPausedAt] = gAsfPl[i][apLastTick];
			if(funcidx("OnPlayerPauseStateChange") != -1) CallLocalFunction("OnPlayerPauseStateChange", "ii", i, 1);
		}
		return 1;
	}
#endif

/* ---------------------------------------------------------------- incoming sync */

IPacket:Asf_P_FOOT(playerid, BitStream:bs)
{
	new data[PR_OnFootSync];
	BS_IgnoreBits(bs, 8);
	BS_ReadOnFootSync(bs, data);
	Asf_Touch(playerid);
	if(gAsfPl[playerid][apDisKeys] || gAsfPl[playerid][apDisUD] || gAsfPl[playerid][apDisLR])
	{
		data[PR_keys] &= ~gAsfPl[playerid][apDisKeys];
		if(gAsfPl[playerid][apDisUD]) data[PR_udKey] = 0;
		if(gAsfPl[playerid][apDisLR]) data[PR_lrKey] = 0;
		BS_SetWriteOffset(bs, 8);
		BS_WriteOnFootSync(bs, data);
	}
	gAsfFoot[playerid] = data;
	gAsfPl[playerid][apHasFoot] = true;
	return 1;
}

IPacket:Asf_P_CAR(playerid, BitStream:bs)
{
	new data[PR_InCarSync];
	BS_IgnoreBits(bs, 8);
	BS_ReadInCarSync(bs, data);
	Asf_Touch(playerid);
	gAsfPl[playerid][apLastVeh] = data[PR_vehicleId];
	gAsfPl[playerid][apLastTrailer] = data[PR_trailerId];
	if(gAsfPl[playerid][apDisKeys] || gAsfPl[playerid][apDisUD] || gAsfPl[playerid][apDisLR])
	{
		data[PR_keys] &= ~gAsfPl[playerid][apDisKeys];
		if(gAsfPl[playerid][apDisUD]) data[PR_udKey] = 0;
		if(gAsfPl[playerid][apDisLR]) data[PR_lrKey] = 0;
		BS_SetWriteOffset(bs, 8);
		BS_WriteInCarSync(bs, data);
	}
	gAsfCar[playerid] = data;
	gAsfPl[playerid][apHasCar] = true;
	return 1;
}

IPacket:Asf_P_PASS(playerid, BitStream:bs)
{
	new data[PR_PassengerSync];
	BS_IgnoreBits(bs, 8);
	BS_ReadPassengerSync(bs, data);
	Asf_Touch(playerid);
	if(gAsfPl[playerid][apDisKeys] || gAsfPl[playerid][apDisUD] || gAsfPl[playerid][apDisLR])
	{
		data[PR_keys] &= ~gAsfPl[playerid][apDisKeys];
		if(gAsfPl[playerid][apDisUD]) data[PR_udKey] = 0;
		if(gAsfPl[playerid][apDisLR]) data[PR_lrKey] = 0;
		BS_SetWriteOffset(bs, 8);
		BS_WritePassengerSync(bs, data);
	}
	gAsfPass[playerid] = data;
	gAsfPl[playerid][apHasPass] = true;
	return 1;
}

IPacket:Asf_P_AIM(playerid, BitStream:bs)
{
	new data[PR_AimSync];
	BS_IgnoreBits(bs, 8);
	BS_ReadAimSync(bs, data);
	gAsfAim[playerid] = data;
	gAsfPl[playerid][apHasAim] = true;
	return 1;
}

IPacket:Asf_P_SPEC(playerid, BitStream:bs)
{
	#pragma unused bs
	Asf_Touch(playerid);
	return 1;
}

IPacket:Asf_P_STATS(playerid, BitStream:bs)
{
	#pragma unused bs
	if(funcidx("OnPlayerStatsAndWeaponsUpdate") == -1) return 1;
	return CallLocalFunction("OnPlayerStatsAndWeaponsUpdate", "i", playerid) ? 1 : 0;
}

IPacket:Asf_P_WEAPONS(playerid, BitStream:bs)
{
	#pragma unused bs
	if(funcidx("OnPlayerStatsAndWeaponsUpdate") == -1) return 1;
	return CallLocalFunction("OnPlayerStatsAndWeaponsUpdate", "i", playerid) ? 1 : 0;
}

/* ---------------------------------------------------------------- outgoing sync */

OPacket:Asf_P_FOOT(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	new who;
	BS_IgnoreBits(bs, 8);
	BS_ReadValue(bs, PR_UINT16, who);
	if(who >= MAX_PLAYERS) return 1;
	if(Asf_BitGet(gAsfHidePl[playerid], who)) return 0;
	if(!Asf_BitGet(gAsfOvrPl[playerid], who)) return 1;
	new slot = Asf_OvrFind(playerid, who, false);
	if(slot == -1 || !(gAsfOvr[slot][aoFlags] & (ASF_O_POS | ASF_O_QUAT))) return 1;
	new data[PR_OnFootSync];
	BS_ReadOnFootSync(bs, data, true);
	Asf_FootOvr(playerid, who, data);
	BS_SetWriteOffset(bs, 24);
	BS_WriteOnFootSync(bs, data, true);
	return 1;
}

OPacket:Asf_P_CAR(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	return Asf_DropSync(playerid, bs, true);
}

OPacket:Asf_P_PASS(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	return Asf_DropSync(playerid, bs, true);
}

OPacket:Asf_P_UNOCC(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	return Asf_DropSync(playerid, bs, true);
}

OPacket:Asf_P_TRAILER(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	return Asf_DropSync(playerid, bs, true);
}

OPacket:Asf_P_AIM(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	return Asf_DropSync(playerid, bs, false);
}

OPacket:Asf_P_BULLET(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	return Asf_DropSync(playerid, bs, false);
}

/* ---------------------------------------------------------------- outgoing RPCs */

ORPC:Asf_R_ADD(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	new who, bits;
	BS_GetNumberOfBitsUsed(bs, bits);
	// 224 / 400 bits = normal client without / with skills, 256 / 432 = DL client
	new bool:dl = (bits == 256 || bits == 432), bool:skills = (bits == 400 || bits == 432);
	if(bits == 224 || bits == 400 || dl) gAsfPl[playerid][apAddFmt] = dl ? 2 : 1;
	BS_ReadValue(bs, PR_UINT16, who);
	if(who >= MAX_PLAYERS) return 1;
	if(Asf_BitGet(gAsfHidePl[playerid], who)) return 0;
	if(!Asf_BitGet(gAsfOvrPl[playerid], who) || !gAsfPl[playerid][apAddFmt]) return 1;
	new slot = Asf_OvrFind(playerid, who, false);
	if(slot == -1) return 1;
	new team, skin, custom, Float:pos[3], Float:ang, col, style, skill[11];
	BS_ReadValue(bs, PR_UINT8, team, PR_UINT32, skin);
	if(dl) BS_ReadValue(bs, PR_UINT32, custom);
	BS_ReadValue(bs, PR_FLOAT3, pos, PR_FLOAT, ang, PR_UINT32, col, PR_UINT8, style);
	if(skills) for(new i = 0; i < 11; i++) BS_ReadValue(bs, PR_UINT16, skill[i]);
	new flags = gAsfOvr[slot][aoFlags];
	if(flags & ASF_O_TEAM) team = gAsfOvr[slot][aoTeam];
	if(flags & ASF_O_SKIN) skin = gAsfOvr[slot][aoSkin], custom = 0;
	if(flags & ASF_O_STYLE) style = gAsfOvr[slot][aoStyle];
	if(flags & ASF_O_POS) pos[0] = gAsfOvr[slot][aoPos][0], pos[1] = gAsfOvr[slot][aoPos][1], pos[2] = gAsfOvr[slot][aoPos][2];
	BS_SetWriteOffset(bs, 16);
	BS_WriteValue(bs, PR_UINT8, team, PR_UINT32, skin);
	if(dl) BS_WriteValue(bs, PR_UINT32, custom);
	BS_WriteValue(bs, PR_FLOAT3, pos, PR_FLOAT, ang, PR_UINT32, col, PR_UINT8, style);
	if(skills) for(new i = 0; i < 11; i++) BS_WriteValue(bs, PR_UINT16, skill[i]);
	return 1;
}

ORPC:Asf_R_NAME(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	new who;
	BS_ReadValue(bs, PR_UINT16, who);
	new slot = Asf_Ovr(playerid, who, ASF_O_NAME);
	if(slot == -1) return 1;
	new buf[MAX_PLAYER_NAME + 1];
	strcat(buf, gAsfOvr[slot][aoName]);
	BS_SetWriteOffset(bs, 16);
	BS_WriteValue(bs, PR_STRING8, buf, PR_UINT8, 1);
	return 1;
}

ORPC:Asf_R_SKIN(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	new who, skin, custom, bits;
	BS_GetNumberOfBitsUsed(bs, bits);
	BS_ReadValue(bs, PR_UINT32, who, PR_UINT32, skin);
	new slot = Asf_Ovr(playerid, who, ASF_O_SKIN);
	if(slot == -1) return 1;
	if(bits >= 96) BS_ReadValue(bs, PR_UINT32, custom);
	BS_SetWriteOffset(bs, 32);
	BS_WriteValue(bs, PR_UINT32, gAsfOvr[slot][aoSkin]);
	if(bits >= 96) BS_WriteValue(bs, PR_UINT32, 0);
	return 1;
}

ORPC:Asf_R_TEAM(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	new who;
	BS_ReadValue(bs, PR_UINT16, who);
	new slot = Asf_Ovr(playerid, who, ASF_O_TEAM);
	if(slot == -1) return 1;
	BS_SetWriteOffset(bs, 16);
	BS_WriteValue(bs, PR_UINT8, gAsfOvr[slot][aoTeam]);
	return 1;
}

ORPC:Asf_R_STYLE(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	new who;
	BS_ReadValue(bs, PR_UINT16, who);
	new slot = Asf_Ovr(playerid, who, ASF_O_STYLE);
	if(slot == -1) return 1;
	BS_SetWriteOffset(bs, 16);
	BS_WriteValue(bs, PR_UINT8, gAsfOvr[slot][aoStyle]);
	return 1;
}

ORPC:Asf_R_BUBBLE(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	new who;
	BS_ReadValue(bs, PR_UINT16, who);
	return (who < MAX_PLAYERS && Asf_BitGet(gAsfHidePl[playerid], who)) ? 0 : 1;
}

ORPC:Asf_R_OCREATE(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	new id;
	BS_ReadValue(bs, PR_UINT16, id);
	if(id >= MAX_OBJECTS || !IsValidObject(id)) return 1;
	if(gAsfObjBS[id] != BitStream:0) BS_Delete(gAsfObjBS[id]);
	gAsfObjBS[id] = BS_NewCopy(bs);
	if(Asf_BitGet(gAsfHideObj[playerid], id)) return 0;
	if(gAsfHideNewObj[playerid])
	{
		Asf_BitSet(gAsfHideObj[playerid], id);
		return 0;
	}
	return 1;
}

ORPC:Asf_R_OMAT(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	new id, mtype, idx;
	BS_ReadValue(bs, PR_UINT16, id, PR_UINT8, mtype, PR_UINT8, idx);
	#pragma unused mtype
	if(id >= MAX_OBJECTS || !IsValidObject(id)) return 1;
	if(idx < 16)
	{
		if(gAsfObjMat[id][idx] != BitStream:0) BS_Delete(gAsfObjMat[id][idx]);
		gAsfObjMat[id][idx] = BS_NewCopy(bs);
	}
	return Asf_BitGet(gAsfHideObj[playerid], id) ? 0 : 1;
}

ORPC:Asf_R_ODESTROY(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	new id;
	BS_ReadValue(bs, PR_UINT16, id);
	if(id < MAX_OBJECTS && !IsValidObject(id))
	{
		Asf_ClearObj(id);
		Asf_BitClr(gAsfHideObj[playerid], id);
	}
	return 1;
}

ORPC:Asf_R_OPOS(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	return Asf_ObjBlocked(playerid, bs) ? 0 : 1;
}

ORPC:Asf_R_OROT(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	return Asf_ObjBlocked(playerid, bs) ? 0 : 1;
}

ORPC:Asf_R_OMOVE(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	return Asf_ObjBlocked(playerid, bs) ? 0 : 1;
}

ORPC:Asf_R_OSTOP(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	return Asf_ObjBlocked(playerid, bs) ? 0 : 1;
}

ORPC:Asf_R_OATTACH(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	return Asf_ObjBlocked(playerid, bs) ? 0 : 1;
}

ORPC:Asf_R_VADD(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	new id;
	BS_ReadValue(bs, PR_UINT16, id);
	if(id <= 0 || id >= MAX_VEHICLES) return 1;
	if(gAsfVehBS[id] != BitStream:0) BS_Delete(gAsfVehBS[id]);
	gAsfVehBS[id] = BS_NewCopy(bs);
	return Asf_BitGet(gAsfHideVeh, id) ? 0 : 1;
}

ORPC:Asf_R_VREMOVE(playerid, BitStream:bs)
{
	#pragma unused playerid
	if(gAsfSelf) return 1;
	new id;
	BS_ReadValue(bs, PR_UINT16, id);
	if(id > 0 && id < MAX_VEHICLES && !IsValidVehicle(id))
	{
		Asf_BitClr(gAsfHideVeh, id);
		if(gAsfVehBS[id] != BitStream:0) BS_Delete(gAsfVehBS[id]);
		gAsfVehBS[id] = BitStream:0;
	}
	return 1;
}

ORPC:Asf_R_PCREATE(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	new id;
	BS_ReadValue(bs, PR_INT32, id);
	for(new i = 0; i < ASF_MAX_PICKUP_OVR; i++)
	{
		if(gAsfPk[i][akPlayer] != playerid + 1 || gAsfPk[i][akId] != id) continue;
		BS_SetWriteOffset(bs, 32);
		BS_WriteValue(bs, PR_INT32, gAsfPk[i][akModel], PR_INT32, gAsfPk[i][akType], PR_FLOAT, gAsfPk[i][akPos][0], PR_FLOAT, gAsfPk[i][akPos][1], PR_FLOAT, gAsfPk[i][akPos][2]);
		break;
	}
	return 1;
}

ORPC:Asf_R_PDESTROY(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	new id;
	BS_ReadValue(bs, PR_INT32, id);
	if(IsValidPickup(id)) return 1;
	for(new i = 0; i < ASF_MAX_PICKUP_OVR; i++)
	{
		if(gAsfPk[i][akPlayer] == playerid + 1 && gAsfPk[i][akId] == id) gAsfPk[i][akPlayer] = 0;
	}
	return 1;
}

ORPC:Asf_R_SCM(playerid, BitStream:bs)
{
	if(gAsfSelf || funcidx("OnOutcomeScmEvent") == -1) return 1;
	new issuer, ev, a1, a2, a3;
	BS_ReadValue(bs, PR_UINT16, issuer, PR_INT32, ev, PR_INT32, a1, PR_INT32, a2, PR_INT32, a3);
	return CallLocalFunction("OnOutcomeScmEvent", "iiiiii", playerid, issuer, ev, a1, a2, a3) ? 1 : 0;
}

ORPC:Asf_R_SCORES(playerid, BitStream:bs)
{
	if(gAsfSelf) return 1;
	if(gAsfPl[playerid][apNoScores]) return 0;
	if(!gAsfFakePingCount) return 1;
	new bits, n;
	BS_GetNumberOfBitsUsed(bs, bits);
	n = bits / 80;
	if(n > MAX_PLAYERS) n = MAX_PLAYERS;
	for(new i = 0; i < n; i++)
	{
		BS_ReadValue(bs, PR_UINT16, gAsfScoreBuf[i][0], PR_INT32, gAsfScoreBuf[i][1], PR_UINT32, gAsfScoreBuf[i][2]);
		new who = gAsfScoreBuf[i][0];
		if(who < MAX_PLAYERS && gAsfPl[who][apFakePingOn]) gAsfScoreBuf[i][2] = gAsfPl[who][apFakePing];
	}
	BS_SetWriteOffset(bs, 0);
	for(new i = 0; i < n; i++) BS_WriteValue(bs, PR_UINT16, gAsfScoreBuf[i][0], PR_INT32, gAsfScoreBuf[i][1], PR_UINT32, gAsfScoreBuf[i][2]);
	return 1;
}

/* ---------------------------------------------------------------- RemoveBuildingForPlayer record */

stock bool:Asf_RemoveBuildingForPlayer(playerid, modelid, Float:centerX, Float:centerY, Float:centerZ, Float:radius)
{
	if(playerid >= 0 && playerid < MAX_PLAYERS)
	{
		for(new i = 0; i < ASF_MAX_REMOVED; i++)
		{
			if(gAsfRB[i][arPlayer]) continue;
			gAsfRB[i][arPlayer] = playerid + 1;
			gAsfRB[i][arModel] = modelid;
			gAsfRB[i][arX] = centerX;
			gAsfRB[i][arY] = centerY;
			gAsfRB[i][arZ] = centerZ;
			gAsfRB[i][arR] = radius;
			break;
		}
	}
	return RemoveBuildingForPlayer(playerid, modelid, centerX, centerY, centerZ, radius);
}
#if defined _ALS_RemoveBuildingForPlayer
	#undef RemoveBuildingForPlayer
#else
	#define _ALS_RemoveBuildingForPlayer
#endif
#define RemoveBuildingForPlayer Asf_RemoveBuildingForPlayer

stock IsBuildingRemovedForPlayer(playerid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRadius)
{
	for(new i = 0; i < ASF_MAX_REMOVED; i++)
	{
		if(gAsfRB[i][arPlayer] != playerid + 1) continue;
		if(gAsfRB[i][arModel] != -1 && modelid != -1 && gAsfRB[i][arModel] != modelid) continue;
		new Float:dx = gAsfRB[i][arX] - fX, Float:dy = gAsfRB[i][arY] - fY, Float:dz = gAsfRB[i][arZ] - fZ;
		if(floatsqroot(dx * dx + dy * dy + dz * dz) <= gAsfRB[i][arR] + fRadius) return 1;
	}
	return 0;
}

/* ---------------------------------------------------------------- per-player view */

stock SetPlayerTeamForPlayer(playerid, teamplayerid, teamid)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(teamplayerid)) return 0;
	new slot = Asf_OvrFind(playerid, teamplayerid, true);
	if(slot == -1) return 0;
	gAsfOvr[slot][aoFlags] |= ASF_O_TEAM;
	gAsfOvr[slot][aoTeam] = teamid;
	Asf_SendTeam(teamplayerid, playerid, teamid);
	return 1;
}

stock GetPlayerTeamForPlayer(playerid, teamplayerid)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(teamplayerid)) return 0;
	return Asf_RealTeam(teamplayerid, playerid);
}

stock ResetPlayerTeamForPlayer(playerid, teamplayerid)
{
	new slot = Asf_Ovr(playerid, teamplayerid, ASF_O_TEAM);
	if(slot == -1) return 0;
	Asf_OvrDrop(slot, ASF_O_TEAM);
	Asf_SendTeam(teamplayerid, playerid, GetPlayerTeam(teamplayerid));
	return 1;
}

stock SetPlayerSkinForPlayer(playerid, skinplayerid, skin)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(skinplayerid)) return 0;
	new slot = Asf_OvrFind(playerid, skinplayerid, true);
	if(slot == -1) return 0;
	gAsfOvr[slot][aoFlags] |= ASF_O_SKIN;
	gAsfOvr[slot][aoSkin] = skin;
	Asf_SendSkin(skinplayerid, playerid, skin);
	return 1;
}

stock GetPlayerSkinForPlayer(playerid, skinplayerid)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(skinplayerid)) return 0;
	return Asf_RealSkin(skinplayerid, playerid);
}

stock ResetPlayerSkinForPlayer(playerid, skinplayerid)
{
	new slot = Asf_Ovr(playerid, skinplayerid, ASF_O_SKIN);
	if(slot == -1) return 0;
	Asf_OvrDrop(slot, ASF_O_SKIN);
	Asf_SendSkin(skinplayerid, playerid, GetPlayerSkin(skinplayerid));
	return 1;
}

stock SetPlayerNameForPlayer(playerid, nameplayerid, const playername[])
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(nameplayerid)) return 0;
	new len = strlen(playername);
	if(len < 1 || len > MAX_PLAYER_NAME) return 0;
	new slot = Asf_OvrFind(playerid, nameplayerid, true);
	if(slot == -1) return 0;
	gAsfOvr[slot][aoFlags] |= ASF_O_NAME;
	gAsfOvr[slot][aoName][0] = EOS;
	strcat(gAsfOvr[slot][aoName], playername, MAX_PLAYER_NAME + 1);
	Asf_SendName(nameplayerid, playerid, playername);
	return 1;
}

stock GetPlayerNameForPlayer(playerid, nameplayerid, playername[], size = sizeof(playername))
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(nameplayerid)) return 0;
	new slot = Asf_Ovr(playerid, nameplayerid, ASF_O_NAME);
	if(slot == -1) return GetPlayerName(nameplayerid, playername, size);
	playername[0] = EOS;
	strcat(playername, gAsfOvr[slot][aoName], size);
	return strlen(playername);
}

stock ResetPlayerNameForPlayer(playerid, nameplayerid)
{
	new slot = Asf_Ovr(playerid, nameplayerid, ASF_O_NAME);
	if(slot == -1) return 0;
	Asf_OvrDrop(slot, ASF_O_NAME);
	new name[MAX_PLAYER_NAME + 1];
	GetPlayerName(nameplayerid, name);
	Asf_SendName(nameplayerid, playerid, name);
	return 1;
}

stock SetPlayerFightStyleForPlayer(playerid, styleplayerid, style)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(styleplayerid)) return 0;
	new slot = Asf_OvrFind(playerid, styleplayerid, true);
	if(slot == -1) return 0;
	gAsfOvr[slot][aoFlags] |= ASF_O_STYLE;
	gAsfOvr[slot][aoStyle] = style;
	Asf_SendStyle(styleplayerid, playerid, style);
	return 1;
}

stock GetPlayerFightStyleForPlayer(playerid, styleplayerid)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(styleplayerid)) return 0;
	return Asf_RealStyle(styleplayerid, playerid);
}

stock ResetPlayerFightStyleForPlayer(playerid, styleplayerid)
{
	new slot = Asf_Ovr(playerid, styleplayerid, ASF_O_STYLE);
	if(slot == -1) return 0;
	Asf_OvrDrop(slot, ASF_O_STYLE);
	Asf_SendStyle(styleplayerid, playerid, _:GetPlayerFightingStyle(styleplayerid));
	return 1;
}

// the position stays until ResetPlayerPosForPlayer (every on-foot sync of posplayerid is rewritten for playerid)
stock SetPlayerPosForPlayer(playerid, posplayerid, Float:fX, Float:fY, Float:fZ, bool:forcesync = true)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(posplayerid)) return 0;
	new slot = Asf_OvrFind(playerid, posplayerid, true);
	if(slot == -1) return 0;
	gAsfOvr[slot][aoFlags] |= ASF_O_POS;
	gAsfOvr[slot][aoPos][0] = fX;
	gAsfOvr[slot][aoPos][1] = fY;
	gAsfOvr[slot][aoPos][2] = fZ;
	if(forcesync) Asf_SendSync(posplayerid, playerid, ASF_SYNC_ONFOOT);
	return 1;
}

stock ResetPlayerPosForPlayer(playerid, posplayerid)
{
	new slot = Asf_Ovr(playerid, posplayerid, ASF_O_POS);
	if(slot == -1) return 0;
	Asf_OvrDrop(slot, ASF_O_POS);
	Asf_SendSync(posplayerid, playerid, ASF_SYNC_ONFOOT);
	return 1;
}

stock SetPlayerRotationQuatForPlayer(playerid, quatplayerid, Float:w, Float:x, Float:y, Float:z, bool:forcesync = true)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(quatplayerid)) return 0;
	new slot = Asf_OvrFind(playerid, quatplayerid, true);
	if(slot == -1) return 0;
	gAsfOvr[slot][aoFlags] |= ASF_O_QUAT;
	gAsfOvr[slot][aoQuat][0] = w;
	gAsfOvr[slot][aoQuat][1] = x;
	gAsfOvr[slot][aoQuat][2] = y;
	gAsfOvr[slot][aoQuat][3] = z;
	if(forcesync) Asf_SendSync(quatplayerid, playerid, ASF_SYNC_ONFOOT);
	return 1;
}

stock ResetPlayerRotationQuatForPlayer(playerid, quatplayerid)
{
	new slot = Asf_Ovr(playerid, quatplayerid, ASF_O_QUAT);
	if(slot == -1) return 0;
	Asf_OvrDrop(slot, ASF_O_QUAT);
	Asf_SendSync(quatplayerid, playerid, ASF_SYNC_ONFOOT);
	return 1;
}

stock ApplyAnimationForPlayer(playerid, animplayerid, const animlib[], const animname[], Float:fDelta, loop, lockx, locky, freeze, time)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(animplayerid)) return 0;
	new lib[64], name[64];
	strcat(lib, animlib);
	strcat(name, animname);
	new BitStream:bs = BS_New();
	BS_WriteValue(bs, PR_UINT16, animplayerid, PR_STRING8, lib, PR_STRING8, name, PR_FLOAT, fDelta,
		PR_BOOL, loop != 0, PR_BOOL, lockx != 0, PR_BOOL, locky != 0, PR_BOOL, freeze != 0, PR_UINT32, time);
	Asf_Send(bs, playerid, Asf_R_ANIM);
	return 1;
}

stock SetPlayerChatBubbleForPlayer(forplayerid, playerid, const text[], color, Float:drawdistance, expiretime)
{
	if(!IsPlayerConnected(forplayerid) || !IsPlayerConnected(playerid)) return 0;
	new buf[145];
	strcat(buf, text);
	new BitStream:bs = BS_New();
	BS_WriteValue(bs, PR_UINT16, playerid, PR_UINT32, color, PR_FLOAT, drawdistance, PR_UINT32, expiretime, PR_STRING8, buf);
	Asf_Send(bs, forplayerid, Asf_R_BUBBLE);
	return 1;
}

stock ResetPlayerMarkerForPlayer(playerid, resetplayerid)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(resetplayerid)) return 0;
	new BitStream:bs = BS_New();
	BS_WriteValue(bs, PR_UINT16, resetplayerid, PR_UINT32, GetPlayerColor(resetplayerid));
	Asf_Send(bs, playerid, Asf_R_COLOR);
	return 1;
}

stock HidePlayerForPlayer(forplayerid, playerid)
{
	if(forplayerid == playerid || !IsPlayerConnected(forplayerid) || !IsPlayerConnected(playerid)) return 0;
	Asf_BitSet(gAsfHidePl[forplayerid], playerid);
	new BitStream:bs = BS_New();
	BS_WriteValue(bs, PR_UINT16, playerid);
	Asf_Send(bs, forplayerid, Asf_R_REMOVE);
	return 1;
}

stock ShowPlayerForPlayer(forplayerid, playerid, bool:setskin = false)
{
	#pragma unused setskin
	if(!IsPlayerConnected(forplayerid) || !IsPlayerConnected(playerid)) return 0;
	if(!Asf_BitGet(gAsfHidePl[forplayerid], playerid)) return 0;
	Asf_BitClr(gAsfHidePl[forplayerid], playerid);
	if(IsPlayerStreamedIn(playerid, forplayerid)) Asf_SendAdd(playerid, forplayerid);
	return 1;
}

stock IsPlayerHiddenForPlayer(forplayerid, playerid)
{
	if(forplayerid < 0 || forplayerid >= MAX_PLAYERS || playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	return Asf_BitGet(gAsfHidePl[forplayerid], playerid) ? 1 : 0;
}

stock AddPlayerForPlayer(forplayerid, playerid, isnpc = 0)
{
	if(!IsPlayerConnected(forplayerid) || !IsPlayerConnected(playerid)) return 0;
	new name[MAX_PLAYER_NAME + 1];
	GetPlayerName(playerid, name);
	new BitStream:bs = BS_New();
	BS_WriteValue(bs, PR_UINT16, playerid, PR_INT32, GetPlayerColor(playerid), PR_UINT8, isnpc ? 1 : 0, PR_STRING8, name);
	Asf_Send(bs, forplayerid, Asf_R_JOIN);
	return 1;
}

stock RemovePlayerForPlayer(forplayerid, playerid)
{
	if(!IsPlayerConnected(forplayerid)) return 0;
	new BitStream:bs = BS_New();
	BS_WriteValue(bs, PR_UINT16, playerid, PR_UINT8, 1);
	Asf_Send(bs, forplayerid, Asf_R_QUIT);
	return 1;
}

stock SpawnForWorld(playerid)
{
	if(!IsPlayerConnected(playerid)) return 0;
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(i == playerid || !IsPlayerConnected(i) || !IsPlayerStreamedIn(playerid, i) || Asf_BitGet(gAsfHidePl[i], playerid)) continue;
		Asf_SendAdd(playerid, i);
	}
	return 1;
}

stock SendPlayerDeath(playerid, forplayerid = -1)
{
	if(!IsPlayerConnected(playerid)) return 0;
	new BitStream:bs = BS_New();
	BS_WriteValue(bs, PR_UINT16, playerid);
	if(forplayerid != -1) Asf_Send(bs, forplayerid, Asf_R_DEATH);
	else
	{
		gAsfSelf = true;
		PR_SendRPCToPlayerStream(bs, playerid, Asf_R_DEATH);
		gAsfSelf = false;
		BS_Delete(bs);
	}
	return 1;
}

stock BroadcastDeath(playerid)
{
	return SendPlayerDeath(playerid, -1);
}

stock SendBulletData(senderid, forplayerid = -1, weaponid, hittype, hitid, Float:fHitOriginX, Float:fHitOriginY, Float:fHitOriginZ, Float:fHitTargetX, Float:fHitTargetY, Float:fHitTargetZ, Float:fCenterOfHitX, Float:fCenterOfHitY, Float:fCenterOfHitZ)
{
	if(!IsPlayerConnected(senderid)) return 0;
	new data[PR_BulletSync];
	data[PR_hitType] = hittype;
	data[PR_hitId] = hitid;
	data[PR_origin][0] = fHitOriginX;
	data[PR_origin][1] = fHitOriginY;
	data[PR_origin][2] = fHitOriginZ;
	data[PR_hitPos][0] = fHitTargetX;
	data[PR_hitPos][1] = fHitTargetY;
	data[PR_hitPos][2] = fHitTargetZ;
	data[PR_offsets][0] = fCenterOfHitX;
	data[PR_offsets][1] = fCenterOfHitY;
	data[PR_offsets][2] = fCenterOfHitZ;
	data[PR_weaponId] = weaponid;
	new BitStream:bs = BS_New();
	BS_WriteValue(bs, PR_UINT8, Asf_P_BULLET, PR_UINT16, senderid);
	BS_WriteBulletSync(bs, data);
	gAsfSelf = true;
	if(forplayerid != -1) PR_SendPacket(bs, forplayerid);
	else PR_SendPacketToPlayerStream(bs, senderid);
	gAsfSelf = false;
	BS_Delete(bs);
	return 1;
}

/* ---------------------------------------------------------------- sync */

stock SetPlayerDisabledKeysSync(playerid, keys, updown = 0, leftright = 0)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfPl[playerid][apDisKeys] = keys;
	gAsfPl[playerid][apDisUD] = updown;
	gAsfPl[playerid][apDisLR] = leftright;
	return 1;
}

stock GetPlayerDisabledKeysSync(playerid, &keys, &updown = 0, &leftright = 0)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	keys = gAsfPl[playerid][apDisKeys];
	updown = gAsfPl[playerid][apDisUD];
	leftright = gAsfPl[playerid][apDisLR];
	return 1;
}

stock GetPlayerLastSyncedVehicleID(playerid)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return INVALID_VEHICLE_ID;
	return gAsfPl[playerid][apLastVeh];
}

stock GetPlayerLastSyncedTrailerID(playerid)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return INVALID_VEHICLE_ID;
	return gAsfPl[playerid][apLastTrailer];
}

stock GetPlayerSyncWeapon(playerid)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	switch(GetPlayerState(playerid))
	{
		case PLAYER_STATE_DRIVER: return gAsfCar[playerid][PR_weaponId];
		case PLAYER_STATE_PASSENGER: return gAsfPass[playerid][PR_weaponId];
	}
	return gAsfFoot[playerid][PR_weaponId];
}

stock SetPlayerSyncKeys(playerid, keys, updown, leftright)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfFoot[playerid][PR_keys] = keys, gAsfFoot[playerid][PR_udKey] = updown, gAsfFoot[playerid][PR_lrKey] = leftright;
	gAsfCar[playerid][PR_keys] = keys, gAsfCar[playerid][PR_udKey] = updown, gAsfCar[playerid][PR_lrKey] = leftright;
	gAsfPass[playerid][PR_keys] = keys, gAsfPass[playerid][PR_udKey] = updown, gAsfPass[playerid][PR_lrKey] = leftright;
	return 1;
}

stock SetPlayerSyncPos(playerid, Float:x, Float:y, Float:z)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfFoot[playerid][PR_position][0] = x, gAsfFoot[playerid][PR_position][1] = y, gAsfFoot[playerid][PR_position][2] = z;
	gAsfPass[playerid][PR_position][0] = x, gAsfPass[playerid][PR_position][1] = y, gAsfPass[playerid][PR_position][2] = z;
	return 1;
}

stock SetPlayerSyncPosition(playerid, Float:x, Float:y, Float:z)
{
	return SetPlayerSyncPos(playerid, x, y, z);
}

stock SetPlayerSyncVelocity(playerid, Float:x, Float:y, Float:z)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfFoot[playerid][PR_velocity][0] = x, gAsfFoot[playerid][PR_velocity][1] = y, gAsfFoot[playerid][PR_velocity][2] = z;
	return 1;
}

stock SetPlayerSyncRotationQuat(playerid, Float:w, Float:x, Float:y, Float:z)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfFoot[playerid][PR_quaternion][0] = w, gAsfFoot[playerid][PR_quaternion][1] = x;
	gAsfFoot[playerid][PR_quaternion][2] = y, gAsfFoot[playerid][PR_quaternion][3] = z;
	return 1;
}

stock SetPlayerSyncCameraFrontVector(playerid, Float:x, Float:y, Float:z)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfAim[playerid][PR_camFrontVec][0] = x, gAsfAim[playerid][PR_camFrontVec][1] = y, gAsfAim[playerid][PR_camFrontVec][2] = z;
	return 1;
}

stock SetPlayerSyncCameraPos(playerid, Float:x, Float:y, Float:z)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfAim[playerid][PR_camPos][0] = x, gAsfAim[playerid][PR_camPos][1] = y, gAsfAim[playerid][PR_camPos][2] = z;
	return 1;
}

stock SetPlayerSyncCameraMode(playerid, mode)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfAim[playerid][PR_camMode] = mode;
	return 1;
}

stock SetPlayerSyncWeapon(playerid, weaponid)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfFoot[playerid][PR_weaponId] = weaponid;
	gAsfCar[playerid][PR_weaponId] = weaponid;
	gAsfPass[playerid][PR_weaponId] = weaponid;
	return 1;
}

stock SetPlayerSyncWeaponState(playerid, weaponstate)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfAim[playerid][PR_weaponState] = weaponstate;
	return 1;
}

stock SetPlayerSyncSpecialAction(playerid, actionid)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfFoot[playerid][PR_specialAction] = actionid;
	return 1;
}

stock SetPlayerSyncHealth(playerid, Float:health)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	new hp = floatround(health);
	gAsfFoot[playerid][PR_health] = hp;
	gAsfCar[playerid][PR_playerHealth] = hp;
	gAsfPass[playerid][PR_playerHealth] = hp;
	return 1;
}

stock SetPlayerSyncArmour(playerid, Float:armour)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	new ar = floatround(armour);
	gAsfFoot[playerid][PR_armour] = ar;
	gAsfCar[playerid][PR_armour] = ar;
	gAsfPass[playerid][PR_playerArmour] = ar;
	return 1;
}

stock SetPlayerSyncVehicleId(playerid, vehicleid, bool:setstate = false)
{
	#pragma unused setstate
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfCar[playerid][PR_vehicleId] = vehicleid;
	gAsfPass[playerid][PR_vehicleId] = vehicleid;
	return 1;
}

stock SetPlayerSyncVehicleSeat(playerid, seat)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfPass[playerid][PR_seatId] = seat;
	return 1;
}

stock SetPlayerSyncVehiclePosition(playerid, Float:x, Float:y, Float:z)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfCar[playerid][PR_position][0] = x, gAsfCar[playerid][PR_position][1] = y, gAsfCar[playerid][PR_position][2] = z;
	return 1;
}

stock SetPlayerSyncVehicleVelocity(playerid, Float:x, Float:y, Float:z)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfCar[playerid][PR_velocity][0] = x, gAsfCar[playerid][PR_velocity][1] = y, gAsfCar[playerid][PR_velocity][2] = z;
	return 1;
}

stock SetPlayerSyncVehicleRotQuat(playerid, Float:w, Float:x, Float:y, Float:z)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfCar[playerid][PR_quaternion][0] = w, gAsfCar[playerid][PR_quaternion][1] = x;
	gAsfCar[playerid][PR_quaternion][2] = y, gAsfCar[playerid][PR_quaternion][3] = z;
	return 1;
}

stock SetPlayerSyncVehicleHealth(playerid, Float:health)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfCar[playerid][PR_vehicleHealth] = health;
	return 1;
}

stock SetPlayerSyncTrainSpeed(playerid, Float:speed)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfCar[playerid][PR_trainSpeed] = speed;
	return 1;
}

stock UpdatePlayerSyncData(playerid, type = -1, bool:setstate = false)
{
	#pragma unused setstate
	if(!IsPlayerConnected(playerid)) return 0;
	if(type == ASF_SYNC_AUTO)
	{
		switch(GetPlayerState(playerid))
		{
			case PLAYER_STATE_DRIVER: type = ASF_SYNC_INCAR;
			case PLAYER_STATE_PASSENGER: type = ASF_SYNC_PASSENGER;
			default: type = ASF_SYNC_ONFOOT;
		}
	}
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(i == playerid || !IsPlayerConnected(i) || !IsPlayerStreamedIn(playerid, i) || Asf_BitGet(gAsfHidePl[i], playerid)) continue;
		Asf_SendSync(playerid, i, type);
	}
	return 1;
}

/* ---------------------------------------------------------------- objects / vehicles / pickups */

stock HideObjectForPlayer(playerid, objectid)
{
	if(!IsPlayerConnected(playerid) || objectid < 0 || objectid >= MAX_OBJECTS || !IsValidObject(objectid)) return 0;
	if(Asf_BitGet(gAsfHideObj[playerid], objectid)) return 1;
	Asf_BitSet(gAsfHideObj[playerid], objectid);
	new BitStream:bs = BS_New();
	BS_WriteValue(bs, PR_UINT16, objectid);
	Asf_Send(bs, playerid, Asf_R_ODESTROY);
	return 1;
}

stock ShowObjectForPlayer(playerid, objectid)
{
	if(!IsPlayerConnected(playerid) || objectid < 0 || objectid >= MAX_OBJECTS || !IsValidObject(objectid)) return 0;
	if(!Asf_BitGet(gAsfHideObj[playerid], objectid)) return 0;
	Asf_BitClr(gAsfHideObj[playerid], objectid);
	if(gAsfObjBS[objectid] == BitStream:0) return 0;
	Asf_Send(gAsfObjBS[objectid], playerid, Asf_R_OCREATE, false);
	for(new i = 0; i < 16; i++)
	{
		if(gAsfObjMat[objectid][i] != BitStream:0) Asf_Send(gAsfObjMat[objectid][i], playerid, Asf_R_OMAT, false);
	}
	new Float:x, Float:y, Float:z, BitStream:bs;
	GetObjectPos(objectid, x, y, z);
	bs = BS_New();
	BS_WriteValue(bs, PR_UINT16, objectid, PR_FLOAT, x, PR_FLOAT, y, PR_FLOAT, z);
	Asf_Send(bs, playerid, Asf_R_OPOS);
	GetObjectRot(objectid, x, y, z);
	bs = BS_New();
	BS_WriteValue(bs, PR_UINT16, objectid, PR_FLOAT, x, PR_FLOAT, y, PR_FLOAT, z);
	Asf_Send(bs, playerid, Asf_R_OROT);
	return 1;
}

stock HideNewObjectsForPlayer(playerid, bool:toggle)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfHideNewObj[playerid] = toggle;
	return 1;
}

stock IsObjectHiddenForPlayer(playerid, objectid)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS || objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	return Asf_BitGet(gAsfHideObj[playerid], objectid) ? 1 : 0;
}

stock NewObjectsHiddenForPlayer(playerid)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	return gAsfHideNewObj[playerid] ? 1 : 0;
}

stock HideVehicle(vehicleid)
{
	if(vehicleid <= 0 || vehicleid >= MAX_VEHICLES || !IsValidVehicle(vehicleid)) return 0;
	if(Asf_BitGet(gAsfHideVeh, vehicleid)) return 1;
	Asf_BitSet(gAsfHideVeh, vehicleid);
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(!IsPlayerConnected(i) || !IsVehicleStreamedIn(vehicleid, i)) continue;
		new BitStream:bs = BS_New();
		BS_WriteValue(bs, PR_UINT16, vehicleid);
		Asf_Send(bs, i, Asf_R_VREMOVE);
	}
	return 1;
}

stock ShowVehicle(vehicleid)
{
	if(vehicleid <= 0 || vehicleid >= MAX_VEHICLES || !IsValidVehicle(vehicleid)) return 0;
	if(!Asf_BitGet(gAsfHideVeh, vehicleid)) return 0;
	Asf_BitClr(gAsfHideVeh, vehicleid);
	if(gAsfVehBS[vehicleid] == BitStream:0) return 1;
	new Float:x, Float:y, Float:z, Float:a, Float:hp;
	GetVehiclePos(vehicleid, x, y, z);
	GetVehicleZAngle(vehicleid, a);
	GetVehicleHealth(vehicleid, hp);
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(!IsPlayerConnected(i) || !IsVehicleStreamedIn(vehicleid, i)) continue;
		Asf_Send(gAsfVehBS[vehicleid], i, Asf_R_VADD, false);
		new BitStream:bs = BS_New();
		BS_WriteValue(bs, PR_UINT16, vehicleid, PR_FLOAT, x, PR_FLOAT, y, PR_FLOAT, z);
		Asf_Send(bs, i, Asf_R_VPOS);
		bs = BS_New();
		BS_WriteValue(bs, PR_UINT16, vehicleid, PR_FLOAT, a);
		Asf_Send(bs, i, Asf_R_VANGLE);
		bs = BS_New();
		BS_WriteValue(bs, PR_UINT16, vehicleid, PR_FLOAT, hp);
		Asf_Send(bs, i, Asf_R_VHEALTH);
	}
	return 1;
}

stock IsVehicleHidden(vehicleid)
{
	if(vehicleid <= 0 || vehicleid >= MAX_VEHICLES) return 0;
	return Asf_BitGet(gAsfHideVeh, vehicleid) ? 1 : 0;
}

stock SetPickupForPlayer(playerid, id, model, type, Float:X, Float:Y, Float:Z)
{
	if(!IsPlayerConnected(playerid) || !IsValidPickup(id)) return 0;
	new slot = -1;
	for(new i = 0; i < ASF_MAX_PICKUP_OVR; i++)
	{
		if(gAsfPk[i][akPlayer] == playerid + 1 && gAsfPk[i][akId] == id) { slot = i; break; }
		if(slot == -1 && !gAsfPk[i][akPlayer]) slot = i;
	}
	if(slot == -1) return 0;
	new BitStream:bs = BS_New();
	BS_WriteValue(bs, PR_INT32, id);
	Asf_Send(bs, playerid, Asf_R_PDESTROY);
	gAsfPk[slot][akPlayer] = playerid + 1;
	gAsfPk[slot][akId] = id;
	gAsfPk[slot][akModel] = model;
	gAsfPk[slot][akType] = type;
	gAsfPk[slot][akPos][0] = X;
	gAsfPk[slot][akPos][1] = Y;
	gAsfPk[slot][akPos][2] = Z;
	bs = BS_New();
	BS_WriteValue(bs, PR_INT32, id, PR_INT32, model, PR_INT32, type, PR_FLOAT, X, PR_FLOAT, Y, PR_FLOAT, Z);
	Asf_Send(bs, playerid, Asf_R_PCREATE);
	return 1;
}

/* ---------------------------------------------------------------- network / server */

#if !defined ASF_NO_EXCLUSIVE_BROADCAST
	stock SetExclusiveBroadcast(toggle)
	{
		gAsfExclusive = (toggle != 0);
		if(!gAsfExclusive) for(new i = 0; i < ASF_PBITS; i++) gAsfBcast[i] = 0;
		return 1;
	}

	stock BroadcastToPlayer(playerid, toggle = 1)
	{
		if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
		if(toggle) Asf_BitSet(gAsfBcast, playerid);
		else Asf_BitClr(gAsfBcast, playerid);
		return 1;
	}
#endif

stock TogglePlayerScoresPingsUpdate(playerid, bool:toggle)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfPl[playerid][apNoScores] = !toggle;
	return 1;
}

stock TogglePlayerFakePing(playerid, bool:toggle)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if(gAsfPl[playerid][apFakePingOn] == toggle) return 1;
	gAsfPl[playerid][apFakePingOn] = toggle;
	gAsfFakePingCount += toggle ? 1 : -1;
	return 1;
}

stock SetPlayerFakePing(playerid, ping)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	gAsfPl[playerid][apFakePing] = ping;
	return 1;
}

// one BS_* type + value pair; returns how many arguments it used (0 = unknown type)
// BS_STRING writes the characters only: write the length first yourself (BS_UNSIGNEDCHAR, len) when the RPC needs it
static stock Asf_WriteArg(BitStream:bs, type, value)
{
	switch(type)
	{
		case BS_BOOL: BS_WriteValue(bs, PR_BOOL, value != 0);
		case BS_CHAR: BS_WriteValue(bs, PR_INT8, value);
		case BS_UNSIGNEDCHAR: BS_WriteValue(bs, PR_UINT8, value);
		case BS_SHORT: BS_WriteValue(bs, PR_INT16, value);
		case BS_UNSIGNEDSHORT: BS_WriteValue(bs, PR_UINT16, value);
		case BS_INT: BS_WriteValue(bs, PR_INT32, value);
		case BS_UNSIGNEDINT: BS_WriteValue(bs, PR_UINT32, value);
		case BS_FLOAT: BS_WriteValue(bs, PR_FLOAT, value);
		default: return 0;
	}
	return 1;
}

stock SendRPC(playerid, RPC, {Float,_}:...)
{
	new BitStream:bs = BS_New(), type, c;
	for(new i = 2, n = numargs(); i + 1 < n; i += 2)
	{
		type = getarg(i);
		if(type == BS_STRING)
		{
			for(new j = 0; (c = getarg(i + 1, j)) != EOS; j++) BS_WriteValue(bs, PR_UINT8, c);
		}
		else if(!Asf_WriteArg(bs, type, getarg(i + 1))) break;
	}
	gAsfSelf = true;
	if(playerid != -1) PR_SendRPC(bs, playerid, RPC);
	else for(new i = 0; i < MAX_PLAYERS; i++) if(IsPlayerConnected(i) && !IsPlayerNPC(i)) PR_SendRPC(bs, i, RPC);
	gAsfSelf = false;
	BS_Delete(bs);
	return 1;
}

stock SendData(playerid, {Float,_}:...)
{
	new BitStream:bs = BS_New(), type, c;
	for(new i = 1, n = numargs(); i + 1 < n; i += 2)
	{
		type = getarg(i);
		if(type == BS_STRING)
		{
			for(new j = 0; (c = getarg(i + 1, j)) != EOS; j++) BS_WriteValue(bs, PR_UINT8, c);
		}
		else if(!Asf_WriteArg(bs, type, getarg(i + 1))) break;
	}
	gAsfSelf = true;
	if(playerid != -1) PR_SendPacket(bs, playerid);
	else for(new i = 0; i < MAX_PLAYERS; i++) if(IsPlayerConnected(i) && !IsPlayerNPC(i)) PR_SendPacket(bs, i);
	gAsfSelf = false;
	BS_Delete(bs);
	return 1;
}

stock LoadFilterScript(const scriptname[])
{
	SendRconCommand("loadfs %s", scriptname);
	return 1;
}

stock UnLoadFilterScript(const scriptname[])
{
	SendRconCommand("unloadfs %s", scriptname);
	return 1;
}

/* ---------------------------------------------------------------- pause (when pause.inc is not used) */

#if !defined pause_included
	stock IsPlayerPaused(playerid)
	{
		if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
		return gAsfPl[playerid][apPaused] ? 1 : 0;
	}

	stock GetPlayerPausedTime(playerid)
	{
		if(playerid < 0 || playerid >= MAX_PLAYERS || !gAsfPl[playerid][apPaused]) return 0;
		return GetTickCount() - gAsfPl[playerid][apPausedAt];
	}

	stock YSF_SetAFKAccuracy(time_ms)
	{
		gAsfAfkAcc = time_ms;
		return 1;
	}

	stock YSF_GetAFKAccuracy()
	{
		return gAsfAfkAcc;
	}
#endif
