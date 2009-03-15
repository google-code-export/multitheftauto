#pragma message("Compiling precompiled header.\n")

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <mmsystem.h>
#include <winsock.h>
#include <pthread.h>

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <ctime>

// SDK includes
#include <xml/CXML.h>
#include <xml/CXMLNode.h>
#include <xml/CXMLFile.h>
#include <xml/CXMLAttributes.h>
#include <xml/CXMLAttribute.h>

// Packet includes
#include "packets/CUnoccupiedVehicleSyncPacket.h"
#include "packets/CUnoccupiedVehicleStopSyncPacket.h"
#include "packets/CUnoccupiedVehicleStartSyncPacket.h"
#include "packets/CServerTextItemPacket.h"
#include "packets/CReturnSyncPacket.h"
#include "packets/CProjectileSyncPacket.h"
#include "packets/CPlayerWastedPacket.h"
#include "packets/CPlayerTimeoutPacket.h"
#include "packets/CPlayerStatsPacket.h"
#include "packets/CPlayerSpawnPacket.h"
#include "packets/CPlayerQuitPacket.h"
#include "packets/CPlayerPuresyncPacket.h"
#include "packets/CPlayerListPacket.h"
#include "packets/CPlayerPingsPacket.h"
#include "packets/CPlayerJoinPacket.h"
#include "packets/CPlayerJoinDataPacket.h"
#include "packets/CPlayerJoinCompletePacket.h"
#include "packets/CPlayerDisconnectedPacket.h"
#include "packets/CPlayerConnectCompletePacket.h"
#include "packets/CPlayerClothesPacket.h"
#include "packets/CPlayerChangeNickPacket.h"
#include "packets/CPickupHitConfirmPacket.h"
#include "packets/CPickupHideShowPacket.h"
#include "packets/CPedSyncPacket.h"
#include "packets/CPedStopSyncPacket.h"
#include "packets/CPedStartSyncPacket.h"
#include "packets/CPacket.h"
#include "packets/CMapInfoPacket.h"
#include "packets/CLuaPacket.h"
#include "packets/CLuaEventPacket.h"
#include "packets/CKeysyncPacket.h"
#include "packets/CFireSyncPacket.h"
#include "packets/CExplosionSyncPacket.h"
#include "packets/CEntityRemovePacket.h"
#include "packets/CDetonateSatchelsPacket.h"
#include "packets/CDebugEchoPacket.h"
#include "packets/CCustomDataPacket.h"
#include "packets/CConsoleEchoPacket.h"
#include "packets/CCommandPacket.h"
#include "packets/CChatEchoPacket.h"
#include "packets/CCameraSyncPacket.h"
#include "packets/PacketIODeclarators.h"
#include "packets/CVoiceDataPacket.h"
#include "packets/CVehicleSpawnPacket.h"
#include "packets/CVehiclePuresyncPacket.h"
#include "packets/CVehicleInOutPacket.h"
#include "packets/CVehicleDamageSyncPacket.h"
#include "packets/CVehicleTrailerPacket.h"
#include "packets/CEntityAddPacket.h"
#include "packets/CResourceStopPacket.h"
#include "packets/CResourceStartPacket.h"

// Lua function definition includes
#include "luadefs/CLuaACLDefs.h"
#include "luadefs/CLuaCameraDefs.h"
#include "luadefs/CLuaXMLDefs.h"
#include "luadefs/CLuaWorldDefs.h"
#include "luadefs/CLuaTextDefs.h"
#include "luadefs/CLuaResourceDefs.h"
#include "luadefs/CLuaPickupDefs.h"
#include "luadefs/CLuaHTTPDefs.h"
#include "luadefs/CLuaHandlingDefs.h"
#include "luadefs/CLuaFileDefs.h"
#include "luadefs/CLuaElementDefs.h"
#include "luadefs/CLuaDefs.h"

// Lua includes
#include "lua/LuaCommon.h"
#include "lua/CLuaMain.h"
#include "lua/CLuaManager.h"
#include "lua/CLuaTimerManager.h"
#include "lua/CLuaTimer.h"
#include "lua/CLuaFunctionDefinitions.h"
#include "lua/CLuaModuleManager.h"
#include "lua/CLuaArgument.h"
#include "lua/CLuaCFunctions.h"
#include "lua/CLuaArguments.h"

// Logic includes
#include "ASEQuerySDK.h"
#include "CBanManager.h"
#include "ASE.h"
#include "CAccessControlList.h"
#include "CAccessControlListGroup.h"
#include "CAccessControlListManager.h"
#include "CAccessControlListRight.h"
#include "CAccountManager.h"
#include "CBan.h"
#include "CBlip.h"
#include "CTeam.h"
#include "CPickup.h"
#include "CVehicleNames.h"
#include "CMarkerManager.h"
#include "CKeyBinds.h"
#include "CWhoWas.h"
#include "CTextDisplay.h"
#include "CPlayerStats.h"
#include "CConsoleCommands.h"
#include "CPlayerManager.h"
#include "CResourceHTMLItem.h"
#include "CPlayerTextManager.h"
#include "CResourceMapItem.h"
#include "CScriptFile.h"
#include "CColManager.h"
#include "CVehicleManager.h"
#include "CConnectHistory.h"
#include "CBlendedWeather.h"
#include "CPlayerClothes.h"
#include "CRegisteredCommands.h"
#include "CMarker.h"
#include "CHTTPD.h"
#include "CResourceFile.h"
#include "CColCallback.h"
#include "CClock.h"
#include "CColShape.h"
#include "CZoneNames.h"
#include "CConsoleCommand.h"
#include "CColCuboid.h"
#include "CCommandLineParser.h"
#include "CEvents.h"
#include "CStack.h"
#include "CGame.h"
#include "CRemoteCalls.h"
#include "CHandlingManager.h"
#include "CGroups.h"
#include "CScriptDebugging.h"
#include "CPacketTranslator.h"
#include "CBlipManager.h"
#include "CTeamManager.h"
#include "CResourceConfigItem.h"
#include "CConsoleClient.h"
#include "CPedSync.h"
#include "CCustomData.h"
#include "CObject.h"
#include "CPed.h"
#include "CMapFile.h"
#include "CResourceManager.h"
#include "CElementIDs.h"
#include "CSettings.h"
#include "CConsole.h"
#include "CPlayerTasks.h"
#include "CMapEventManager.h"
#include "CVehicleColor.h"
#include "CMD5Hasher.h"
#include "CCommon.h"
#include "CColCircle.h"
#include "CResourceClientFileItem.h"
#include "CVehicleDefaultColors.h"
#include "CSerialManager.h"
#include "CVehicleUpgrades.h"
#include "CRPCFunctions.h"
#include "CRadarAreaManager.h"
#include "CHandling.h"
#include "CPickupManager.h"
#include "CResourceClientConfigItem.h"
#include "CColTube.h"
#include "CPerPlayerEntity.h"
#include "CMapFiles.h"
#include "CObjectManager.h"
#include "CColRectangle.h"
#include "CTextItem.h"
#include "CPlayerCamera.h"
#include "CRadarArea.h"
#include "TaskNames.h"
#include "CResourceScriptItem.h"
#include "CUnoccupiedVehicleSync.h"
#include "CElementDeleter.h"
#include "CPad.h"
#include "CElementGroup.h"
#include "CMapManager.h"
#include "CVehicleColorManager.h"
#include "CWeaponNames.h"
#include "CClient.h"
#include "CCommandFile.h"
#include "CLogger.h"
#include "CVehicle.h"
#include "CColSphere.h"
#include "CMapEvent.h"
#include "CPlayer.h"
#include "Utils.h"
#include "CXMLConfig.h"
#include "CStaticFunctionDefinitions.h"
#include "CResourceDownloader.h"
#include "CRegistry.h"
#include "CMainConfig.h"
#include "CAccount.h"
#include "CElement.h"
#include "CResourceClientScriptItem.h"
#include "CPedManager.h"
#include "CDummy.h"
#include "CConfig.h"
#include "CResource.h"

// Utility includes
#include "utils/base64.h"
#include "utils/CHTTPClient.h"
#include "utils/CHTTPRequest.h"
#include "utils/CHTTPResponse.h"
#include "utils/CTCP.h"
#include "utils/CTCPClientSocket.h"
#include "utils/CTCPClientSocketImpl.h"
#include "utils/CTCPImpl.h"
#include "utils/CTCPServerSocket.h"
#include "utils/CTCPServerSocketImpl.h"
#include "utils/CTCPSocket.h"
#include "utils/CTCPSocketImpl.h"
#include "utils/expanding_string.h"
#include "utils/zip/crc32.h"

// Module includes
#include "CServer.h"
#include "Config.h"