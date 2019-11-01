#ifndef __InfoTabDropItem_h__
#define __InfoTabDropItem_h__



#include <Arduino.h>
#include <Web\InfoTabItem.h>


class InfoTabDropItem : public InfoTabItem 
{

private:
	//TODO:: MAKE DYNAMIC
	static const int ITEMS = 10;

	InfoTabItem* items[ITEMS] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	int count = 0;
public:
	//InfoTabDropItem(String id, String description, PageContentsCallback callback, void *object);
	using InfoTabItem::InfoTabItem;

	void addDropItem(InfoTabItem* item) ;
	
	bool isDropItem() override {return true;}

	int GetDropItemCount() {return count;}

	InfoTabItem* GetDropItemAt(int idx) override;

};

#endif //__InfoTabDropItem_h__

