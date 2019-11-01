#include <Arduino.h>

#include <Web\InfoTabItem.h>
#include <Web\InfoTabDropItem.h>

/*
InfoTabDropItem::InfoTabDropItem(String id, String description, PageContentsCallback callback, void *object) 
    : InfoTabDropItem(id, description, callback, object)
{

}

*/

void InfoTabDropItem::addDropItem(InfoTabItem* item)
{
    if (count<ITEMS) {
        items[count] = item;
        count++;
    }
}
	
InfoTabItem* InfoTabDropItem::GetDropItemAt(int idx)
{
    if ((idx<InfoTabDropItem::ITEMS) && idx>-1) 
    {
        return items[idx];
    }
    return NULL;
};
