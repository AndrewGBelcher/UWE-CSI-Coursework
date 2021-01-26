/*****************************************************************************
*																			 *
* File: item.cpp							     							 *
* Author: Andrew Belcher						     						 *
* Date: 12-11-2017							     							 *
* Desc: item class 									 			     		 *
* Copyright: University of West of England 2017				    			 *
*									     									 *
******************************************************************************/

// our custom headers
#include "item.h"

// our default contructor
Item::Item(){}

// construct our item class with its details
Item::Item (std::string name, std::string itemDesc){

	itemName = name;
	itemHtml = itemDesc;
	held = false;

}

// method to see if this item has been picked up before *ie point abuse prevention
bool Item::chkHeld(){
	return held;
}

// method to set if the item has been picked up once so it cant be counted for points again
void Item::setHeld(){
	held = true;
}

// method to return the items name
std::string Item::getName() {
    return itemName;
}

// method to return the items corresponding html
std::string Item::getHtml() {

    std::string returnString = "";
    returnString += itemHtml + " (" + "<b> " + itemName + " </b>";

    return returnString;
}






