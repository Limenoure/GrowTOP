#include <BaseApp.hpp> //precomp

#include <Items/ItemInfo.hpp>
#include <ProtonSDK/MiscUtils.h>



size_t ItemInfo::GetMemoryEstimated(const uint16_t& version) 
{
    size_t len = 0;

    len += sizeof(uint32_t);
    len += sizeof(uint16_t);
    len += sizeof(uint8_t);
    len += sizeof(uint8_t);

    std::string itemName = name;
    if (version >= 3)
    {
        itemName = EncryptItemNameString(name, ID < 0 ? itemID : ID);
    }

    len += sizeof(uint16_t) + itemName.length();

    len += sizeof(uint16_t) + texture.length();
    len += sizeof(uint32_t);

    len += sizeof(uint8_t);
    len += sizeof(uint32_t);
    len += sizeof(uint8_t);
    len += sizeof(uint8_t);

    len += sizeof(uint8_t);
    len += sizeof(uint8_t);
    len += sizeof(uint8_t);

    len += sizeof(uint8_t);
    len += sizeof(uint32_t);
    len += sizeof(uint8_t);
    len += sizeof(uint16_t);
    len += sizeof(uint8_t);

    len += sizeof(uint16_t) + textureExtra.length();
    len += sizeof(uint32_t);
    len += sizeof(uint32_t);

    if (version >= 4)
    {
        len += sizeof(uint16_t) + petName.length();
        len += sizeof(uint16_t) + petPrefix.length();
        len += sizeof(uint16_t) + petSuffix.length();
    }

    if (version >= 5)
        len += sizeof(uint16_t) + petAbility.length();

    len += sizeof(uint8_t);
    len += sizeof(uint8_t);
    len += sizeof(uint8_t);
    len += sizeof(uint8_t);
    len += sizeof(uint32_t);
    len += sizeof(uint32_t);
    len += sizeof(uint16_t);
    len += sizeof(uint16_t);
    len += sizeof(uint32_t);

    if (version >= 7)
    {
        len += sizeof(uint32_t);
        len += sizeof(uint16_t) + animString.length();
    }

    if (version >= 8)
    {
        len += sizeof(uint16_t) + animTexture.length();
        len += sizeof(uint16_t) + animString2.length();
        len += sizeof(uint32_t);
        len += sizeof(uint32_t);
    }

    if (version >= 9)
    {
        len += sizeof(uint32_t);
        len += 60;
    }

    if (version >= 10)
    {
        len += sizeof(uint32_t);
        len += sizeof(uint32_t);
    }

    if (version >= 11)
        len += sizeof(uint16_t) + customPunch.length();

    if (version >= 12)
    {
        len += sizeof(uint32_t);
        len += 9;
    }

    if (version >= 13)
        len += sizeof(uint32_t);

    if (version >= 14)
        len += sizeof(uint32_t);

    if (version >= 15)
    {
        len += sizeof(uint8_t);
        len += sizeof(uint32_t);
        len += sizeof(uint32_t);
        len += sizeof(uint32_t);
        len += sizeof(uint32_t);
        len += sizeof(uint32_t);
        len += sizeof(uint32_t);
        len += sizeof(uint16_t) + sitTexture.length();
    }

    if (version >= 16)
        len += sizeof(uint16_t) + rendererFile.length();

    if (version >= 17)
        len += sizeof(unwantedV17);

    if (version >= 18)
        len += sizeof(rendererHash);

    return len;
}

void ItemInfo::SerializeToMem(const uint16_t& version, uint8_t* pMem, int& offsetInOut) 
{
    bool bWriteToMem = true;

    if (ID < 0)
        MemorySerialize(&itemID, pMem, offsetInOut, bWriteToMem);
    else
        MemorySerialize(&ID, pMem, offsetInOut, bWriteToMem);

    MemorySerialize(&editableTypes, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&type, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&soundType, pMem, offsetInOut, bWriteToMem);

    std::string itemName = name;
    if (version >= 3)
    {
        itemName = EncryptItemNameString(name, ID < 0 ? itemID : ID);
    }
       
    MemorySerialize(itemName, pMem, offsetInOut, bWriteToMem);

    MemorySerialize(texture, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&textureHash, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&visualType, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&cookingTime, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&textureX, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&textureY, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&tileStorage, pMem, offsetInOut, bWriteToMem);

    MemorySerialize(&layer, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&tileCollision, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&hardness, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&regenTime, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&bodyPart, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&rarity, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&maxCount, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(textureExtra, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&textureExtraHash, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&animMS, pMem, offsetInOut, bWriteToMem);

    if (version >= 4) 
    {
        MemorySerialize(petName, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(petPrefix, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(petSuffix, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 5)
        MemorySerialize(petAbility, pMem, offsetInOut, bWriteToMem);

    MemorySerialize(&seedBase, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&seedOver, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&treeBase, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&treeOver, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&seedColor, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&treeColor, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&seed1, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&seed2, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&bloomTime, pMem, offsetInOut, bWriteToMem);

    if (version >= 7) 
    {
        MemorySerialize(&animationType, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(animString, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 8) 
    {
        MemorySerialize(animTexture, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(animString2, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&DLayer1, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&DLayer2, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 9) 
    {
        MemorySerialize(&flags, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(&unwantedV9[0], pMem, 60, offsetInOut, bWriteToMem);
    }

    if (version >= 10) 
    {
        MemorySerialize(&tileRange, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&pileRange, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 11)
        MemorySerialize(customPunch, pMem, offsetInOut, bWriteToMem);

    if (version >= 12) 
    {
        MemorySerialize(&fxFlags, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(&bodyParts[0], pMem, 9, offsetInOut, bWriteToMem);
    }

    if (version >= 13) 
        MemorySerialize(&clockDivider, pMem, offsetInOut, bWriteToMem);

    if (version >= 14)
        MemorySerialize(&parentID, pMem, offsetInOut, bWriteToMem);

    if (version >= 13) 
    {
        MemorySerialize(&sitable, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&sitOffsetX, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&sitOffsetY, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&sitOverlayX, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&sitOverlayY, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&sitOverlayOffsetX, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&sitOverlayOffsetY, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(sitTexture, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 16) 
         MemorySerialize(rendererFile, pMem, offsetInOut, bWriteToMem);

    if (version >= 17)
        MemorySerialize(&unwantedV17, pMem, offsetInOut, bWriteToMem);

    if (version >= 18)
        MemorySerialize(&rendererHash, pMem, offsetInOut, bWriteToMem);
}

void ItemInfo::SerializeFromMem(const uint16_t& version, uint8_t* pMem, int& offsetInOut) 
{
    bool bWriteToMem = false;

    MemorySerialize(&ID, pMem, offsetInOut, bWriteToMem);
    switch (ID) 
    {
    case ITEM_ID_SMALL_LOCK: { lockPower = 10; } break;
    case ITEM_ID_BIG_LOCK: { lockPower = 48; } break;
    case ITEM_ID_HUGE_LOCK: case ITEM_ID_BUILDER_S_LOCK: { lockPower = 200; } break;
    default: lockPower = 0; break;
    }

    MemorySerialize(&editableTypes, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&type, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&soundType, pMem, offsetInOut, bWriteToMem);

    if (version >= 3)
        MemorySerializeStringEncrypted(name, pMem, offsetInOut, bWriteToMem, ID, "PBG892FXX982ABC*");
    else
        MemorySerialize(name, pMem, offsetInOut, bWriteToMem);

    MemorySerialize(texture, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&textureHash, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&visualType, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&cookingTime, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&textureX, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&textureY, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&tileStorage, pMem, offsetInOut, bWriteToMem);

    MemorySerialize(&layer, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&tileCollision, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&hardness, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&regenTime, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&bodyPart, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&rarity, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&maxCount, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(textureExtra, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&textureExtraHash, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&animMS, pMem, offsetInOut, bWriteToMem);

    if (version >= 4)
    {
        MemorySerialize(petName, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(petPrefix, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(petSuffix, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 5)
        MemorySerialize(petAbility, pMem, offsetInOut, bWriteToMem);

    MemorySerialize(&seedBase, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&seedOver, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&treeBase, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&treeOver, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&seedColor, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&treeColor, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&seed1, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&seed2, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(&bloomTime, pMem, offsetInOut, bWriteToMem);

    if (version >= 7)
    {
        MemorySerialize(&animationType, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(animString, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 8)
    {
        MemorySerialize(animTexture, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(animString2, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&DLayer1, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&DLayer2, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 9)
    {
        MemorySerialize(&flags, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(&unwantedV9[0], pMem, 60, offsetInOut, bWriteToMem);
    }

    if (version >= 10)
    {
        MemorySerialize(&tileRange, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&pileRange, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 11)
        MemorySerialize(customPunch, pMem, offsetInOut, bWriteToMem);

    if (version >= 12)
    {
        MemorySerialize(&fxFlags, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(&bodyParts[0], pMem, 9, offsetInOut, bWriteToMem);
    }

    if (version >= 13)
        MemorySerialize(&clockDivider, pMem, offsetInOut, bWriteToMem);

    if (version >= 14)
        MemorySerialize(&parentID, pMem, offsetInOut, bWriteToMem);

    if (version >= 13)
    {
        MemorySerialize(&sitable, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&sitOffsetX, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&sitOffsetY, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&sitOverlayX, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&sitOverlayY, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&sitOverlayOffsetX, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(&sitOverlayOffsetY, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(sitTexture, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 16)
        MemorySerialize(rendererFile, pMem, offsetInOut, bWriteToMem);

    if (version >= 17)
        MemorySerialize(&unwantedV17, pMem, offsetInOut, bWriteToMem);

    if (version >= 18)
        MemorySerialize(&rendererHash, pMem, offsetInOut, bWriteToMem);
}