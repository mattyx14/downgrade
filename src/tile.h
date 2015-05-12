/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2015  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_TILE_H_96C7EE7CF8CD48E59D5D554A181F0C56
#define FS_TILE_H_96C7EE7CF8CD48E59D5D554A181F0C56

#include <unordered_set>

#include "cylinder.h"
#include "item.h"
#include "tools.h"

class Creature;
class Teleport;
class TrashHolder;
class Mailbox;
class MagicField;
class QTreeLeafNode;
class BedItem;

typedef std::vector<Creature*> CreatureVector;
typedef std::vector<Item*> ItemVector;
typedef std::unordered_set<Creature*> SpectatorVec;

enum tileflags_t : uint32_t {
	TILESTATE_NONE,

	TILESTATE_PROTECTIONZONE = 1 << 0,
	TILESTATE_DEPRECATED_HOUSE = 1 << 1,
	TILESTATE_NOPVPZONE = 1 << 2,
	TILESTATE_NOLOGOUT = 1 << 3,
	TILESTATE_PVPZONE = 1 << 4,
	TILESTATE_REFRESH = 1 << 5, // unused

	//internal usage
	TILESTATE_HOUSE = 1 << 6,
	TILESTATE_FLOORCHANGE = 1 << 7,
	TILESTATE_FLOORCHANGE_DOWN = 1 << 8,
	TILESTATE_FLOORCHANGE_NORTH = 1 << 9,
	TILESTATE_FLOORCHANGE_SOUTH = 1 << 10,
	TILESTATE_FLOORCHANGE_EAST = 1 << 11,
	TILESTATE_FLOORCHANGE_WEST = 1 << 12,
	TILESTATE_TELEPORT = 1 << 13,
	TILESTATE_MAGICFIELD = 1 << 14,
	TILESTATE_MAILBOX = 1 << 15,
	TILESTATE_TRASHHOLDER = 1 << 16,
	TILESTATE_BED = 1 << 17,
	TILESTATE_DEPOT = 1 << 18,
	TILESTATE_BLOCKSOLID = 1 << 19,
	TILESTATE_BLOCKPATH = 1 << 20,
	TILESTATE_IMMOVABLEBLOCKSOLID = 1 << 21,
	TILESTATE_IMMOVABLEBLOCKPATH = 1 << 22,
	TILESTATE_IMMOVABLENOFIELDBLOCKPATH = 1 << 23,
	TILESTATE_NOFIELDBLOCKPATH = 1 << 24,
	TILESTATE_DYNAMIC_TILE = 1 << 25,
	TILESTATE_FLOORCHANGE_SOUTH_ALT = 1 << 26,
	TILESTATE_FLOORCHANGE_EAST_ALT = 1 << 27,
	TILESTATE_SUPPORTS_HANGABLE = 1 << 28,
};

enum ZoneType_t {
	ZONE_PROTECTION,
	ZONE_NOPVP,
	ZONE_PVP,
	ZONE_NOLOGOUT,
	ZONE_NORMAL,
};

class TileItemVector
{
	public:
		TileItemVector() : downItemCount(0) {}

		ItemVector::const_iterator begin() const {
			return items.begin();
		}
		ItemVector::const_reverse_iterator rbegin() const {
			return items.rbegin();
		}

		ItemVector::const_iterator end() const {
			return items.end();
		}
		ItemVector::const_reverse_iterator rend() const {
			return items.rend();
		}

		void clear() {
			items.clear();
		}

		size_t size() const {
			return items.size();
		}

		ItemVector::iterator insert(ItemVector::iterator _where, Item* item) {
			return items.insert(_where, item);
		}
		ItemVector::iterator erase(ItemVector::iterator _pos) {
			return items.erase(_pos);
		}
		Item* at(size_t _pos) const {
			return items.at(_pos);
		}
		Item* back() const {
			return items.back();
		}
		void push_back(Item* item) {
			return items.push_back(item);
		}

		ItemVector::iterator getBeginDownItem() {
			return items.begin();
		}
		ItemVector::const_iterator getBeginDownItem() const {
			return begin();
		}
		ItemVector::iterator getEndDownItem() {
			return items.begin() + downItemCount;
		}
		ItemVector::const_iterator getEndDownItem() const {
			return items.begin() + downItemCount;
		}
		ItemVector::iterator getBeginTopItem() {
			return getEndDownItem();
		}
		ItemVector::const_iterator getBeginTopItem() const {
			return getEndDownItem();
		}
		ItemVector::iterator getEndTopItem() {
			return items.end();
		}
		ItemVector::const_iterator getEndTopItem() const {
			return end();
		}

		uint32_t getTopItemCount() const {
			return size() - downItemCount;
		}
		uint32_t getDownItemCount() const {
			return downItemCount;
		}
		inline Item* getTopTopItem() const {
			if (getTopItemCount() == 0) {
				return nullptr;
			}
			return *(getEndTopItem() - 1);
		}
		inline Item* getTopDownItem() const {
			if (downItemCount == 0) {
				return nullptr;
			}
			return *getBeginDownItem();
		}

	private:
		ItemVector items;
		uint16_t downItemCount;

		friend class Tile;
};

class Tile : public Cylinder
{
	public:
		static Tile& nullptr_tile;
		Tile(uint16_t x, uint16_t y, uint8_t z);
		virtual ~Tile();

		// non-copyable
		Tile(const Tile&) = delete;
		Tile& operator=(const Tile&) = delete;

		TileItemVector* getItemList();
		const TileItemVector* getItemList() const;
		TileItemVector* makeItemList();

		CreatureVector* getCreatures();
		const CreatureVector* getCreatures() const;
		CreatureVector* makeCreatures();

		int32_t getThrowRange() const final {
			return 0;
		}
		bool isPushable() const final {
			return false;
		}

		MagicField* getFieldItem() const;
		Teleport* getTeleportItem() const;
		TrashHolder* getTrashHolder() const;
		Mailbox* getMailbox() const;
		BedItem* getBedItem() const;

		Creature* getTopCreature() const;
		const Creature* getBottomCreature() const;
		Creature* getTopVisibleCreature(const Creature* creature) const;
		const Creature* getBottomVisibleCreature(const Creature* creature) const;
		Item* getTopTopItem() const;
		Item* getTopDownItem() const;
		bool isMoveableBlocking() const;
		Thing* getTopVisibleThing(const Creature* creature);
		Item* getItemByTopOrder(int32_t topOrder);

		size_t getThingCount() const {
			size_t thingCount = getCreatureCount() + getItemCount();
			if (ground) {
				thingCount++;
			}
			return thingCount;
		}
		// If these return != 0 the associated vectors are guaranteed to exists
		size_t getCreatureCount() const;
		size_t getItemCount() const;
		uint32_t getTopItemCount() const;
		uint32_t getDownItemCount() const;

		bool hasProperty(ITEMPROPERTY prop) const;
		bool hasProperty(const Item* exclude, ITEMPROPERTY prop) const;

		bool hasFlag(tileflags_t flag) const {
			return hasBitSet(flag, m_flags);
		}
		void setFlag(tileflags_t flag) {
			m_flags |= static_cast<uint32_t>(flag);
		}
		void resetFlag(tileflags_t flag) {
			m_flags &= ~static_cast<uint32_t>(flag);
		}

		bool positionChange() const {
			return hasFlag(TILESTATE_TELEPORT);
		}
		bool floorChange() const {
			return hasFlag(TILESTATE_FLOORCHANGE);
		}
		bool floorChangeDown() const {
			return hasFlag(TILESTATE_FLOORCHANGE_DOWN);
		}
		bool floorChange(Direction direction) const {
			switch (direction) {
				case DIRECTION_NORTH:
						return hasFlag(TILESTATE_FLOORCHANGE_NORTH);

				case DIRECTION_SOUTH:
					return hasFlag(TILESTATE_FLOORCHANGE_SOUTH);

				case DIRECTION_EAST:
					return hasFlag(TILESTATE_FLOORCHANGE_EAST);

				case DIRECTION_WEST:
					return hasFlag(TILESTATE_FLOORCHANGE_WEST);

				case DIRECTION_SOUTH_ALT:
					return hasFlag(TILESTATE_FLOORCHANGE_SOUTH_ALT);

				case DIRECTION_EAST_ALT:
					return hasFlag(TILESTATE_FLOORCHANGE_EAST_ALT);

				default:
					return false;
			}
		}

		ZoneType_t getZone() const {
			if (hasFlag(TILESTATE_PROTECTIONZONE)) {
				return ZONE_PROTECTION;
			} else if (hasFlag(TILESTATE_NOPVPZONE)) {
				return ZONE_NOPVP;
			} else if (hasFlag(TILESTATE_PVPZONE)) {
				return ZONE_PVP;
			} else {
				return ZONE_NORMAL;
			}
		}

		bool hasHeight(uint32_t n) const;

		std::string getDescription(int32_t lookDistance) const final;

		int32_t getClientIndexOfCreature(const Player* player, const Creature* creature) const;
		int32_t getStackposOfCreature(const Player* player, const Creature* creature) const;
		int32_t getStackposOfItem(const Player* player, const Item* item) const;

		//cylinder implementations
		ReturnValue queryAdd(int32_t index, const Thing& thing, uint32_t count,
				uint32_t flags, Creature* actor = nullptr) const override;
		ReturnValue queryMaxCount(int32_t index, const Thing& thing, uint32_t count,
				uint32_t& maxQueryCount, uint32_t flags) const final;
		ReturnValue queryRemove(const Thing& thing, uint32_t count, uint32_t flags) const final;
		Tile* queryDestination(int32_t& index, const Thing& thing, Item** destItem, uint32_t& flags) override;

		void addThing(Thing* thing) final;
		void addThing(int32_t index, Thing* thing) override;

		void updateThing(Thing* thing, uint16_t itemId, uint32_t count) final;
		void replaceThing(uint32_t index, Thing* thing) final;

		void removeThing(Thing* thing, uint32_t count) final;

		void removeCreature(Creature* creature);

		int32_t getThingIndex(const Thing* thing) const final;
		size_t getFirstIndex() const final;
		size_t getLastIndex() const final;
		uint32_t getItemTypeCount(uint16_t itemId, int32_t subType = -1) const final;
		Thing* getThing(size_t index) const final;

		void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link = LINK_OWNER) final;
		void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t link = LINK_OWNER) final;

		void internalAddThing(Thing* thing) final;
		void internalAddThing(uint32_t index, Thing* thing) override;

		const Position& getPosition() const {
			return tilePos;
		}

		bool isRemoved() const final {
			return false;
		}

		Item* getUseItem() const;

		Item* getGround() const {
			return ground;
		}
		void setGround(Item* item) {
			ground = item;
		}

	private:
		void onAddTileItem(Item* item);
		void onUpdateTileItem(Item* oldItem, const ItemType& oldType, Item* newItem, const ItemType& newType);
		void onRemoveTileItem(const SpectatorVec& list, const std::vector<int32_t>& oldStackPosVector, Item* item);
		void onUpdateTile(const SpectatorVec& list);

		void setTileFlags(const Item* item);
		void resetTileFlags(const Item* item);

	protected:
		// Put this first for cache-coherency
		bool isDynamic() const {
			return (m_flags & TILESTATE_DYNAMIC_TILE) != 0;
		}

		Item* ground;
		Position tilePos;
		uint32_t m_flags;
};

// Used for walkable tiles, where there is high likeliness of
// items being added/removed
class DynamicTile : public Tile
{
		// By allocating the vectors in-house, we avoid some memory fragmentation
		TileItemVector items;
		CreatureVector creatures;

	public:
		DynamicTile(uint16_t x, uint16_t y, uint8_t z);
		~DynamicTile();

		// non-copyable
		DynamicTile(const DynamicTile&) = delete;
		DynamicTile& operator=(const DynamicTile&) = delete;

		TileItemVector* getItemList() {
			return &items;
		}
		const TileItemVector* getItemList() const {
			return &items;
		}
		TileItemVector* makeItemList() {
			return &items;
		}

		CreatureVector* getCreatures() {
			return &creatures;
		}
		const CreatureVector* getCreatures() const {
			return &creatures;
		}
		CreatureVector* makeCreatures() {
			return &creatures;
		}
};

// For blocking tiles, where we very rarely actually have items
class StaticTile final : public Tile
{
	// We very rarely even need the vectors, so don't keep them in memory
	TileItemVector* items;
	CreatureVector* creatures;

	public:
		StaticTile(uint16_t x, uint16_t y, uint8_t z);
		~StaticTile();

		// non-copyable
		StaticTile(const StaticTile&) = delete;
		StaticTile& operator=(const StaticTile&) = delete;

		TileItemVector* getItemList() {
			return items;
		}
		const TileItemVector* getItemList() const {
			return items;
		}
		TileItemVector* makeItemList() {
			if (!items) {
				items = new TileItemVector;
			}
			return items;
		}

		CreatureVector* getCreatures() {
			return creatures;
		}
		const CreatureVector* getCreatures() const {
			return creatures;
		}
		CreatureVector* makeCreatures() {
			if (!creatures) {
				creatures = new CreatureVector;
			}
			return creatures;
		}
};

inline Tile::Tile(uint16_t x, uint16_t y, uint8_t z) :
	ground(nullptr),
	tilePos(x, y, z),
	m_flags(0)
{
}

inline Tile::~Tile()
{
	delete ground;
}

inline CreatureVector* Tile::getCreatures()
{
	if (isDynamic()) {
		return reinterpret_cast<DynamicTile*>(this)->DynamicTile::getCreatures();
	}
	return reinterpret_cast<StaticTile*>(this)->StaticTile::getCreatures();
}

inline const CreatureVector* Tile::getCreatures() const
{
	if (isDynamic()) {
		return reinterpret_cast<const DynamicTile*>(this)->DynamicTile::getCreatures();
	}
	return reinterpret_cast<const StaticTile*>(this)->StaticTile::getCreatures();
}

inline TileItemVector* Tile::getItemList()
{
	if (isDynamic()) {
		return reinterpret_cast<DynamicTile*>(this)->DynamicTile::getItemList();
	}
	return reinterpret_cast<StaticTile*>(this)->StaticTile::getItemList();
}

inline const TileItemVector* Tile::getItemList() const
{
	if (isDynamic()) {
		return reinterpret_cast<const DynamicTile*>(this)->DynamicTile::getItemList();
	}
	return reinterpret_cast<const StaticTile*>(this)->StaticTile::getItemList();
}

inline CreatureVector* Tile::makeCreatures()
{
	if (isDynamic()) {
		return reinterpret_cast<DynamicTile*>(this)->DynamicTile::makeCreatures();
	}
	return reinterpret_cast<StaticTile*>(this)->StaticTile::makeCreatures();
}

inline TileItemVector* Tile::makeItemList()
{
	if (isDynamic()) {
		return reinterpret_cast<DynamicTile*>(this)->DynamicTile::makeItemList();
	}
	return reinterpret_cast<StaticTile*>(this)->StaticTile::makeItemList();
}

inline StaticTile::StaticTile(uint16_t x, uint16_t y, uint8_t z) :
	Tile(x, y, z),
	items(nullptr),
	creatures(nullptr)
{
}

inline StaticTile::~StaticTile()
{
	if (items) {
		for (Item* item : *items) {
			item->decrementReferenceCounter();
		}
		delete items;
	}

	delete creatures;
}

inline DynamicTile::DynamicTile(uint16_t x, uint16_t y, uint8_t z) :
	Tile(x, y, z)
{
	m_flags |= TILESTATE_DYNAMIC_TILE;
}

inline DynamicTile::~DynamicTile()
{
	for (Item* item : items) {
		item->decrementReferenceCounter();
	}
}

#endif
