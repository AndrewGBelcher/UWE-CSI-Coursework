/*****************************************************************************
*																			 *
* File: ZombieBot.h							     							 *
* Author: Andrew Belcher						     						 *
* Date: 12-11-2017							     							 *
* Desc: ZombieBot class header						 			     		 *
* Copyright: University of West of England 2017				    			 *
*									     									 *
******************************************************************************/

#pragma once

#include <zombies/ZombieBot.h>
#include "world.h"
	

class ZombieBot : public uwe::ZombieBot {

	private:

	public:

		// create the world class
		World world;

		// create the zombie bot and pass it the world
		ZombieBot (World w);

		/**
		 * should game quit
		 * @return return true if exit program, otherwise false
		 */
		bool shouldQuit() override;

		/**
		 * text to be displayed at beginning of game
		 * @return text to be displayed
		 */
		std::string begin() override;

		/**
		 * compute current score
		 * @return current score
		 */
		int currentScore() override;

		/**
		 * should zombie timer be enabled
		 * @return true if timer should be enabled, otherwise false
		 */
		bool enableTimer() override;

		/**
		 * should zombie timer be disabled
		 * @return true if timer should be disabled, otherwise false
		 */
		bool disableTimer() override;

		/**
		 * process player commands.<br><br>
		 * The set of commands are as follows:<br>
		 *<ul>
		 * <li>info     - return the info string for the world.</li>
		 * <li>
		 *    look     - return the HTML representing the view of the current room.
		 *              this should include entrances, items, and so on.<br>
		 * </li>
		 * <li>
		 *    move dir - try and move in the direction dir. if there is an extrance
		 *               in that direction, then if it is not locked, move through
		 *               to the connected room, return message about new room. if
		 *               can't move, because no entrance in that direction, or
		 *               entrance is locked, then return a message to that
		 *               effect.<br>
		 * </li>
		 * <li>
		 *               one entry to a room if it contains zombies, then the
		 *               next time the method enableTimer() is called it should
		 *               return true, just the one time. This will cause the
		 *               client to start the Zombie kill timer event, displaying
		 *               a Zombie and timer, to show the player that they will die
		 *               if the zombie(s) is not killed in time.<br>
		 * </li>
		 * <li>
		 *    pickup item - pick up item from room, if item exits then this should
		 *                  cause it to be removed from room and get added to
		 *                  players inventory, and return message to say picked up.
		 *                  if it does not exist, then return message to that
		 *                  effect.<br>
		 *                  The users score should be increased by one, each time
		 *                  an item is picked up.
		 * </li>
		 * <li>
		 *    kill      - if the room contains at least one zombie and players
		 *                inventory contains either a Daisy or Chainsaw,
		 *                then one should be killed (i.e. zombie count drops by one),
		 *                return a message to inform the user that a zombie has died.
		 *                if kill is succcessful, then the player's inventory should
		 *                have one less Daisy or Chainsaw.<br>
		 *                if no zombies, then nothing happens.
		 * </li>
		 * <li>
		 *                if the zombie timer was running, i.e. a call to
		 *                enableTimer() had returned true for the current room,
		 *                then if the zombie count is now zero, then disableTimer()
		 *                should return true, one time, to tell the client to
		 *                stop the zombie timer.<br>
		 * </li>
		 * <li>
		 *    drop item - drop an item into room from inventory. if the item is
		 *                not in inventory, then nothing happens, otherwise it
		 *                should be removed, when added to room. return a message
		 *                to say item dropped.<br>
		 * </li>
		 * <li>
		 *  timerexpired - the client's zombie timer expired before all zombies
		 *                   in the current room were killed and so the player is
		 *                   dead. shouldQuit() should now return true.
		 * </li>
		 * <li>
		 *    quit         - shouldQuit() should now return true.
		 *</li>
		 * <li>
		 *    inventory    - return HTML to display the players current inventory.
		 *</li>
		 * <li>
		 *    anything else - return HTML saying "That's not a verb I recognise."
		 *</li>
		 * </ul>
		 * @param cmd to be processed
		 * @return output to be displayed. each element in the list returned is
		 * in HTML and will be output in the clients display window
		 */

		std::vector<std::string>processCmd (std::string cmd) override;
};
