/*****************************************************************************
*																			 *
* File: item.h								     							 *
* Author: Andrew Belcher						     						 *
* Date: 12-11-2017							     							 *
* Desc: item class header							 			     		 *
* Copyright: University of West of England 2017				    			 *
*									     									 *
******************************************************************************/

# pragma once

#include <ufcfgl-30-1.h>


class Item{

	private:
		
		// protect our variables from the user
		std::string itemName;
		std::string itemHtml;
		std::string name;
		bool held;

	public:

		// our class constructors
		Item();
		Item(std::string name, std::string itemDesc);

		// methods for getting item information
		std::string getHtml();
		std::string getName();

		// point abuse prevention methods
		bool chkHeld();
		void setHeld();

};
