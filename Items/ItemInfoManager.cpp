#include <BaseApp.hpp> //precomp

#include <chrono>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <json.hpp>

#include <ProtonSDK/TextScanner.h>
#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/FileSystem/FileManager.h>

#include <Items/ItemInfoManager.hpp>


ItemInfoManager g_itemInfoManager;
ItemInfoManager* GetItemInfoManager() { return &g_itemInfoManager; }


int ItemInfoManager::StringToItemFlag(const std::string& str)
{
    if (str == "NONE") return ITEMFLAG_NONE;
    else if (str == "FLIPPED") return FLIPPED;
    else if (str == "EDITABLE") return EDITABLE;
    else if (str == "SEEDLESS") return SEEDLESS;
    else if (str == "PERMANENT") return PERMANENT;
    else if (str == "DROPLESS") return DROPLESS;
    else if (str == "NOSELF") return NOSELF;
    else if (str == "NOSHADOW") return NOSHADOW;
    else if (str == "WORLD_LOCK") return WORLD_LOCK;
    else if (str == "BETA") return BETA;
    else if (str == "AUTOPICKUP") return AUTOPICKUP;
    else if (str == "MOD") return MOD;
    else if (str == "RANDGROW") return RANDGROW;
    else if (str == "PUBLIC") return PUBLIC;
    else if (str == "FOREGROUND") return FOREGROUND;
    else if (str == "HOLIDAY") return HOLIDAY;
    else if (str == "UNTRADEABLE") return UNTRADEABLE;

    return ITEMFLAG_NONE;
}

std::string ItemInfoManager::ItemFlagToString(const int& itemFlag)
{
    switch (itemFlag) 
    {
    case ITEMFLAG_NONE: return "NONE";
    case FLIPPED: return "FLIPPED";
    case EDITABLE: return "EDITABLE";
    case SEEDLESS: return "SEEDLESS";
    case PERMANENT: return "PERMANENT";
    case DROPLESS: return "DROPLESS";
    case NOSELF: return "NOSELF";
    case NOSHADOW: return "NOSHADOW";
    case WORLD_LOCK: return "WORLD_LOCK";
    case BETA: return "BETA";
    case AUTOPICKUP: return "AUTOPICKUP";
    case MOD: return "MOD";
    case RANDGROW: return "RANDGROW";
    case PUBLIC: return "PUBLIC";
    case FOREGROUND: return "FOREGROUND";
    case HOLIDAY: return "HOLIDAY";
    case UNTRADEABLE: return "UNTRADEABLE";
    default: return "NONE";
    }

    return "NONE";
}

int ItemInfoManager::StringToItemFlag2(const std::string& str)
{
    if (str == "NONE") return ITEMFLAG2_NONE;
    else if (str == "ROBOT_DEADLY") return ROBOT_DEADLY;
    else if (str == "ROBOT_SHOOT_LEFT") return ROBOT_SHOOT_LEFT;
    else if (str == "ROBOT_SHOOT_RIGHT") return ROBOT_SHOOT_RIGHT;
    else if (str == "ROBOT_SHOOT_DOWN") return ROBOT_SHOOT_DOWN;
    else if (str == "ROBOT_SHOOT_UP") return ROBOT_SHOOT_UP;
    else if (str == "ROBOT_CAN_SHOOT") return ROBOT_CAN_SHOOT;
    else if (str == "ROBOT_LAVA") return ROBOT_LAVA;
    else if (str == "ROBOT_POINTY") return ROBOT_POINTY;
    else if (str == "ROBOT_SHOOT_DEADLY") return ROBOT_SHOOT_DEADLY;
    else if (str == "GUILD_ITEM") return GUILD_ITEM;
    else if (str == "GUILD_FLAG") return GUILD_FLAG;
    else if (str == "STARSHIP_HELP") return STARSHIP_HELP;
    else if (str == "STARSHIP_REACTOR") return STARSHIP_REACTOR;
    else if (str == "STARSHIP_VIEWSCREEN") return STARSHIP_VIEWSCREEN;
    else if (str == "SUPERMOD") return SUPERMOD;
    else if (str == "TILE_DEADLY_IF_ON") return TILE_DEADLY_IF_ON;
    else if (str == "LONG_HAND_ITEM") return LONG_HAND_ITEM;
    else if (str == "GEMLESS") return GEMLESS;
    else if (str == "TRANSMUTABLE") return TRANSMUTABLE;
    else if (str == "DUNGEON_ITEM") return DUNGEON_ITEM;
    else if (str == "PVE_MELEE") return PVE_MELEE;
    else if (str == "PVE_RANGED") return PVE_RANGED;
    else if (str == "PVE_AUTO_AIM") return PVE_AUTO_AIM;
    else if (str == "ONE_IN_WORLD") return ONE_IN_WORLD;
    else if (str == "ONLY_FOR_WORLD_OWNER") return ONLY_FOR_WORLD_OWNER;

    return ITEMFLAG2_NONE;
}

std::string ItemInfoManager::ItemFlag2ToString(const int& itemFlag)
{
    switch (itemFlag)
    {
    case ITEMFLAG2_NONE: return "NONE";
    case ROBOT_DEADLY: return "ROBOT_DEADLY";
    case ROBOT_SHOOT_LEFT: return "ROBOT_SHOOT_LEFT";
    case ROBOT_SHOOT_RIGHT: return "ROBOT_SHOOT_RIGHT";
    case ROBOT_SHOOT_DOWN: return "ROBOT_SHOOT_DOWN";
    case ROBOT_SHOOT_UP: return "ROBOT_SHOOT_UP";
    case ROBOT_CAN_SHOOT: return "ROBOT_CAN_SHOOT";
    case ROBOT_LAVA: return "ROBOT_LAVA";
    case ROBOT_POINTY: return "ROBOT_POINTY";
    case ROBOT_SHOOT_DEADLY: return "ROBOT_SHOOT_DEADLY";
    case GUILD_ITEM: return "GUILD_ITEM";
    case GUILD_FLAG: return "GUILD_FLAG";
    case STARSHIP_HELP: return "STARSHIP_HELP";
    case STARSHIP_REACTOR: return "STARSHIP_REACTOR";
    case STARSHIP_VIEWSCREEN: return "STARSHIP_VIEWSCREEN";
    case SUPERMOD: return "SUPERMOD";
    case TILE_DEADLY_IF_ON: return "TILE_DEADLY_IF_ON";
    case LONG_HAND_ITEM: return "LONG_HAND_ITEM";
    case GEMLESS: return "GEMLESS";
    case TRANSMUTABLE: return "TRANSMUTABLE";
    case DUNGEON_ITEM: return "DUNGEON_ITEM";
    case PVE_MELEE: return "PVE_MELEE";
    case PVE_RANGED: return "PVE_RANGED";
    case PVE_AUTO_AIM: return "PVE_AUTO_AIM";
    case ONE_IN_WORLD: return "ONE_IN_WORLD";
    case ONLY_FOR_WORLD_OWNER: return "ONLY_FOR_WORLD_OWNER";
    default: return "NONE";
    }

    return "NONE";
}

int ItemInfoManager::StringToItemFxFlag(const std::string& str)
{
    if (str == "NONE") return ITEMFXFLAG_NONE;
    else if (str == "MULTI_ANIM_START") return MULTI_ANIM_START;
    else if (str == "PING_PONG_ANIM") return PING_PONG_ANIM;
    else if (str == "OVERLAY_OBJECT") return OVERLAY_OBJECT;
    else if (str == "OFFSET_UP") return OFFSET_UP;
    else if (str == "DUAL_LAYER") return DUAL_LAYER;
    else if (str == "MULTI_ANIM2_START") return MULTI_ANIM2_START;
    else if (str == "UNK_0x40") return UNK_0x40;
    else if (str == "USE_SKIN_TINT") return USE_SKIN_TINT;
    else if (str == "SEED_TINT_LAYER1") return SEED_TINT_LAYER1;
    else if (str == "SEED_TINT_LAYER2") return SEED_TINT_LAYER2;
    else if (str == "RAINBOW_TINT_LAYER1") return RAINBOW_TINT_LAYER1;
    else if (str == "RAINBOW_TINT_LAYER2") return RAINBOW_TINT_LAYER2;
    else if (str == "GLOW") return GLOW;
    else if (str == "NO_ARMS") return NO_ARMS;
    else if (str == "FRONT_ARM_PUNCH") return FRONT_ARM_PUNCH;
    else if (str == "RENDER_OFFHAND") return RENDER_OFFHAND;
    else if (str == "SLOWFALL_OBJECT") return SLOWFALL_OBJECT;
    else if (str == "REPLACEMENT_SPRITE") return REPLACEMENT_SPRITE;
    else if (str == "ORB_FLOAT") return ORB_FLOAT;
    else if (str == "UNK_0x80000") return UNK_0x80000;
    else if (str == "RENDER_FX_VARIANT_VERSION") return RENDER_FX_VARIANT_VERSION;

    return ITEMFXFLAG_NONE;
}

std::string ItemInfoManager::ItemFxFlagToString(const int& fxFlag)
{
    switch (fxFlag)
    {
    case ITEMFXFLAG_NONE: return "NONE";
    case MULTI_ANIM_START: return "MULTI_ANIM_START";
    case PING_PONG_ANIM: return "PING_PONG_ANIM";
    case OVERLAY_OBJECT: return "OVERLAY_OBJECT";
    case OFFSET_UP: return "OFFSET_UP";
    case DUAL_LAYER: return "DUAL_LAYER";
    case MULTI_ANIM2_START: return "MULTI_ANIM2_START";
    case UNK_0x40: return "UNK_0x40";
    case USE_SKIN_TINT: return "USE_SKIN_TINT";
    case SEED_TINT_LAYER1: return "SEED_TINT_LAYER1";
    case SEED_TINT_LAYER2: return "SEED_TINT_LAYER2";
    case RAINBOW_TINT_LAYER1: return "RAINBOW_TINT_LAYER1";
    case RAINBOW_TINT_LAYER2: return "RAINBOW_TINT_LAYER2";
    case GLOW: return "GLOW";
    case NO_ARMS: return "NO_ARMS";
    case FRONT_ARM_PUNCH: return "FRONT_ARM_PUNCH";
    case RENDER_OFFHAND: return "RENDER_OFFHAND";
    case SLOWFALL_OBJECT: return "SLOWFALL_OBJECT";
    case REPLACEMENT_SPRITE: return "REPLACEMENT_SPRITE";
    case ORB_FLOAT: return "ORB_FLOAT";
    case UNK_0x80000: return "UNK_0x80000";
    case RENDER_FX_VARIANT_VERSION: return "RENDER_FX_VARIANT_VERSION";
    default: return "NONE";
    }

    return "NONE";
}

bool ItemInfoManager::Load(TextScanner& t)
{
#ifdef _MSC_VER
    g_lastCall = __FUNCSIG__;
#else
    g_lastCall = __PRETTY_FUNCTION__;
#endif
    if (!t.LoadFile("item_definitions.txt"))
    {
        return false;
    }

    std::vector<ItemInfo*> customItems;
    auto then = std::chrono::system_clock::now();
    int updated = 0;
    int currentItemID = 0;
    for (int currentLine = 0; currentLine < t.GetLineCount(); currentLine++) 
    {
        const std::string& lineString = t.GetLine(currentLine);
        std::vector<std::string> line = t.TokenizeLine(currentLine);
        if (line.empty() || lineString.starts_with("#"))
            continue;

        if (line[0] == "add_item" || line[0] == "add_item_seed" || line[0] == "add_item_clothing")
        {
            //#add_item|ID|name|eItemType|soundType|eVisualType|eTileStorage|layer|eTileCollision|hardness|regenTime|eClothingType|

            ItemInfo* pItem = new ItemInfo();
            pItem->ID = std::atoi(line[1].c_str());
            pItem->name = line[2];
            pItem->type = std::atoi(line[3].c_str());
            pItem->soundType = std::atoi(line[4].c_str());
            pItem->visualType = std::atoi(line[5].c_str());
            pItem->tileStorage = std::atoi(line[6].c_str());
            pItem->layer = std::atoi(line[7].c_str());
            pItem->tileCollision = std::atoi(line[8].c_str());
            pItem->hardness = std::atoi(line[9].c_str());
            pItem->regenTime = std::atoi(line[10].c_str());
            pItem->bodyPart = std::atoi(line[11].c_str());

            if (pItem->ID < 0)
            {
                //it's added after the items
                customItems.push_back(std::move(pItem));
            }
            else
            {
                m_items.push_back(std::move(pItem));
                currentItemID = pItem->ID;
            }
        }

        if (line[0] == "set_flags")
        {
            //set_flags|FLAG |FLAG1 |FLAG2 |FLAG3 |

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
            {
                for (size_t i = 1; i < line.size(); i++)
                {
                    pItem->editableTypes |= StringToItemFlag(line[i].c_str());
                }
            }
        }

        if (line[0] == "set_cook")
        {
            //set_cook|cookingTime|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
                pItem->cookingTime = std::atoi(line[1].c_str());
        }

        if (line[0] == "set_texture")
        {
            //set_texture|texture|textureHash|textureX|textureY|
            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
            {
                pItem->texture = line[1];
                pItem->textureHash = std::atoi(line[2].c_str());
                pItem->textureX = std::atoi(line[3].c_str());
                pItem->textureY = std::atoi(line[4].c_str());
            }
        }

        if (line[0] == "set_extra_string")
        {
            //set_extra_string|animMS|extraTexture|extraTextureHash|
            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
            {
                pItem->animMS = std::atoi(line[1].c_str());
                pItem->textureExtra = line[2];
                pItem->textureExtraHash = std::atoi(line[3].c_str());
            }
        }

        if (line[0] == "set_rarity")
        {
            //set_rarity|rarity|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
                pItem->rarity = std::atoi(line[1].c_str());
        }

        if (line[0] == "set_max_count")
        {
            //set_max_count|count|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
                pItem->maxCount = std::atoi(line[1].c_str());
        }

        if (line[0] == "set_pet_info")
        {
            //set_pet_info|petName|petPrefix|petSuffix|petAbility|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
            {
                pItem->petName = line[1];
                pItem->petPrefix = line[2];
                pItem->petSuffix = line[3];
                pItem->petAbility = line[4];
            }
        }

        if (line[0] == "set_seed_info")
        {
            //set_seed_info|seedBase|seedOver|treeBase|treeOver|seedColor|r,g,b,a|treeColor|r,g,b,a|seed1|seed2|bloomTime|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
            {
                pItem->seedBase = std::atoi(line[1].c_str());
                pItem->seedOver = std::atoi(line[2].c_str());
                pItem->treeBase = std::atoi(line[3].c_str());
                pItem->treeOver = std::atoi(line[4].c_str());

                std::vector<std::string> seedColorArr = Utils::StringTokenize(line[6], ",");
                pItem->seedColor = MAKE_RGBA(uint32_t(std::atoi(seedColorArr[0].c_str())), uint32_t(std::atoi(seedColorArr[1].c_str())), uint32_t(std::atoi(seedColorArr[2].c_str())), uint32_t(std::atoi(seedColorArr[3].c_str())));
                
                std::vector<std::string> treeColorArr = Utils::StringTokenize(line[8], ",");
                pItem->treeColor = MAKE_RGBA(uint32_t(std::atoi(treeColorArr[0].c_str())), uint32_t(std::atoi(treeColorArr[1].c_str())), uint32_t(std::atoi(treeColorArr[2].c_str())), uint32_t(std::atoi(treeColorArr[3].c_str())));

                pItem->seed1 = std::atoi(line[9].c_str());
                pItem->seed2 = std::atoi(line[10].c_str());
                pItem->bloomTime = std::atoi(line[11].c_str());
            }
        }

        if (line[0] == "set_animations")
        {
            //set_animations|animationType|animString|animTexture|animString2|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
            {
                pItem->animationType = std::atoi(line[1].c_str());
                pItem->animString = line[2];
                pItem->animTexture = line[3];
                pItem->animString2 = line[4];
            }
        }

        if (line[0] == "set_dlayers")
        {
            //set_dlayers|DLayer1|DLayer2|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
            {
                pItem->DLayer1 = std::atoi(line[1].c_str());
                pItem->DLayer2 = std::atoi(line[2].c_str());
            }
        }

        if (line[0] == "set_flags2")
        {
            //set_flags2|FLAG |FLAG1 |FLAG2 |FLAG3 |
            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
            {
                std::string bits = "";
                for (size_t i = 1; i < line.size(); i++)
                {
                    bits.append(line[i] + ", ");
                    pItem->flags |= StringToItemFlag2(line[i].c_str());
                }

                bits.resize(bits.size() - 2); //removing ', ' from end
            }
        }

        if (line[0] == "set_unknown9")
        {
            //set_unknown9|0,1,2,3,...|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
            {
                std::vector<std::string> v9 = Utils::StringTokenize(line[1], ",");
                if (v9.size() != 60)
                {
                    return false;
                }

                for (uint8_t i = 0; i < 60; i++)
                {
                    pItem->unwantedV9[i] = std::atoi(v9[i].c_str());
                }
            }
        }

        if (line[0] == "set_ranges")
        {
            //set_ranges|tileRange|pileRange|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
            {
                pItem->tileRange = std::atoi(line[1].c_str());
                pItem->pileRange = std::atoi(line[2].c_str());
            }
        }

        if (line[0] == "set_custom_punch")
        {
            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
                pItem->customPunch = line[1];
        }

        if (line[0] == "set_fx_flags")
        {
            //set_fx_flags|FLAG |FLAG1 |FLAG2 |FLAG3 |
            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
            {
                for (size_t i = 1; i < line.size(); i++)
                {
                    pItem->fxFlags |= StringToItemFxFlag(line[i].c_str());
                }
            }
        }

        if (line[0] == "set_ranges")
        {
            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
            {
                pItem->tileRange = std::atoi(line[1].c_str());
                pItem->pileRange = std::atoi(line[2].c_str());
            }
        }

        if (line[0] == "set_custom_punch")
        {
            //set_custom_punch|customPunch|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
                pItem->customPunch = line[1];
        }


        if (line[0] == "set_body_parts")
        {
            //set_body_parts|0,1,2,3,...|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
            {
                std::vector<std::string> parts = Utils::StringTokenize(line[1], ",");
                if (parts.size() != 9)
                {
                    return false;
                }

                for (uint8_t i = 0; i < 9; i++)
                {
                    pItem->bodyParts[i] = std::atoi(parts[i].c_str());
                }
            }
        }

        if (line[0] == "set_clock")
        {
            //set_clock|clockDivier|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
                pItem->clockDivider = std::atoi(line[1].c_str());
        }

        if (line[0] == "set_parent")
        {
            //set_parent|parentID|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
                pItem->parentID = std::atoi(line[1].c_str());
        }

        if (line[0] == "set_sit_info")
        {
            //set_sit_info|sitable|sitOffsetX|sitOffsetY|sitOverlayX|sitOverlayY|sitOverlayOffsetX|sitOverlayOffsetY|sitTexture|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
            {
                pItem->sitable = std::atoi(line[1].c_str());
                pItem->sitOffsetX = std::atoi(line[2].c_str());
                pItem->sitOffsetY = std::atoi(line[3].c_str());
                pItem->sitOverlayX = std::atoi(line[4].c_str());
                pItem->sitOverlayY = std::atoi(line[5].c_str());
                pItem->sitOverlayOffsetX = std::atoi(line[6].c_str());
                pItem->sitOverlayOffsetY = std::atoi(line[7].c_str());
                pItem->sitTexture = line[8];
            }
        }

        if (line[0] == "set_renderer_info")
        {
            //set_renderer_info|rendererFile|unwantedV17|rendererHash|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
            {
                pItem->rendererFile = line[1];
                pItem->unwantedV17 = std::atoi(line[2].c_str());
                pItem->rendererHash = std::atoi(line[3].c_str());
            }
        }

        if (line[0] == "set_desc")
        {
            //set_desc|description|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
                pItem->desc = line[1];
        }

        if (line[0] == "set_lock_power")
        {
            //set_lock_power|lockPower|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
                pItem->lockPower = std::atoi(line[1].c_str());
        }

        if (line[0] == "is_locked")
        {
            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
                pItem->bLocked = true;
        }

        if (line[0] == "link_playmod")
        {
            //link_playmod|playmodID|

            ItemInfo* pItem = GetItem(currentItemID);
            if (pItem)
                pItem->playmodID = std::atoi(line[1].c_str());
        }
    }

    for (ItemInfo* pItem : customItems)
    {
        pItem->itemID = currentItemID + pItem->ID * -1 - 1;
        m_items.push_back(std::move(pItem));
    }

    m_itemCount = (int)m_items.size();
    LogMsg("loaded item_definitions.txt, %d items, %d hashes updated.", (int)m_items.size(), updated);
    t.Kill();
    DumpDefinitions();
    return true;
}

bool ItemInfoManager::LoadFromFile() 
{
#ifdef _MSC_VER
    g_lastCall = __FUNCSIG__;
#else
    g_lastCall = __PRETTY_FUNCTION__;
#endif
    FileInstance f("essential/items.dat");
    if (!f.IsLoaded())
    {
        return false;
    }

    char* pCharData = f.GetAsChars();
    int size = f.GetSize();
    int offset = 0;

    m_data = std::vector<char>(pCharData, pCharData + size);
    m_hash = Utils::HashString(pCharData, size);
    uint8_t* pMem = reinterpret_cast<uint8_t*>(m_data.data());

    MemorySerialize(&m_version, pMem, offset, false);
    MemorySerialize(&m_itemCount, pMem, offset, false);

    /*std::ofstream o("enum.txt");
    o << "enum eItems {\n";*/
    for (int i = 0; i < m_itemCount; i++) 
    {
        ItemInfo* pItem = new ItemInfo();
        pItem->SerializeFromMem(m_version, pMem, offset);
        if (i != pItem->ID)
        {
            return false;
        }

        /*std::string name = pItem->name;
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        Utils::StringReplace(" ", "_", name);
        Utils::StringReplace("_-_", "_", name);
        Utils::StringReplace("-", "_", name);
        Utils::StringReplace(":", "_", name);
        Utils::StringReplace("'", "_", name);
        Utils::StringReplace("!", "_", name);
        Utils::StringReplace("#", "_", name);
        Utils::StringReplace(".", "_", name);
        Utils::StringReplace("(", "_", name);
        Utils::StringReplace(")", "_", name);

        o << std::format("    ITEM_ID_{} = {},\n", name, pItem->ID);*/
        m_items.push_back(std::move(pItem));
    }

    /*o << "};\n";
    o.close();*/

    m_packet = (GameUpdatePacket*)std::malloc(sizeof(GameUpdatePacket) + size);
    std::memset(m_packet, 0, sizeof(GameUpdatePacket) + size);
    m_packet->type = NET_GAME_PACKET_SEND_ITEM_DATABASE_DATA;
    m_packet->netID = -1;
    m_packet->flags |= NET_GAME_PACKET_FLAG_EXTENDED;
    m_packet->dataLength = size;
    memcpy(&m_packet->data, pCharData, size);

    LogMsg("loaded items.dat");
    DumpDefinitions();
    return true;
}

bool ItemInfoManager::LoadSplicing()
{
#ifdef _MSC_VER
    g_lastCall = __FUNCSIG__;
#else
    g_lastCall = __PRETTY_FUNCTION__;
#endif

    TextScanner t;
    t.LoadFile("essential/seeds.txt");
    if (!t.IsLoaded())
    {
        LogError("seeds.txt missing, failed to load splicing definitions.");
        return false;
    }

    int currentItemID = 0;
    for (int currentLine = 2; currentLine < t.GetLineCount(); currentLine++) 
    {
        const std::string& lineString = t.GetLine(currentLine);
        const std::vector<std::string>& line = t.TokenizeLine(currentLine);
        if (line.empty() || lineString.empty() || lineString.starts_with("#") || lineString.starts_with("//"))
        {
            continue;
        }

        if (line[0] == "setup_splicing_recipe") 
        {
            if (line.size() < 6)
            {
                return false;
            }

            SplicingRecipe recipe{};
            recipe.seed1 = (uint16_t)std::atoi(line[2].c_str());
            recipe.seed2 = (uint16_t)std::atoi(line[4].c_str());
            recipe.result = (uint16_t)std::atoi(line[6].c_str());

            ItemInfo* pItem = GetItem(recipe.result - 1);
            if (pItem) 
            {
                pItem->spliceOne = recipe.seed1;
                pItem->spliceTwo = recipe.seed2;
            }

            m_splices.push_back(recipe);
        }
    }

    LogMsg("%d splicing recipes loaded.", (int)m_splices.size());
    t.Kill();
    return true;
}

void ItemInfoManager::DumpDefinitions() 
{
#ifdef _MSC_VER
    g_lastCall = __FUNCSIG__;
#else
    g_lastCall = __PRETTY_FUNCTION__;
#endif

    std::fstream defs("items/item_definitions.txt", std::ios::out);
    if (!defs.is_open())
    {
        LogError("failed to dump item_definitions.txt");
        return;
    }

    defs << "#add_item|ID|name|eItemType|soundType|eVisualType|eTileStorage|layer|eTileCollision|hardness|regenTime|eClothingType|\n";
    defs << "#add_item_seed|ID|name|eItemType|soundType|eVisualType|eTileStorage|layer|eTileCollision|hardness|regenTime|eClothingType|\n";
    defs << "#add_item_clothing|ID|name|eItemType|soundType|eVisualType|eTileStorage|layer|eTileCollision|hardness|regenTime|eClothingType|\n";
    defs << "#set_flags|FLAG |FLAG1 |FLAG2 |FLAG3 |\n";
    defs << "#set_cook|cookingTime|\n";
    defs << "#set_texture|texture|textureHash|textureX|textureY|\n";
    defs << "#set_extra_string|animMS|extraTexture|extraTextureHash|\n";
    defs << "#set_rarity|rarity|\n";
    defs << "#set_max_count|count|\n";
    defs << "#set_pet_info|petName|petPrefix|petSuffix|petAbility|\n";
    defs << "#set_seed_info|seedBase|seedOver|treeBase|treeOver|seedColor|r,g,b,a|treeColor|r,g,b,a|seed1|seed2|bloomTime|\n";
    defs << "#set_animations|animationType|animString|animTexture|animString2|\n";
    defs << "#set_dlayers|DLayer1|DLayer2|\n";
    defs << "#set_flags2|FLAG |FLAG1 |FLAG2 |FLAG3 |\n";
    defs << "#set_unknown9|0,1,2,3,...|\n";
    defs << "#set_ranges|tileRange|pileRange|\n";
    defs << "#set_custom_punch|customPunch|\n";
    defs << "#set_fx_flags|FLAG |FLAG1 |FLAG2 |FLAG3 |\n";
    defs << "#set_body_parts|0,1,2,3,...|\n";
    defs << "#set_clock|clockDivider|\n";
    defs << "#set_parent|parentID|\n";
    defs << "#set_sit_info|sitable|sitOffsetX|sitOffsetY|sitOverlayX|sitOverlayY|sitOverlayOffsetX|sitOverlayOffsetY|sitTexture|\n";
    defs << "#set_renderer_info|rendererFile|unwantedV17|rendererHash|\n";
    defs << "#set_desc|description|\n";
    defs << "#set_lock_power|lockPower|\n";
    defs << "#set_chi|eElement|\n";
    defs << "#is_locked|\n";
    defs << "#link_playmod|playmodID|\n";

    defs << "\n\n\n";

    for (ItemInfo* pItem : m_items)
    {
        std::string type = "add_item";
        if (pItem->type == TYPE_SEED)
        {
            type = "add_item_seed";
        }

        if (pItem->type == TYPE_CLOTHES)
        {
            type = "add_item_clothing";
        }

        defs << type + "|" + std::to_string((int)pItem->ID) + "|" + pItem->name + "|" + std::to_string((int)pItem->type) + "|" + std::to_string((int)pItem->soundType) + "|" + std::to_string((int)pItem->tileStorage) + "|" + std::to_string((int)pItem->tileCollision) + "|" + std::to_string((int)pItem->layer) + "|" + std::to_string((int)pItem->tileCollision) + "|" + std::to_string((int)pItem->hardness) + "|" + std::to_string((int)pItem->regenTime) + "|" + std::to_string((int)pItem->bodyPart) + "|\n";
        if (pItem->editableTypes != 0)
        {
            std::string bits = "";

            if (pItem->editableTypes & FLIPPED)
                bits.append(ItemFlagToString(FLIPPED) + "|");
            if (pItem->editableTypes & EDITABLE)
                bits.append(ItemFlagToString(EDITABLE) + "|");
            if (pItem->editableTypes & SEEDLESS)
                bits.append(ItemFlagToString(SEEDLESS) + "|");
            if (pItem->editableTypes & PERMANENT)
                bits.append(ItemFlagToString(PERMANENT) + "|");
            if (pItem->editableTypes & DROPLESS)
                bits.append(ItemFlagToString(DROPLESS) + "|");
            if (pItem->editableTypes & NOSELF)
                bits.append(ItemFlagToString(NOSELF) + "|");
            if (pItem->editableTypes & NOSHADOW)
                bits.append(ItemFlagToString(NOSHADOW) + "|");
            if (pItem->editableTypes & WORLD_LOCK)
                bits.append(ItemFlagToString(WORLD_LOCK) + "|");
            if (pItem->editableTypes & BETA)
                bits.append(ItemFlagToString(BETA) + "|");
            if (pItem->editableTypes & AUTOPICKUP)
                bits.append(ItemFlagToString(AUTOPICKUP) + "|");
            if (pItem->editableTypes & MOD)
                bits.append(ItemFlagToString(MOD) + "|");
            if (pItem->editableTypes & RANDGROW)
                bits.append(ItemFlagToString(RANDGROW) + "|");
            if (pItem->editableTypes & PUBLIC)
                bits.append(ItemFlagToString(PUBLIC) + "|");
            if (pItem->editableTypes & FOREGROUND)
                bits.append(ItemFlagToString(FOREGROUND) + "|");
            if (pItem->editableTypes & HOLIDAY)
                bits.append(ItemFlagToString(HOLIDAY) + "|");
            if (pItem->editableTypes & UNTRADEABLE)
                bits.append(ItemFlagToString(UNTRADEABLE) + "|");

            defs << std::format("set_flags|{}\n", bits);
        }

        if (pItem->cookingTime != -1) 
        {
            defs << std::format("set_cook|{}|\n", (int)pItem->cookingTime);
        }

        defs << "set_texture|" + pItem->texture + "|" + std::to_string((int)pItem->textureHash) + "|" + std::to_string((int)pItem->textureX) + "|" + std::to_string((int)pItem->textureY) + "|\n";

        if (!pItem->textureExtra.empty() || pItem->animMS != 400 || pItem->textureExtraHash != 0)
        {
            defs << "set_extra_string|" + std::to_string((int)pItem->animMS) + "|" + pItem->textureExtra + "|" + std::to_string((int)pItem->textureExtraHash) + "|\n";
        }

        defs << std::format("set_rarity|{}|\n", (int)pItem->rarity);

        if (pItem->maxCount != 200)
        {
            defs << std::format("set_max_count|{}|\n", (int)pItem->maxCount);
        }

        if (!pItem->petName.empty() || !pItem->petPrefix.empty() || !pItem->petSuffix.empty() || !pItem->petAbility.empty()) 
        {
            defs << std::format("set_pet_info|{}|{}|{}|{}|\n", pItem->petName, pItem->petPrefix, pItem->petSuffix, pItem->petAbility);
        }

        std::string seedRGBA = std::to_string((int)GET_RED(pItem->seedColor)) + "," + std::to_string((int)GET_GREEN(pItem->seedColor)) + "," + std::to_string((int)GET_BLUE(pItem->seedColor)) + "," + std::to_string((int)GET_ALPHA(pItem->seedColor));
        std::string treeRGBA = std::to_string((int)GET_RED(pItem->treeColor)) + "," + std::to_string((int)GET_GREEN(pItem->treeColor)) + "," + std::to_string((int)GET_BLUE(pItem->treeColor)) + "," + std::to_string((int)GET_ALPHA(pItem->treeColor));
        defs << "set_seed_info|" + std::to_string((int)pItem->seedBase) + "|" + std::to_string((int)pItem->seedOver) + "|" + std::to_string((int)pItem->treeBase) + "|" + std::to_string((int)pItem->treeOver) + "|seedColor|" + seedRGBA + "|treeColor|" + treeRGBA + "|" + std::to_string((int)pItem->seed1) + "|" + std::to_string((int)pItem->seed2) + "|" + std::to_string((int)pItem->bloomTime) + "|\n";

        if (!pItem->animString.empty() || !pItem->animTexture.empty() || !pItem->animString2.empty() || pItem->animationType != 0)
        {
            defs << "set_animations|" + std::to_string((int)pItem->animationType) + "|" + pItem->animString + "|" + pItem->animTexture + "|" + pItem->animString2 + "|\n";
        }

        if (pItem->DLayer1 != 0 || pItem->DLayer2 != 0)
        {
            defs << "set_dlayers|" + std::to_string((int)pItem->DLayer1) + "|" + std::to_string((int)pItem->DLayer2) + "|\n";
        }

        if (pItem->flags != 0)
        {
            std::string bits = "";

            if (pItem->flags & ROBOT_DEADLY)
                bits.append(ItemFlag2ToString(ROBOT_DEADLY) + "|");
            if (pItem->flags & ROBOT_SHOOT_LEFT)
                bits.append(ItemFlag2ToString(ROBOT_SHOOT_LEFT) + "|");
            if (pItem->flags & ROBOT_SHOOT_RIGHT)
                bits.append(ItemFlag2ToString(ROBOT_SHOOT_RIGHT) + "|");
            if (pItem->flags & ROBOT_SHOOT_DOWN)
                bits.append(ItemFlag2ToString(ROBOT_SHOOT_DOWN) + "|");
            if (pItem->flags & ROBOT_SHOOT_UP)
                bits.append(ItemFlag2ToString(ROBOT_SHOOT_UP) + "|");
            if (pItem->flags & ROBOT_CAN_SHOOT)
                bits.append(ItemFlag2ToString(ROBOT_CAN_SHOOT) + "|");
            if (pItem->flags & ROBOT_LAVA)
                bits.append(ItemFlag2ToString(ROBOT_LAVA) + "|");
            if (pItem->flags & ROBOT_POINTY)
                bits.append(ItemFlag2ToString(ROBOT_POINTY) + "|");
            if (pItem->flags & ROBOT_SHOOT_DEADLY)
                bits.append(ItemFlag2ToString(ROBOT_SHOOT_DEADLY) + "|");
            if (pItem->flags & GUILD_ITEM)
                bits.append(ItemFlag2ToString(GUILD_ITEM) + "|");
            if (pItem->flags & GUILD_FLAG)
                bits.append(ItemFlag2ToString(GUILD_FLAG) + "|");
            if (pItem->flags & STARSHIP_HELP)
                bits.append(ItemFlag2ToString(STARSHIP_HELP) + "|");
            if (pItem->flags & STARSHIP_REACTOR)
                bits.append(ItemFlag2ToString(STARSHIP_REACTOR) + "|");
            if (pItem->flags & STARSHIP_VIEWSCREEN)
                bits.append(ItemFlag2ToString(STARSHIP_VIEWSCREEN) + "|");
            if (pItem->flags & SUPERMOD)
                bits.append(ItemFlag2ToString(SUPERMOD) + "|");
            if (pItem->flags & TILE_DEADLY_IF_ON)
                bits.append(ItemFlag2ToString(TILE_DEADLY_IF_ON) + "|");
            if (pItem->flags & LONG_HAND_ITEM)
                bits.append(ItemFlag2ToString(LONG_HAND_ITEM) + "|");
            if (pItem->flags & GEMLESS)
                bits.append(ItemFlag2ToString(GEMLESS) + "|");
            if (pItem->flags & TRANSMUTABLE)
                bits.append(ItemFlag2ToString(TRANSMUTABLE) + "|");
            if (pItem->flags & DUNGEON_ITEM)
                bits.append(ItemFlag2ToString(DUNGEON_ITEM) + "|");
            if (pItem->flags & PVE_MELEE)
                bits.append(ItemFlag2ToString(PVE_MELEE) + "|");
            if (pItem->flags & PVE_RANGED)
                bits.append(ItemFlag2ToString(PVE_RANGED) + "|");
            if (pItem->flags & PVE_AUTO_AIM)
                bits.append(ItemFlag2ToString(PVE_AUTO_AIM) + "|");
            if (pItem->flags & ONE_IN_WORLD)
                bits.append(ItemFlag2ToString(ONE_IN_WORLD) + "|");
            if (pItem->flags & ONLY_FOR_WORLD_OWNER)
                bits.append(ItemFlag2ToString(ONLY_FOR_WORLD_OWNER) + "|");

            defs << std::format("set_flags2|{}\n", bits);
        }

        std::string v9 = "";
        for (uint8_t i = 0; i < 60; i++)
        {
            v9.append(std::format("{},", (int)pItem->unwantedV9[i]));
        }

        v9.resize(v9.size() - 1);
        defs << "set_unknown9|" + v9 + "\n";

        if (pItem->tileRange != 0 || pItem->pileRange != 0)
        {
            defs << "set_ranges|" + std::to_string((int)pItem->tileRange) + "|" + std::to_string((int)pItem->pileRange) + "|\n";
        }

        if (!pItem->customPunch.empty())
        {
            defs << "set_custom_punch|" + pItem->customPunch + "|\n";
        }

        if (pItem->fxFlags != 0)
        {
            std::string fxBits = "";

            if (pItem->fxFlags & MULTI_ANIM_START)
                fxBits.append(ItemFxFlagToString(MULTI_ANIM_START) + "|");
            if (pItem->fxFlags & PING_PONG_ANIM)
                fxBits.append(ItemFxFlagToString(PING_PONG_ANIM) + "|");
            if (pItem->fxFlags & OVERLAY_OBJECT)
                fxBits.append(ItemFxFlagToString(OVERLAY_OBJECT) + "|");
            if (pItem->fxFlags & OFFSET_UP)
                fxBits.append(ItemFxFlagToString(OFFSET_UP) + "|");
            if (pItem->fxFlags & DUAL_LAYER)
                fxBits.append(ItemFxFlagToString(DUAL_LAYER) + "|");
            if (pItem->fxFlags & MULTI_ANIM2_START)
                fxBits.append(ItemFxFlagToString(MULTI_ANIM2_START) + "|");
            if (pItem->fxFlags & UNK_0x40)
                fxBits.append(ItemFxFlagToString(UNK_0x40) + "|");
            if (pItem->fxFlags & USE_SKIN_TINT)
                fxBits.append(ItemFxFlagToString(USE_SKIN_TINT) + "|");
            if (pItem->fxFlags & SEED_TINT_LAYER1)
                fxBits.append(ItemFxFlagToString(SEED_TINT_LAYER1) + "|");
            if (pItem->fxFlags & SEED_TINT_LAYER2)
                fxBits.append(ItemFxFlagToString(SEED_TINT_LAYER2) + "|");
            if (pItem->fxFlags & RAINBOW_TINT_LAYER1)
                fxBits.append(ItemFxFlagToString(RAINBOW_TINT_LAYER1) + "|");
            if (pItem->fxFlags & RAINBOW_TINT_LAYER2)
                fxBits.append(ItemFxFlagToString(RAINBOW_TINT_LAYER2) + "|");
            if (pItem->fxFlags & GLOW)
                fxBits.append(ItemFxFlagToString(GLOW) + "|");
            if (pItem->fxFlags & NO_ARMS)
                fxBits.append(ItemFxFlagToString(NO_ARMS) + "|");
            if (pItem->fxFlags & FRONT_ARM_PUNCH)
                fxBits.append(ItemFxFlagToString(FRONT_ARM_PUNCH) + "|");
            if (pItem->fxFlags & RENDER_OFFHAND)
                fxBits.append(ItemFxFlagToString(RENDER_OFFHAND) + "|");
            if (pItem->fxFlags & SLOWFALL_OBJECT)
                fxBits.append(ItemFxFlagToString(SLOWFALL_OBJECT) + "|");
            if (pItem->fxFlags & REPLACEMENT_SPRITE)
                fxBits.append(ItemFxFlagToString(REPLACEMENT_SPRITE) + "|");
            if (pItem->fxFlags & ORB_FLOAT)
                fxBits.append(ItemFxFlagToString(ORB_FLOAT) + "|");
            if (pItem->fxFlags & UNK_0x80000)
                fxBits.append(ItemFxFlagToString(UNK_0x80000) + "|");
            if (pItem->fxFlags & RENDER_FX_VARIANT_VERSION)
                fxBits.append(ItemFxFlagToString(RENDER_FX_VARIANT_VERSION) + "|");

            defs << std::format("set_fx_flags|{}\n", fxBits);
        }

        std::string parts = "";
        for (uint8_t i = 0; i < 9; i++)
        {
            parts.append(std::format("{},", (int)pItem->bodyParts[i]));
        }

        parts.resize(parts.size() - 1);
        defs << "set_body_parts|" + parts + "\n";

        if (pItem->clockDivider != 0)
        {
            defs << "set_clock|" + std::to_string((int)pItem->clockDivider) + "|\n";
        }

        if (pItem->parentID != 0)
        {
            defs << "set_parent|" + std::to_string((int)pItem->parentID) + "|\n";
        }

        if (!pItem->sitTexture.empty())
        {
            defs << "set_sit_info|" + std::to_string((int)pItem->sitable) + "|" + std::to_string((int)pItem->sitOffsetX) + "|" + std::to_string((int)pItem->sitOffsetY) + "|" + std::to_string((int)pItem->sitOverlayX) + "|" + std::to_string((int)pItem->sitOverlayY) + "|" + std::to_string((int)pItem->sitOverlayOffsetX) + "|" + std::to_string((int)pItem->sitOverlayOffsetY) + "|" + pItem->sitTexture + "|\n";
        }

        if (!pItem->rendererFile.empty() || pItem->unwantedV17 != 0 || pItem->rendererHash != 0)
        {
            defs << "set_renderer_info|" + pItem->rendererFile + "|" + std::to_string((int)pItem->unwantedV17) + "|" + std::to_string((int)pItem->rendererHash) + "|\n";
        }

        if (!pItem->desc.empty() && pItem->desc != "This item has no description.")
        {
            defs << "set_desc|" + pItem->desc + "|\n";
        }

        if (pItem->lockPower != 0)
        {
            defs << "set_lock_power|" + std::to_string((int)pItem->lockPower) + "|\n";
        }

        if (pItem->chi != 5)
        {
            defs << "set_chi|" + std::to_string((int)pItem->chi) + "|\n";
        }

        if (pItem->bLocked)
        {
            defs << "is_locked|\n";
        }

        if (pItem->playmodID != -1)
        {
            defs << "link_playmod|" + std::to_string((int)pItem->playmodID) + "|\n";
        }

        defs << "\n";
    }

    defs.close();
    LogMsg("dumped item_definitions.txt");
}

void ItemInfoManager::Serialize(const uint16_t& version)
{
#ifdef _MSC_VER
    g_lastCall = __FUNCSIG__;
#else
    g_lastCall = __PRETTY_FUNCTION__;
#endif

    auto then = std::chrono::system_clock::now();

    size_t len = 6;
    for (int i = 0; i < m_itemCount; i++)
    {
        len += m_items[i]->GetMemoryEstimated(version);
    }

    int offsetIn = 0;
    uint16_t ver = version;
    int items = (int)m_items.size();

    m_packet = (GameUpdatePacket*)std::malloc(sizeof(GameUpdatePacket) + len);
    std::memset(m_packet, 0, sizeof(GameUpdatePacket) + len);

    m_packet->type = NET_GAME_PACKET_SEND_ITEM_DATABASE_DATA;
    m_packet->netID = -1;
    m_packet->flags |= NET_GAME_PACKET_FLAG_EXTENDED;
    m_packet->dataLength = (uint32_t)len;

    MemorySerialize(&ver, m_packet->data, offsetIn, true);
    MemorySerialize(&items, m_packet->data, offsetIn, true);

    for (int i = 0; i < m_itemCount; i++)
    {
        m_items[i]->SerializeToMem(version, m_packet->data, offsetIn);
    }

    m_hash = Utils::HashString(m_packet->data, (uint32_t)len);
    LogMsg("serializing items data(size: %d, hash: %d) took %d milliseconds.", (int)len, m_hash, (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - then).count());
}

ItemInfo* ItemInfoManager::GetItem(const uint16_t& ID)
{
#ifdef _MSC_VER
    g_lastCall = __FUNCSIG__;
#else
    g_lastCall = __PRETTY_FUNCTION__;
#endif
    return (ID >= 0 && ID <= m_items.size()) ? m_items[ID] : nullptr;
}

ItemInfo* ItemInfoManager::GetItemByName(std::string fName) 
{
#ifdef _MSC_VER
    g_lastCall = __FUNCSIG__;
#else
    g_lastCall = __PRETTY_FUNCTION__;
#endif
    std::transform(fName.begin(), fName.end(), fName.begin(), ::tolower);

    for (ItemInfo* item : m_items) 
    {
        std::string name = item->name;
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);

        if (name == fName)
            return item;
    }

    return nullptr;
}

ItemInfo* ItemInfoManager::CreateSeedVersionOfLastAddedItem(const uint16_t& tileID) {
#ifdef _MSC_VER
    g_lastCall = __FUNCSIG__;
#else
    g_lastCall = __PRETTY_FUNCTION__;
#endif
    ItemInfo* pSeed = new ItemInfo();
    ItemInfo* pItem = GetItem(tileID);

    if (pItem)
    {
        std::string name = pItem->name + " Seed";
        if (tileID == 611) 
        {
            name = "Magic Egg";
        }
        if (tileID == 2034) 
        {
            name = "Starseed";
        }
        if (tileID == 2036) 
        {
            name = "Galactic Starseed";
        }
        if (tileID == 4454) 
        {
            name = "Mutated Seed";
        }

        pSeed->name = name;
        pSeed->ID = tileID + 1;
        pSeed->desc = "Plant it to grow a `w" + pItem->name + "``.";
        pSeed->type = TYPE_SEED;
        pSeed->rarity = pItem->rarity;
        pSeed->tileStorage = STORAGE_SINGLE_FRAME_ALONE;
        pSeed->tileCollision = TILE_COLLISION_NONE;
        pSeed->hardness = 120;
        pSeed->bodyPart = HAT;
        pSeed->soundType = 0;
        pSeed->layer = 0;
        pSeed->regenTime = 2;
        pSeed->editableTypes = pItem->editableTypes;
        pSeed->seedColor = pItem->seedColor;
        pSeed->treeColor = pItem->treeColor;

        return pSeed;
    }

    return nullptr;
}

ItemInfo* ItemInfoManager::CreateNullItemOfLastItemAdded(const uint16_t& tileID) {
#ifdef _MSC_VER
    g_lastCall = __FUNCSIG__;
#else
    g_lastCall = __PRETTY_FUNCTION__;
#endif
    return nullptr;
}

uint16_t ItemInfoManager::GetSplicingInfo(const uint16_t& seed1, const uint16_t& seed2)
{
#ifdef _MSC_VER
    g_lastCall = __FUNCSIG__;
#else
    g_lastCall = __PRETTY_FUNCTION__;
#endif

    if (m_splices.empty())
    {
        return -1;
    }

    auto it = std::find_if(m_splices.begin(), m_splices.end(), [&seed1, &seed2](const SplicingRecipe& recipe) { return (recipe.seed1 == seed1 && recipe.seed2 == seed2) || (recipe.seed1 == seed2 && recipe.seed2 == seed1); });
    return it != m_splices.end() ? it->result : -1;
}