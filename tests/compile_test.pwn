// ASF compile test - calls every function once. By noiseXYZ, https://github.com/noiseXYZ/openmp-asf
#include <open.mp>
#include <Pawn.RakNet>
#if defined TEST_PAUSE
	#include <pause>
#endif
#include <asf>

main() {}

public OnPlayerConnect(playerid)
{
	return 1;
}

public OnOutgoingRPC(playerid, rpcid, BitStream:bs)
{
	return 1;
}

public OnPlayerStatsAndWeaponsUpdate(playerid) return 1;
public OnOutcomeScmEvent(playerid, issuerid, E_SCM_EVENT_TYPE:eventid, vehicleid, arg1, arg2) return 1;
#if !defined TEST_PAUSE
public OnPlayerPauseStateChange(playerid, pausestate) return 1;
#endif

public OnGameModeInit()
{
	new p = 0, q = 1, s[25], k, u, l;
	SetPlayerTeamForPlayer(p, q, 1); GetPlayerTeamForPlayer(p, q); ResetPlayerTeamForPlayer(p, q);
	SetPlayerSkinForPlayer(p, q, 1); GetPlayerSkinForPlayer(p, q); ResetPlayerSkinForPlayer(p, q);
	SetPlayerNameForPlayer(p, q, "abc"); GetPlayerNameForPlayer(p, q, s); ResetPlayerNameForPlayer(p, q);
	SetPlayerFightStyleForPlayer(p, q, 4); GetPlayerFightStyleForPlayer(p, q); ResetPlayerFightStyleForPlayer(p, q);
	SetPlayerPosForPlayer(p, q, 1.0, 2.0, 3.0); ResetPlayerPosForPlayer(p, q);
	SetPlayerRotationQuatForPlayer(p, q, 1.0, 0.0, 0.0, 0.0); ResetPlayerRotationQuatForPlayer(p, q);
	ApplyAnimationForPlayer(p, q, "PED", "WALK", 4.1, 0, 0, 0, 0, 0);
	SetPlayerChatBubbleForPlayer(p, q, "hi", -1, 20.0, 1000);
	ResetPlayerMarkerForPlayer(p, q);
	HidePlayerForPlayer(p, q); ShowPlayerForPlayer(p, q); IsPlayerHiddenForPlayer(p, q);
	AddPlayerForPlayer(p, q); RemovePlayerForPlayer(p, q); SpawnForWorld(p);
	SendPlayerDeath(p); BroadcastDeath(p);
	SendBulletData(p, -1, 24, 0, 0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0);
	SetPlayerDisabledKeysSync(p, 4); GetPlayerDisabledKeysSync(p, k, u, l);
	GetPlayerLastSyncedVehicleID(p); GetPlayerLastSyncedTrailerID(p); GetPlayerSyncWeapon(p);
	SetPlayerSyncKeys(p, 0, 0, 0); SetPlayerSyncPos(p, 0.0, 0.0, 0.0); SetPlayerSyncPosition(p, 0.0, 0.0, 0.0);
	SetPlayerSyncVelocity(p, 0.0, 0.0, 0.0); SetPlayerSyncRotationQuat(p, 1.0, 0.0, 0.0, 0.0);
	SetPlayerSyncCameraFrontVector(p, 0.0, 0.0, 0.0); SetPlayerSyncCameraPos(p, 0.0, 0.0, 0.0); SetPlayerSyncCameraMode(p, 4);
	SetPlayerSyncWeapon(p, 24); SetPlayerSyncWeaponState(p, 2); SetPlayerSyncSpecialAction(p, 0);
	SetPlayerSyncHealth(p, 50.0); SetPlayerSyncArmour(p, 50.0); SetPlayerSyncVehicleId(p, 1); SetPlayerSyncVehicleSeat(p, 1);
	SetPlayerSyncVehiclePosition(p, 0.0, 0.0, 0.0); SetPlayerSyncVehicleVelocity(p, 0.0, 0.0, 0.0);
	SetPlayerSyncVehicleRotQuat(p, 1.0, 0.0, 0.0, 0.0); SetPlayerSyncVehicleHealth(p, 1000.0); SetPlayerSyncTrainSpeed(p, 0.0);
	UpdatePlayerSyncData(p);
	HideObjectForPlayer(p, 1); ShowObjectForPlayer(p, 1); HideNewObjectsForPlayer(p, true); IsObjectHiddenForPlayer(p, 1); NewObjectsHiddenForPlayer(p);
	HideVehicle(1); ShowVehicle(1); IsVehicleHidden(1);
	SetPickupForPlayer(p, 1, 1239, 1, 0.0, 0.0, 0.0);
	IsBuildingRemovedForPlayer(p, 1, 0.0, 0.0, 0.0, 1.0); RemoveBuildingForPlayer(p, 1, 0.0, 0.0, 0.0, 1.0);
	SetExclusiveBroadcast(1); BroadcastToPlayer(p); SetExclusiveBroadcast(0);
	TogglePlayerScoresPingsUpdate(p, false); TogglePlayerFakePing(p, true); SetPlayerFakePing(p, 5);
	SendRPC(p, 95, BS_INT, 1, BS_FLOAT, 1.0, BS_UNSIGNEDCHAR, 3, BS_STRING, "abc", BS_BOOL, true);
	SendData(-1, BS_UNSIGNEDCHAR, 200, BS_SHORT, 5);
	LoadFilterScript("x"); UnLoadFilterScript("x");
	IsPlayerPaused(p); GetPlayerPausedTime(p);
	#if !defined TEST_PAUSE
	YSF_SetAFKAccuracy(2000); YSF_GetAFKAccuracy();
	#endif
	return 1;
}
