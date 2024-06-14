#include "MenuManager.h"
#include <EventSystem.h>
#include "GameEvent.h"

MenuManager::MenuManager(GraphicsSystem& graphicsSystem)
{
	mGraphicsSystem = &graphicsSystem;
	mShouldQuit = false;
	mIsMenuOpen = true;
	mCurrentMenu = 0;
	mCurrentDifficulty = 1;
	mCurrentGridSize = 0;
}

MenuManager::~MenuManager()
{
}

void MenuManager::loadColorData(Color& text)
{
	mTextColor = text;
}

void MenuManager::loadFontData(string assetPath, string fontName, int menuFontSize, int smallMenuFontSize)
{
	mAssetPath = assetPath;
	mFontName = fontName;
	mMenuFontSize = menuFontSize;
	mSmallMenuFontSize = smallMenuFontSize;
	mTextBuffer = menuFontSize + 20;
}

void MenuManager::draw()
{
	Font menuFont(mAssetPath + mFontName, mMenuFontSize);
	Font smallMenuFont(mAssetPath + mFontName, mSmallMenuFontSize);
	if (mIsMenuOpen)
	{
		if (mCurrentMenu == 0)
		{
			//MAIN MENU
			Vector2D loc1(300, 100);
			string text1 = mTextMap["Menu0_1"];
			string text2 = mTextMap["Menu0_2"]; //START
			string text3 = mTextMap["Menu0_3"]; //LOAD
			string text4 = mTextMap["Menu0_4"]; //OPTIONS
			string text5 = mTextMap["Menu0_5"]; //QUIT

			mGraphicsSystem->writeTextToBackbuffer(loc1, menuFont, mTextColor, text1, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + mTextBuffer), menuFont, mTextColor, text2, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + (mTextBuffer * 2)), menuFont, mTextColor, text3, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + (mTextBuffer * 3)), menuFont, mTextColor, text4, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + (mTextBuffer * 4)), menuFont, mTextColor, text5, false);
			mGraphicsSystem->drawButtons(4, 70, 40, 250, 180, mTextColor);
		}

		if (mCurrentMenu == 1)
		{
			//PAUSE SCREEN
			Vector2D loc1(300, 200);
			Vector2D loc2(250, 300);
			Vector2D loc3(300, 340);
			string text1 = mTextMap["Menu1_1"];
			string text2 = mTextMap["Menu1_2"];
			string text3 = mTextMap["Menu1_3"];

			mGraphicsSystem->writeTextToBackbuffer(loc1, menuFont, mTextColor, text1, false);
			mGraphicsSystem->writeTextToBackbuffer(loc2, smallMenuFont, mTextColor, text2, false);
			mGraphicsSystem->writeTextToBackbuffer(loc3, smallMenuFont, mTextColor, text3, false);
		}

		if (mCurrentMenu == 2)
		{
			//GAME OVER SCREEN
			Vector2D loc1(250, 200);
			Vector2D loc2(270, 300);
			string text1 = mTextMap["Menu2_1"];
			string text2 = mTextMap["Menu2_2"];

			mGraphicsSystem->writeTextToBackbuffer(loc1, menuFont, mTextColor, text1, false);
			mGraphicsSystem->writeTextToBackbuffer(loc2, smallMenuFont, mTextColor, text2, false);
		}

		if (mCurrentMenu == 3)
		{
			//OPTIONS MENU
			Vector2D loc1(300, 100);
			string text1 = mTextMap["Menu3_1"];
			string text2 = mTextMap["Menu3_2"]; //DIFFICULTY
			string text3 = mTextMap["Menu3_3"]; //GRID SIZE
			string text4 = mTextMap["Menu3_4"]; //SOUND
			string text5 = mTextMap["Menu3_5"]; //BACK

			mGraphicsSystem->writeTextToBackbuffer(loc1, menuFont, mTextColor, text1, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + mTextBuffer), menuFont, mTextColor, text2, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + (mTextBuffer * 2)), menuFont, mTextColor, text3, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + (mTextBuffer * 3)), menuFont, mTextColor, text4, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + (mTextBuffer * 4)), menuFont, mTextColor, text5, false);
			mGraphicsSystem->drawButtons(4, 70, 40, 250, 180, mTextColor);
		}

		if (mCurrentMenu == 4)
		{
			//DIFFICULTY MENU
			Vector2D loc1(300, 100);
			string text1;
			string text2 = mTextMap["Menu4_2"]; //EASY
			string text3 = mTextMap["Menu4_3"]; //NORMAL
			string text4 = mTextMap["Menu4_4"]; //HARD
			string text5 = mTextMap["Menu4_5"]; //BACK

			if (mCurrentDifficulty == 0) { text1 = mTextMap["Menu4_1"] + text2; }
			if (mCurrentDifficulty == 1) { text1 = mTextMap["Menu4_1"] + text3; }
			if (mCurrentDifficulty == 2) { text1 = mTextMap["Menu4_1"] + text4; }

			mGraphicsSystem->writeTextToBackbuffer(loc1 - Vector2D(50), menuFont, mTextColor, text1, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + mTextBuffer), menuFont, mTextColor, text2, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + (mTextBuffer * 2)), menuFont, mTextColor, text3, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + (mTextBuffer * 3)), menuFont, mTextColor, text4, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + (mTextBuffer * 4)), menuFont, mTextColor, text5, false);
			mGraphicsSystem->drawButtons(4, 70, 40, 250, 180, mTextColor);
		}

		if (mCurrentMenu == 5)
		{
			//Grid Size Menu
			Vector2D loc1(300, 100);
			string text1;
			string text2 = mTextMap["Menu5_2"]; //9x9
			string text3 = mTextMap["Menu5_3"]; //6x6
			string text4 = mTextMap["Menu5_4"]; //BACK

			if (mCurrentGridSize == 0) { text1 = mTextMap["Menu5_1"] + text2; }
			if (mCurrentGridSize == 1) { text1 = mTextMap["Menu5_1"] + text3; }

			mGraphicsSystem->writeTextToBackbuffer(loc1 - Vector2D(50), menuFont, mTextColor, text1, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + mTextBuffer), menuFont, mTextColor, text2, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + (mTextBuffer * 2)), menuFont, mTextColor, text3, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + (mTextBuffer * 3)), menuFont, mTextColor, text4, false);
			mGraphicsSystem->drawButtons(3, 70, 40, 250, 180, mTextColor);
		}

		if (mCurrentMenu == 6)
		{
			// Sound Menu
			Vector2D loc1(300, 100);
			string text1;
			string text2 = mTextMap["Menu6_2"]; //ON
			string text3 = mTextMap["Menu6_3"]; //OFF
			string text4 = mTextMap["Menu6_4"]; //BACK

			if (mIsSoundOn) { text1 = mTextMap["Menu6_1"] + text2; }
			if (!mIsSoundOn) { text1 = mTextMap["Menu6_1"] + text3; }

			mGraphicsSystem->writeTextToBackbuffer(loc1 - Vector2D(50), menuFont, mTextColor, text1, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + mTextBuffer), menuFont, mTextColor, text2, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + (mTextBuffer * 2)), menuFont, mTextColor, text3, false);
			mGraphicsSystem->writeTextToBackbuffer(Vector2D(loc1.getX(), loc1.getY() + (mTextBuffer * 3)), menuFont, mTextColor, text4, false);
			mGraphicsSystem->drawButtons(3, 70, 40, 250, 180, mTextColor);
		}

		if (mCurrentMenu == 7)
		{
			//WIN SCREEN
			Vector2D loc1(250, 200);
			Vector2D loc2(200, 300);
			Vector2D loc3(250, 400);
			string text1 = mTextMap["Menu7_1"];
			string text2 = mTextMap["Menu7_2"] + to_string(mScore);
			string text3 = mTextMap["Menu7_3"];

			mGraphicsSystem->writeTextToBackbuffer(loc1, menuFont, mTextColor, text1, false);
			mGraphicsSystem->writeTextToBackbuffer(loc2, menuFont, mTextColor, text2, false);
			mGraphicsSystem->writeTextToBackbuffer(loc3, smallMenuFont, mTextColor, text3, false);
		}
	}
}

void MenuManager::toggleMenu()
{
	//1 is the pause menu so if the pause menu is open we can toggle it to being off
	//otherwise if no menu is open when this is called we toggle the menu system to come on
	if (mIsMenuOpen == true && mCurrentMenu == 1)
	{
		mIsMenuOpen = false;
	}
	else
	{
		mIsMenuOpen = true;
	}
}

void MenuManager::checkInput(Vector2D loc)
{
	EventSystem* pEventSystem = EventSystem::getInstance();

	float y1 = 180;
	float y2 = 220;
	float spacing = 70;

	//MAIN MENU
	if (mCurrentMenu == 0)
	{
		if (loc.getX() > 250 && loc.getX() < 290)
		{
			if (loc.getY() > y1 && loc.getY() < y2)
			{
				//START
				mIsMenuOpen = false;
				mCurrentMenu++;
				GameEvent gameEvent(NEW_GAME_EVENT);
				pEventSystem->fireEvent(gameEvent);
			}

			if (loc.getY() > y1 + spacing && loc.getY() < y2 + spacing)
			{
				//LOAD
				mIsMenuOpen = false;
				mCurrentMenu++;
			}

			if (loc.getY() > y1 + (spacing * 2) && loc.getY() < y2 + (spacing * 2))
			{
				//OPTIONS
				mCurrentMenu = 3;
			}

			if (loc.getY() > y1 + (spacing * 3) && loc.getY() < y2 + (spacing * 3))
			{
				//QUIT
				mShouldQuit = true;
				GameEvent gameEvent(QUITTING_EVENT);
				pEventSystem->fireEvent(gameEvent);
			}
		}

		return;
	}

	//OPTIONS MENU
	if (mCurrentMenu == 3)
	{
		if (loc.getX() > 250 && loc.getX() < 290)
		{
			if (loc.getY() > y1 && loc.getY() < y2)
			{
				//DIFFICULTY
				mCurrentMenu = 4;
			}

			if (loc.getY() > y1 + spacing && loc.getY() < y2 + spacing)
			{
				//GRID SIZE
				mCurrentMenu = 5;
			}

			if (loc.getY() > y1 + (spacing * 2) && loc.getY() < y2 + (spacing * 2))
			{
				//SOUND
				mCurrentMenu = 6;
			}

			if (loc.getY() > y1 + (spacing * 3) && loc.getY() < y2 + (spacing * 3))
			{
				//BACK
				GameEvent gameEvent(QUITTING_EVENT);
				pEventSystem->fireEvent(gameEvent);
			}
		}
		return;
	}

	//DIFFICULTY MENU
	if (mCurrentMenu == 4)
	{
		if (loc.getX() > 250 && loc.getX() < 290)
		{
			if (loc.getY() > y1 && loc.getY() < y2)
			{
				//EASY
				mCurrentDifficulty = 0;
				GameEvent gameEvent(CHANGE_DIFFICULTY_EVENT);
				pEventSystem->fireEvent(gameEvent);
			}

			if (loc.getY() > y1 + spacing && loc.getY() < y2 + spacing)
			{
				//NORMAL
				mCurrentDifficulty = 1;
				GameEvent gameEvent(CHANGE_DIFFICULTY_EVENT);
				pEventSystem->fireEvent(gameEvent);
			}

			if (loc.getY() > y1 + (spacing * 2) && loc.getY() < y2 + (spacing * 2))
			{
				//HARD
				mCurrentDifficulty = 2;
				GameEvent gameEvent(CHANGE_DIFFICULTY_EVENT);
				pEventSystem->fireEvent(gameEvent);
			}

			if (loc.getY() > y1 + (spacing * 3) && loc.getY() < y2 + (spacing * 3))
			{
				//BACK
				mCurrentMenu = 3;
			}
		}
		return;
	}

	//GRID SIZE MENU
	if (mCurrentMenu == 5)
	{
		if (loc.getX() > 250 && loc.getX() < 290)
		{
			if (loc.getY() > y1 && loc.getY() < y2)
			{
				//9x9
				mCurrentGridSize = 0;
				GameEvent gameEvent(CHANGE_GRID_SIZE_EVENT);
				pEventSystem->fireEvent(gameEvent);
			}

			if (loc.getY() > y1 + spacing && loc.getY() < y2 + spacing)
			{
				//6x6
				mCurrentGridSize = 1;
				GameEvent gameEvent(CHANGE_GRID_SIZE_EVENT);
				pEventSystem->fireEvent(gameEvent);
			}
			if (loc.getY() > y1 + (spacing * 2) && loc.getY() < y2 + (spacing * 2))
			{
				//BACK
				mCurrentMenu = 3;
			}
		}
		return;
	}

	//SOUND MENU
	if (mCurrentMenu == 6)
	{
		if (loc.getX() > 250 && loc.getX() < 290)
		{
			if (loc.getY() > y1 && loc.getY() < y2)
			{
				//ON
				mIsSoundOn = true;
				GameEvent gameEvent(START_SOUND_EVENT);
				pEventSystem->fireEvent(gameEvent);
			}

			if (loc.getY() > y1 + spacing && loc.getY() < y2 + spacing)
			{
				//OFF
				mIsSoundOn = false;
				GameEvent gameEvent(STOP_SOUND_EVENT);
				pEventSystem->fireEvent(gameEvent);
			}
			if (loc.getY() > y1 + (spacing * 2) && loc.getY() < y2 + (spacing * 2))
			{
				//BACK
				mCurrentMenu = 3;
			}
		}
		return;
	}
}

void MenuManager::loadData(string& filename)
{
	mTextMap.clear();
	ifstream data(filename);
	string currentString;
	int amount;

	//MAIN MENU
	data >> amount;
	for (int i = 1; i < amount + 1; i++)
	{
		string menuKey = "Menu0_" + to_string(i);
		data >> currentString;
		mTextMap[menuKey] = currentString;
	}

	//PAUSE MENU
	data >> amount;
	for (int i = 1; i < amount + 1; i++)
	{
		string menuKey = "Menu1_" + to_string(i);
		getline(data, currentString, ',');
		mTextMap[menuKey] = currentString;
	}

	//GAME OVER MENU
	data >> amount;
	for (int i = 1; i < amount + 1; i++)
	{
		string menuKey = "Menu2_" + to_string(i);
		getline(data, currentString, ',');
		mTextMap[menuKey] = currentString;
	}

	//OPTIONS MENU
	data >> amount;
	for (int i = 1; i < amount + 1; i++)
	{
		string menuKey = "Menu3_" + to_string(i);
		getline(data, currentString, ',');
		mTextMap[menuKey] = currentString;
	}

	//DIFFICULTY MENU
	data >> amount;
	for (int i = 1; i < amount + 1; i++)
	{
		string menuKey = "Menu4_" + to_string(i);
		data >> currentString;
		mTextMap[menuKey] = currentString;
	}

	//GRID SIZE MENU
	data >> amount;
	for (int i = 1; i < amount + 1; i++)
	{
		string menuKey = "Menu5_" + to_string(i);
		getline(data, currentString, ',');
		mTextMap[menuKey] = currentString;
	}

	//SOUND MENU
	data >> amount;
	for (int i = 1; i < amount + 1; i++)
	{
		string menuKey = "Menu6_" + to_string(i);
		data >> currentString;
		mTextMap[menuKey] = currentString;
	}

	//WIN SCREEN
	data >> amount;
	for (int i = 1; i < amount + 1; i++)
	{
		string menuKey = "Menu7_" + to_string(i);
		getline(data, currentString, ',');
		mTextMap[menuKey] = currentString;
	}
}