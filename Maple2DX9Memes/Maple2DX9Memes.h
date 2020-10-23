#pragma once
#include <vector>
#include "flags.h"
#include "d3d9.h"
#include <unordered_map>
//#define BANABLE
//#define DEV

struct renderChange {
	D3DRENDERSTATETYPE state;
	DWORD value;
	renderChange(D3DRENDERSTATETYPE s, DWORD v) : state(s), value(v)
	{

	}
	renderChange(D3DPRIMITIVETYPE s, DWORD v) : state((D3DRENDERSTATETYPE)s), value(v)
	{

	}
};

static struct configInfo {
	bool chestChams = true;
	bool wireFrame = false;
	bool portalChams = false;
	int zoomCap = 1100;
	bool rollCheck = false;
	float viewRoll = 90.f;
	bool yawCheck = false;
	float viewYaw = 45.f;
	bool pitchCheck = false;
	float viewPitch = -40.89387131f;
	bool eyeXCheck = false;
	float eyeX = 500;
	bool eyeYCheck = false;
	float eyeY = 500; 
	float eyeXPrev;
	float eyeYPrev;
	bool debugMode = false;
	bool setRenderLog = false;
	bool setPrimLog = false;
	bool nullRender = false;
	bool nullRenderCheck = false;
	bool holdKey = false;
	unsigned int keycode = 0x20;
	bool playerChams = false;

	bool noFog = false;
	bool wndProcHooks = false;
} hack_config;

static struct playerHacks {
	bool riseOnly = false;
	bool moveToggle = false;
	int moveSpeed = 100;
	bool flightSpeedToggle = false;
	float flyingSpeed = 6.5;
	bool deltaToggle = false;
	int deltaSpeed = 200;
	bool noFall = false;
	bool noClip = false;
	bool jumpToggle = false;
	float jumpHeight = 1;
	bool turboRise = false;
	bool posBypass = false;
	bool altNoFall = false;
	bool floorAdjustToggle = false;
	float newFloor = 0;
	bool spacebarDown = false;
	bool groundMountToggle = false;
	float groundMountSpeed = 0.f;
	bool mountLock = false;
	bool semiNoClip = false;
	bool fMountNoClip = false;

	bool floorOffsetToggle = false;
	int floorOffset = 0;
	bool wallWalkToggle = false;
	float wallWalkHeight = 75.f;
	
	float prevZ = 0;
} player_hacks;


void teleport(float cords[], bool target=true);
void steppedTele(float cords[]);
float* getpos();
void resetWallStep();
void resetFloorOffset();
void key_press(unsigned int key, bool keyup=false);

static std::vector<renderChange> renders;
static std::vector<std::string> recvPackets;
static std::vector<std::string> sentPackets;

static std::vector<float*> teleQueue;

#if defined(DEV) and not defined(KMS)
bool write_packet_hooks();

static std::unordered_map<DWORD, std::string> tClient;
static std::unordered_map<DWORD, std::string> tServer;
static void initMaps() {
	tClient[0] = "kc_NULLPACKET";
	tClient[1] = "kc_response_version";
	tClient[2] = "kc_response_id";
	tClient[3] = "kc_response_key";
	tClient[4] = "kc_request_character_management";
	tClient[5] = "kc_response_heartbeat";
	tClient[6] = "kc_request_timesync";
	tClient[7] = "kc_response_clientticksync";
	tClient[8] = "kc_request_quit";
	tClient[0x0A] = "kc_request_load_ugcmap";
	tClient[0x0B] = "kc_response_field_enter";
	tClient[0x0C] = "kc_send_user_chat";
	tClient[0x0D] = "kc_send_user_sync";
	tClient[0x0E] = "kc_send_emotion";
	tClient[0x0F] = "kc_request_serverlist";
	tClient[0x10] = "kc_request_item_create";
	tClient[0x11] = "kc_request_item_equipment";
	tClient[0x12] = "kc_request_item_inventory";
	tClient[0x13] = "kc_request_item_store";
	tClient[0x14] = "kc_request_item_use";
	tClient[0x15] = "kc_send_item_extradata";
	tClient[0x16] = "kc_request_item_pickup";
	tClient[0x17] = "kc_request_money_pickup";
	tClient[0x18] = "kc_request_pc_info";
	tClient[0x19] = "kc_send_revival";
	tClient[0x1A] = "kc_send_skill";
	tClient[0x1B] = "kc_send_state_skill";
	tClient[0x1C] = "kc_send_npc_talk";
	tClient[0x1D] = "kc_send_beauty";
	tClient[0x1E] = "kc_request_npc_create";
	tClient[0x1F] = "kc_send_job";
	tClient[0x20] = "kc_send_vibrate";
	tClient[0x21] = "kc_send_breakable";
	tClient[0x22] = "kc_send_shop";
	tClient[0x23] = "kc_send_quest";
	tClient[0x24] = "kc_send_tombstone";
	tClient[0x25] = "kc_send_room_dungeon";
	tClient[0x26] = "kc_send_party";
	tClient[0x27] = "kc_send_mail";
	tClient[0x28] = "kc_send_functionCube";
	tClient[0x29] = "kc_send_trigger";
	tClient[0x2A] = "kc_send_buddy";
	tClient[0x2B] = "kc_send_interact_object";
	tClient[0x2C] = "kc_send_interact_npc";
	tClient[0x2D] = "kc_send_state_consumeEP";
	tClient[0x2E] = "kc_send_state_fallDamage";
	tClient[0x2F] = "kc_send_guide_object_sync";
	tClient[0x30] = "kc_request_set_craftmode";
	tClient[0x31] = "kc_request_cube";
	tClient[0x32] = "kc_request_apartment";
	tClient[0x33] = "kc_send_ugc";
	tClient[0x34] = "kc_send_merat_market";
	tClient[0x35] = "kc_send_keytable";
	tClient[0x36] = "kc_send_channel";
	tClient[0x37] = "kc_liftable";
	tClient[0x38] = "kc_send_myinfo";
	tClient[0x39] = "kc_relocate_world";
	tClient[0x3A] = "kc_send_item_fusion";
	tClient[0x3B] = "kc_request_ride";
	tClient[0x3C] = "kc_send_ridee_sync";
	tClient[0x3D] = "kc_send_fittingdoll";
	tClient[0x3E] = "kc_send_bonus_game";
	tClient[0x3F] = "kc_send_resolve_panelty";
	tClient[0x40] = "kc_send_take_boat";
	tClient[0x41] = "kc_send_achieve";
	tClient[0x42] = "kc_request_gem_equipment";
	tClient[0x43] = "kc_request_taxi";
	tClient[0x44] = "kc_send_trade_ex";
	tClient[0x45] = "kc_request_worldmap";
	tClient[0x46] = "kc_guild";
	tClient[0x47] = "kc_group_chat";
	tClient[0x48] = "kc_request_homebank";
	tClient[0x49] = "kc_request_homedoctor";
	tClient[0x4A] = "kc_ah";
	tClient[0x4B] = "kc_set_debug_mode";
	tClient[0x4C] = "kc_request_home";
	tClient[0x4D] = "kc_request_report";
	tClient[0x4E] = "kc_request_warehouse_container";
	tClient[0x4F] = "kc_request_move_eventfield";
	tClient[0x50] = "kc_log_send";
	tClient[0x51] = "kc_send_dps_mode";
	tClient[0x52] = "kc_send_guide_record";
	tClient[0x53] = "kc_rank";
	tClient[0x54] = "kc_request_skip_tutorial";
	tClient[0x55] = "kc_request_item_break";
	tClient[0x56] = "kc_send_ridee_consume_ep";
	tClient[0x57] = "kc_request_additionaleffect";
	tClient[0x58] = "kc_recall_user";
	tClient[0x59] = "kc_request_item_enchant";
	tClient[0x5A] = "kc_black_market";
	tClient[0x5B] = "kc_send_gamble";
	tClient[0x5C] = "kc_send_pvp";
	tClient[0x5D] = "kc_send_maid";
	tClient[0x5E] = "kc_maview";
	tClient[0x5F] = "kc_smart_recommend_billing";
	tClient[0x60] = "kc_system_shop";
	tClient[0x61] = "kc_attend_gift";
	tClient[0x62] = "kc_pcbang_bonus";
	tClient[0x63] = "kc_request_manufacture";
	tClient[0x64] = "kc_request_user_env";
	tClient[0x65] = "kc_send_cash";
	tClient[0x66] = "kc_send_nametag_symbol";
	tClient[0x67] = "kc_request_move_field";
	tClient[0x68] = "kc_waitingticket";
	tClient[0x69] = "kc_match_party";
	tClient[0x6A] = "kc_recall_scroll";
	tClient[0x6B] = "kc_potential_ability";
	tClient[0x6C] = "kc_enchant_scroll";
	tClient[0x6D] = "kc_global_portal";
	tClient[0x6E] = "kc_fishing";
	tClient[0x6F] = "kc_play_instrument";
	tClient[0x70] = "kc_item_remake_option";
	tClient[0x71] = "kc_item_remake_option_scroll";
	//0x72 = UNK (seems to be in place of 83???)
	tClient[0x73] = "kc_request_pet";
	tClient[0x74] = "kc_request_pet_inventory";
	tClient[0x75] = "kc_notice_dialog";
	tClient[0x76] = "kc_skill_compact_control";
	tClient[0x77] = "kc_banword";
	tClient[0x78] = "kc_check_char_name";
	tClient[0x79] = "kc_platform_protect_packet";
	tClient[0x7A] = "kc_platform_account_safe";
	tClient[0x7B] = "kc_send_state";
	tClient[0x7C] = "kc_meso_market";
	tClient[0x7D] = "kc_global_factor";
	tClient[0x7E] = "kc_smart_push";
	tClient[0x7F] = "kc_play_arcade";
	tClient[0x80] = "kc_enter_eventfield";
	tClient[0x81] = "kc_send_card_reverse_game";
	tClient[0x82] = "kc_request_item_lock";
	tClient[0x83] = "kc_item_socket_system";
	tClient[0x72] = "kc_item_enchant_transform";
	tClient[0x84] = "kc_character_ability";
	tClient[0x85] = "kc_tutorial";
	tClient[0x86] = "kc_item_socket_scroll";
	tClient[0x8E] = "kc_send_construct_shop";
	tClient[0x22] = "kc_send_shop"; //declared again?
	tClient[0x94] = "kc_stat_point";
	tClient[0x95] = "kc_send_item_repeat";
	tClient[0xA2] = "kc_superworldchat";
	tClient[0xA9] = "kc_item_lapenshard";
	tClient[0xA3] = "kc_microgame";
	tClient[0xA8] = "kc_adventure_level";

	//server
	tServer[0] = "ks_NULLPACKET";
	tServer[0x01] = "ks_request_version";
	tServer[0x02] = "ks_send_version_result";
	tServer[0x03] = "ks_request_key";
	tServer[0x04] = "ks_request_login";
	tServer[0x05] = "ks_send_login_result";
	tServer[0x06] = "ks_send_serverlist";
	tServer[0x07] = "ks_send_characterlist";
	tServer[0x08] = "ks_send_move_result";
	tServer[0x09] = "ks_send_login_to_game";
	tServer[0x0A] = "ks_send_game_to_login";
	tServer[0x0B] = "ks_send_game_to_game";
	tServer[0x0C] = "ks_response_timesync";
	tServer[0x0D] = "ks_request_heartbeat";
	tServer[0x0E] = "ks_request_clientticksync";
	tServer[0x0F] = "ks_send_sync_number";
	tServer[0x10] = "ks_send_server_enter";
	tServer[0x11] = "ks_request_field_enter";
	tServer[0x12] = "ks_send_field_adduser";
	tServer[0x13] = "ks_send_field_removeuser";
	tServer[0x14] = "ks_field_enterance";
	tServer[0x15] = "ks_send_room_dungeon";
	tServer[0x17] = "ks_send_user_sync";
	tServer[0x18] = "ks_send_user_chat";
	tServer[0x19] = "ks_send_user_chat_itemlink";
	tServer[0x1A] = "ks_send_emotion";
	tServer[0x1B] = "ks_send_item_puton_or_putoff";
	tServer[0x1C] = "ks_send_item_inventory";
	tServer[0x1D] = "ks_send_item_store";
	tServer[0x1E] = "ks_send_warehouse_container";
	//1F = UNK?
	tServer[0x20] = "ks_response_item_puton";
	tServer[0x21] = "ks_response_item_putoff";
	tServer[0x24] = "ks_response_item_drop";
	tServer[0x25] = "ks_send_item_extradata";
	tServer[0x26] = "ks_send_field_additem";
	tServer[0x27] = "ks_send_field_removeitem";
	tServer[0x28] = "ks_send_field_pickupitem";
	tServer[0x29] = "ks_send_field_mutateitem";
	tServer[0x2A] = "ks_send_stat";
	tServer[0x2B] = "ks_send_user_battle";
	tServer[0x2C] = "ks_send_skin_color";
	tServer[0x2D] = "ks_send_beauty";
	tServer[0x2E] = "ks_send_adventurebar";
	tServer[0x2F] = "ks_send_revival_confirm";
	tServer[0x30] = "ks_send_revival";
	//31?
	tServer[0x32] = "ks_send_user_state";
	tServer[0x33] = "ks_send_expup";
	tServer[0x34] = "ks_send_levelup";
	tServer[0x35] = "ks_send_money";
	tServer[0x36] = "ks_send_merat";
	tServer[0x37] = "ks_send_money_token";
	tServer[0x38] = "ks_skill_use";
	tServer[0x39] = "ks_skill_damage";
	tServer[0x3A] = "ks_skill_sync";
	tServer[0x3B] = "ks_skill_cancel";
	tServer[0x3C] = "ks_skill_use_failed";
	//3D?
	tServer[0x3E] = "ks_skill_cooldown";
	tServer[0x3F] = "ks_skill_reset_cooldown";
	tServer[0x40] = "ks_skill_point";
	tServer[0x41] = "ks_stat_point";
	tServer[0x42] = "ks_response_character_create";
	tServer[0x43] = "ks_send_buff";
	tServer[0x44] = "ks_send_field_portal";
	tServer[0x45] = "ks_send_job";
	tServer[0x46] = "ks_send_npc_monologue";
	tServer[0x47] = "ks_send_npc_talk";
	tServer[0x48] = "ks_send_regionskill";
	tServer[0x49] = "ks_send_functionCube";
	tServer[0x4A] = "ks_send_trigger";
	tServer[0x4B] = "ks_send_breakable";
	tServer[0x4C] = "ks_send_room";
	tServer[0x4D] = "ks_send_shop";
	tServer[0x4E] = "ks_send_quest";
	tServer[0x4F] = "ks_send_party";
	tServer[0x50] = "ks_send_mail";
	tServer[0x51] = "ks_send_field_addnpc";
	tServer[0x52] = "ks_send_field_removenpc";
	tServer[0x53] = "ks_send_field_dead_npc";
	tServer[0x54] = "ks_send_npc_control";
	tServer[0x55] = "ks_send_interact_npc";
	tServer[0x56] = "ks_send_field_add_pet";
	tServer[0x57] = "ks_send_field_remove_pet";
	tServer[0x58] = "ks_sync_pet_taming_point";
	tServer[0x59] = "ks_send_tombstone";
	tServer[0x5A] = "ks_send_achieve";
	tServer[0x5B] = "ks_send_usermove_byportal";
	tServer[0x5C] = "ks_send_item_title";
	tServer[0x5D] = "ks_send_massive_event";
	tServer[0x5E] = "ks_send_buddy";
	tServer[0x5F] = "ks_send_admin_block";
	tServer[0x60] = "ks_send_interact_object";
	tServer[0x61] = "ks_send_state_consumeEP";
	tServer[0x62] = "ks_send_state_fallDamage";
	tServer[0x63] = "ks_send_cinematic";
	tServer[0x64] = "ks_send_admin";
	tServer[0x65] = "ks_response_set_craftmode";
	tServer[0x66] = "ks_response_cube";
	tServer[0x67] = "ks_send_cubes";
	tServer[0x68] = "ks_send_ugc";
	tServer[0x69] = "ks_send_merat_market";
	tServer[0x6A] = "ks_send_game_variable";
	tServer[0x6B] = "ks_send_guide_object";
	tServer[0x6C] = "ks_send_keytable";
	tServer[0x6D] = "ks_send_follow_npc";
	tServer[0x6E] = "ks_send_notice";
	tServer[0x6F] = "ks_relocate_world";
	tServer[0x70] = "ks_liftable";
	tServer[0x71] = "ks_send_item_fusion";
	tServer[0x72] = "ks_send_vibrate";
	tServer[0x73] = "ks_send_hide_vibrate";
	tServer[0x74] = "ks_send_show_vibrate";
	tServer[0x75] = "ks_send_pc_info";
	tServer[0x76] = "ks_response_ride";
	tServer[0x77] = "ks_send_ridee_sync";
	tServer[0x78] = "ks_send_fitting_doll";
	tServer[0x79] = "ks_send_bonus_game";
	tServer[0x7A] = "ks_response_load_ugcmap";
	tServer[0x7B] = "ks_send_proxy_gameobj";
	tServer[0x7C] = "ks_send_gem";
	tServer[0x7D] = "ks_send_taxi";
	tServer[0x7E] = "ks_send_find_fields";
	tServer[0x7F] = "ks_send_trade_ex";
	tServer[0x80] = "ks_send_invincible_effect";
	tServer[0x81] = "ks_send_worldmap";
	tServer[0x82] = "ks_send_move_eventfield";
	tServer[0x83] = "ks_send_dps_stat";
	tServer[0x84] = "ks_send_debug_mode";
	tServer[0x85] = "ks_send_story_book";
	tServer[0x86] = "ks_send_guide_record";
	tServer[0x87] = "ks_guild";
	tServer[0x88] = "ks_group_chat";
	tServer[0x89] = "ks_recall_user";
	tServer[0x8A] = "ks_rank";
	tServer[0x8B] = "ks_send_append_message_common";
	tServer[0x8C] = "ks_send_append_message_string";
	tServer[0x8D] = "ks_send_append_message_kill_boss";
	tServer[0x8E] = "ks_send_append_client_log";
	tServer[0x8F] = "ks_send_append_message_fieldboss_assistbonus";
	tServer[0x90] = "ks_ah";
	tServer[0x91] = "ks_send_ui_text";
	tServer[0x92] = "ks_send_play_npc_sound";
	tServer[0x93] = "ks_send_item_break";
	tServer[0x94] = "ks_send_item_enchant";
	tServer[0x95] = "ks_black_market";
	tServer[0x96] = "ks_meso_market";
	tServer[0x97] = "ks_send_team_pvp";
	tServer[0x98] = "ks_send_webopen";
	tServer[0x99] = "ks_send_gamble";
	tServer[0x9A] = "ks_send_field_maid";
	tServer[0x9B] = "ks_send_user_maid";
	tServer[0x9C] = "ks_maview";
	tServer[0x9D] = "ks_smart_recommend_billing";
	tServer[0x9E] = "ks_system_shop";
	tServer[0x9F] = "ks_send_auto_revive";
	tServer[0xA0] = "ks_send_player_kill_notice";
	tServer[0xA1] = "ks_attend_gift";
	tServer[0xA2] = "ks_pcbang_bonus";
	tServer[0xA3] = "ks_send_dead";
	tServer[0xA4] = "ks_dynamic_channel";
	tServer[0xA5] = "ks_send_user_env";
	tServer[0xA6] = "ks_send_manufacture";
	tServer[0xA7] = "ks_response_enter_ugcmap_result";
	tServer[0xA8] = "ks_response_item_use";
	tServer[0xA9] = "ks_send_cash";
	tServer[0xAA] = "ks_send_myinfo";
	tServer[0xAB] = "ks_send_session";
	tServer[0xAC] = "ks_send_world_share_info";
	tServer[0xAD] = "ks_send_nametag_symbol";
	tServer[0xAE] = "ks_game_event";
	tServer[0xAF] = "ks_send_bannerlist";
	tServer[0xB0] = "ks_waitingticketupdate";
	tServer[0xB1] = "ks_set_pcbang";
	tServer[0xB2] = "ks_send_pvp";
	tServer[0xB3] = "ks_send_home_commend";
	tServer[0xB4] = "ks_send_char_max_count";
	tServer[0xB5] = "ks_server_enter";
	tServer[0xB6] = "ks_send_drop_item_get_messsage";
	tServer[0xB7] = "ks_match_party";
	tServer[0xB8] = "ks_recall_scroll";
	tServer[0xB9] = "ks_send_user_condition_event";
	tServer[0xBA] = "ks_potential_ability";
	tServer[0xBB] = "ks_enchant_scroll";
	tServer[0xBC] = "ks_boss_ranking";
	tServer[0xBD] = "ks_global_portal";
	tServer[0xBE] = "ks_quiz_event";
	tServer[0xBF] = "ks_play_system_sound";
	tServer[0xC0] = "ks_fishing";
	tServer[0xC1] = "ks_darkstream";
	tServer[0xC2] = "ks_send_nps_info";
	tServer[0xC3] = "ks_play_instrument";
	tServer[0xC4] = "ks_item_remake_option";
	tServer[0xC5] = "ks_item_remake_option_scroll";
	tServer[0xC6] = "ks_field_property";
	tServer[0xC7] = "ks_game_event_user_value";
	tServer[0xC8] = "ks_response_pet";
	tServer[0xC9] = "ks_send_mastery";
	tServer[0xCA] = "ks_send_pet_inventory";
	tServer[0xCB] = "ks_notice_dialog";
	tServer[0xCC] = "ks_aa_err";
	tServer[0xCD] = "ks_skill_compact_control";
	tServer[0xCE] = "ks_banword";
	tServer[0xCF] = "ks_check_char_name_result";
	tServer[0xD0] = "ks_platform_protect_packet";
	tServer[0xD1] = "ks_item_enchant_transform";
	tServer[0xD2] = "ks_platform_account_safe";
	tServer[0xD3] = "ks_global_factor";
	tServer[0xD4] = "ks_smart_push";
	tServer[0xD5] = "ks_play_arcade";
	tServer[0xD6] = "ks_debug_state";
	tServer[0xD7] = "ks_send_card_reverse_game";
	tServer[0xD8] = "ks_send_item_lock";
	tServer[0xD9] = "ks_send_homebank";
	tServer[0xDA] = "ks_send_homedoctor";
	tServer[0xDB] = "ks_item_socket_system";
	tServer[0xDC] = "ks_character_ability";
	tServer[0xDD] = "ks_shadow_buff";
	tServer[0xDE] = "ks_shadow_expedition";
	tServer[0xDF] = "ks_item_socket_scroll";
	tServer[0xE0] = "ks_item_repacking_scroll";
	//E1?
	tServer[0xE2] = "ks_bypass_key";
	tServer[0xE5] = "ks_send_local_camera";
	tServer[0xEE] = "ks_send_reset_camera";
	tServer[0xED] = "ks_send_onetime_effect";
	tServer[0x102] = "ks_send_gmcommand";
	tServer[0x107] = "ks_skill_book_tree";
	tServer[0x10D] = "ks_superworldchat";
	tServer[0x10F] = "ks_microgame";
	tServer[0x116] = "ks_item_lapenshard";
	tServer[0x111] = "ks_survival_event";
	tServer[0x117] = "ks_adventure_level";
	tServer[0x119] = "ks_world_champion";
	tServer[0x11A] = "ks_sync_value";
}
#endif
template<class T>
T Read(DWORD dwPointer) {
	if(dwPointer>0)
		return *(T*)dwPointer;
	return 0;
}

