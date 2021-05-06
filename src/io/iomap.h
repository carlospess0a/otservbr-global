/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef SRC_IO_IOMAP_H_
#define SRC_IO_IOMAP_H_

#include <utility>

#include "items/item.h"
#include "map/map.h"
#include "map/house/house.h"
#include "creatures/monsters/spawns/spawn_monster.h"
#include "creatures/npcs/spawns/spawn_npc.h"
#include "config/configmanager.h"

extern ConfigManager g_config;

enum OTBM_AttrTypes_t {
	OTBM_ATTR_DESCRIPTION = 1,
	OTBM_ATTR_EXT_FILE = 2,
	OTBM_ATTR_TILE_FLAGS = 3,
	OTBM_ATTR_ACTION_ID = 4,
	OTBM_ATTR_UNIQUE_ID = 5,
	OTBM_ATTR_TEXT = 6,
	OTBM_ATTR_DESC = 7,
	OTBM_ATTR_TELE_DEST = 8,
	OTBM_ATTR_ITEM = 9,
	OTBM_ATTR_DEPOT_ID = 10,
	OTBM_ATTR_EXT_SPAWN_MONSTER_FILE = 11,
	OTBM_ATTR_RUNE_CHARGES = 12,
	OTBM_ATTR_EXT_HOUSE_FILE = 13,
	OTBM_ATTR_HOUSEDOORID = 14,
	OTBM_ATTR_COUNT = 15,
	OTBM_ATTR_DURATION = 16,
	OTBM_ATTR_DECAYING_STATE = 17,
	OTBM_ATTR_WRITTENDATE = 18,
	OTBM_ATTR_WRITTENBY = 19,
	OTBM_ATTR_SLEEPERGUID = 20,
	OTBM_ATTR_SLEEPSTART = 21,
	OTBM_ATTR_CHARGES = 22,
	OTBM_ATTR_EXT_SPAWN_NPC_FILE = 23
};

enum OTBM_NodeTypes_t {
	OTBM_ROOTV1 = 1,
	OTBM_MAP_DATA = 2,
	OTBM_ITEM_DEF = 3,
	OTBM_TILE_AREA = 4,
	OTBM_TILE = 5,
	OTBM_ITEM = 6,
	OTBM_TILE_SQUARE = 7,
	OTBM_TILE_REF = 8,
	OTBM_SPAWNS = 9,
	OTBM_SPAWN_AREA = 10,
	OTBM_MONSTER = 11,
	OTBM_TOWNS = 12,
	OTBM_TOWN = 13,
	OTBM_HOUSETILE = 14,
	OTBM_WAYPOINTS = 15,
	OTBM_WAYPOINT = 16,
};

enum OTBM_TileFlag_t : uint32_t {
	OTBM_TILEFLAG_PROTECTIONZONE = 1 << 0,
	OTBM_TILEFLAG_NOPVPZONE = 1 << 2,
	OTBM_TILEFLAG_NOLOGOUT = 1 << 3,
	OTBM_TILEFLAG_PVPZONE = 1 << 4
};

#pragma pack(1)

struct OTBM_root_header {
	uint32_t version;
	uint16_t width;
	uint16_t height;
	uint32_t majorVersionItems;
	uint32_t minorVersionItems;
};

struct OTBM_Destination_coords {
	uint16_t x;
	uint16_t y;
	uint8_t z;
};

struct OTBM_Tile_coords {
	uint8_t x;
	uint8_t y;
};

#pragma pack()

class IOMap
{
	static Tile* createTile(Item*& ground, Item* item, uint16_t x, uint16_t y, uint8_t z);

	public:
		bool loadMap(Map* map, const std::string& identifier);

		static bool loadMonsters(Map* map) {
			if (map->monsterfile.empty()) {
				// OTBM file doesn't tell us about the monsterfile,
				// Lets guess it is mapname-monster.xml.
				map->monsterfile = g_config.getString(ConfigManager::MAP_NAME);
				map->monsterfile += "-monster.xml";
			}

			return map->spawnsMonster.loadFromXML(map->monsterfile);
		}

		static bool loadNpcs(Map* map) {
			if (map->npcfile.empty()) {
				// OTBM file doesn't tell us about the npcfile,
				// Lets guess it is mapname-npc.xml.
				map->npcfile = g_config.getString(ConfigManager::MAP_NAME);
				map->npcfile += "-npc.xml";
			}

			return map->spawnsNpc.loadFromXml(map->npcfile);
		}

		static bool loadHouses(Map* map) {
			if (map->housefile.empty()) {
				// OTBM file doesn't tell us about the housefile,
				// Lets guess it is mapname-house.xml.
				map->housefile = g_config.getString(ConfigManager::MAP_NAME);
				map->housefile += "-house.xml";
			}

			return map->houses.loadHousesXML(map->housefile);
		}

		const std::string& getLastErrorString() const {
			return errorString;
		}

		void setLastErrorString(std::string error) {
			errorString = std::move(error);
		}

	private:
		bool parseMapDataAttributes(OTB::Loader& loader, const OTB::Node& mapNode, Map& map, const std::string& fileName);
		bool parseWaypoints(OTB::Loader& loader, const OTB::Node& waypointsNode, Map& map);
		bool parseTowns(OTB::Loader& loader, const OTB::Node& townsNode, Map& map);
		bool parseTileArea(OTB::Loader& loader, const OTB::Node& tileAreaNode, Map& map);
		std::string errorString;
};

#endif  // SRC_IO_IOMAP_H_