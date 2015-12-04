//
//  KeyProperty.cpp
//  Voxel_Explorer
//
//  Created by wang haibo on 15/10/24.
//
//

#include "KeyProperty.hpp"
#include "UtilityHelper.h"
USING_NS_CC;

KeyProperty::KeyProperty(unsigned int instanceID, PickableItem::PickableItemType type)
    :PickableItemProperty(instanceID, type)
{
    m_PropertyType = PIPT_KEY;
    m_bIdentified = true;
    m_bStackable = true;
    m_bEquipable = false;
    m_bCombinable = true;
    m_bDiscardable = true;
    
    m_strPropertyTypeName = UtilityHelper::getLocalString(PICKABLE_ITEM_PROPERTY_TYPE_NAMES[m_PropertyType]);
    
    if(m_ItemType == PickableItem::PIT_KEY_COPPER)
        m_nValueCopper = 10;
    else if(m_ItemType == PickableItem::PIT_KEY_SILVER)
        m_nValueCopper = 50;
    else if(m_ItemType == PickableItem::PIT_KEY_GOLD)
        m_nValueCopper = 100;
    else if(m_ItemType == PickableItem::PIT_KEY_ROOM)
    {
        m_bDiscardable = false;
        m_nValueCopper = 0;
    }
    else if(m_ItemType == PickableItem::PIT_KEY_BOSS)
    {
        m_bStackable = false;
        m_bDiscardable = false;
        m_nValueCopper = 0;
    }
    m_nCopperWhenBuy = m_nValueCopper * 1.5f;
}

void KeyProperty::adjustByLevel()
{
}
void KeyProperty::handleIdentify()
{
    if(m_bIdentified)
        return;
}