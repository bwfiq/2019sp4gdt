#include "SceneSP.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "EasingStyle.h"

#include "SceneData.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "ProjectileManager.h"

#include "UIManager.h"
#include "UIAltarPopup.h"
#include "UICoreInfo.h"
#include "UIGameButton.h"
#include "UIGameText.h"
#include "UIMenuButton.h"
#include "UIOverlay.h"
#include "UIReligionBar.h"
#include "UIResearchButton.h"

#include "UITween.h"

#include "EffectManager.h"
#include "EffectTrail.h"
#include "EffectHand.h"
#include "EffectReticle.h"
#include "EffectGridWarning.h"
#include "EffectCloud.h"
#include "EffectDirt.h"
#include "EffectRing.h"

#include "CalamityManager.h"
#include "CalamityEarthquake.h"
#include "CalamityTsunami.h"
#include "CalamityTornado.h"
#include "CalamityBlizzard.h"
#include "CalamityMeteorShower.h"

#include "SMManager.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "SoundEngine.h"

#include "Villager.h"
#include "Pig.h"
#include "Granary.h"
#include "WoodShed.h"
#include "StoneShed.h"
#include "House.h"
#include "Building.h"
#include "ChiefHut.h"
#include "ResearchLab.h"
#include "Bush.h"
#include "Tree.h"
#include "Altar.h"
#include "Mountain.h"
#include "Logs.h"
#include "Tsunami.h"
#include "Tornado.h"
#include "Meteor.h"

#include "AnimationJump.h"
#include "AnimationWalk.h"
#include "AnimationPanic.h"

#define SEA_WIDTH	100.f
#define SEA_HEIGHT	100.f
SceneSP::SceneSP()
{
}

SceneSP::~SceneSP()
{
}

float FoodToReligionPoints(int food)
{
	//Insert Conversion Rates
	return (float)(food) * 1;//* some dt related converter or someth
}

void SceneSP::ChangeState(GAME_STATE newstate)
{
	if (game_state == G_INPLAY)
		tempCamera = camera; // save gamecam pos
	CSoundEngine::GetInstance()->GetSoundEngine()->setSoundVolume(1.f);
	for (auto UI : m_coreUi)
		UI->bIsDone = true;
	m_coreUi.clear();
	/*for (auto UI : m_selectedUi)
		UI->bIsDone = true;
	m_selectedUi.clear();*/
	UIBase*	newUI;
	float worldRadius = SceneData::GetInstance()->GetNoGrid() * SceneData::GetInstance()->GetGridSize() * 0.5f;
	Application::GetInstance().SetMouseVisiblity(true);
	switch (newstate)
	{
	case G_SPLASHSCREEN:
	{
		camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));	// splashscreen

	}
	break;
	case G_MAINMENU:
	{
		camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));	// splashscreen
		SceneData::GetInstance()->SetMainMenuElapsedTime(0);
		fMainMenuDelta = m_worldWidth * 0.5f;

		newUI = new UIMenuButton("", 0.775f, 0.6f);
		newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("start");
		UIManager::GetInstance()->AddUI("startbutton", newUI);
		m_coreUi.push_back(newUI);
		newUI = new UIMenuButton("", 0.775f, 0.45f);
		newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("optionsbutton");
		UIManager::GetInstance()->AddUI("optionsbutton", newUI);
		m_coreUi.push_back(newUI);
		newUI = new UIMenuButton("", 0.775f, 0.3f);
		newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("quitbutton");
		UIManager::GetInstance()->AddUI("quitbutton", newUI);
		m_coreUi.push_back(newUI);

		int increment = 0;
		for (auto theUI : m_coreUi) // It'll be for looping through the coreUIs to "easily" tween them
		{
			Vector3 origPos = theUI->pos;
			theUI->pos.Set(2, origPos.y, origPos.z);
			UITween* newTween = new UITween(
				theUI
				, 1.f
				, UITween::ES_BACK
				, UITween::ED_OUT
				, 0
				, 1 + 0.25f * increment
			);
			newTween->properties_goal["pos"] = origPos;
			theUI->AddTween(newTween);
			increment++;
		}
	}
	break;
	case G_OPTIONS:
	{
		camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));	// ortho
		newUI = new UIMenuButton("", 0.5f, 0.5f);
		newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("backbutton");
		UIManager::GetInstance()->AddUI("backbutton", newUI);
		m_coreUi.push_back(newUI);
		newUI = new UIMenuButton("", 0.65f, 0.65f);
		newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("resetbutton");
		UIManager::GetInstance()->AddUI("resetbutton", newUI);
		m_coreUi.push_back(newUI);
		newUI = new UIMenuButton("", 0.35f, 0.65f);
		if(SceneData::GetInstance()->AABBRAY)
			newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("objselectionaabb");
		else
			newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("objselectiongrid");
		UIManager::GetInstance()->AddUI("objselectionbutton", newUI);
		m_coreUi.push_back(newUI);
	}
	break;
	case G_INPLAY:
	{
		if (reticle->selected && reticle->selected != NULL)
			selected = reticle->selected;

		if (isnan(reticle->pos.x))
			reticle->pos.SetZero();

		//camera.Init(Vector3(0, 2, 2), Vector3(0, 0, 0), Vector3(0, 1, 0));	// game
		camera = tempCamera;
		camera.SetCamBounds(Vector3(worldRadius + 5, 0, worldRadius + 5));
		Application::GetInstance().SetMouseVisiblity(false);

		newUI = new UIMenuButton("", 0.1f, 0.1f);
		newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("optionsbutton");
		UIManager::GetInstance()->AddUI("optionsbutton", newUI);
		m_coreUi.push_back(newUI);
		case G_RESEARCHTREE: // will not init camera for overlays but will add ui for all ingame states
		case G_INGAMEOPTIONS:
		{
			CSoundEngine::GetInstance()->PlayASound("sea", true);

			newUI = new UIReligionBar();
			UIManager::GetInstance()->AddUI("uiReligionBar", newUI);
			m_coreUi.push_back(newUI);

			newUI = new UICoreInfo(UICoreInfo::INFO_FOOD, Vector3(0.1f, 0.9));
			UIManager::GetInstance()->AddUI("ui_Info_Food", newUI);
			m_coreUi.push_back(newUI);

			newUI = new UICoreInfo(UICoreInfo::INFO_POPULATION, Vector3(0.3f, 0.9));
			UIManager::GetInstance()->AddUI("ui_Info_Population", newUI);
			m_coreUi.push_back(newUI);

			newUI = new UICoreInfo(UICoreInfo::INFO_STONE, Vector3(0.7f, 0.9));
			UIManager::GetInstance()->AddUI("ui_Info_Stone", newUI);
			m_coreUi.push_back(newUI);

			newUI = new UICoreInfo(UICoreInfo::INFO_WOOD, Vector3(0.9f, 0.9));
			UIManager::GetInstance()->AddUI("ui_Info_Wood", newUI);
			m_coreUi.push_back(newUI);

			newUI = new UICoreInfo(UICoreInfo::INFO_DAY, Vector3(0.5f, 0.95f));
			UIManager::GetInstance()->AddUI("ui_Info_Day", newUI);
			m_coreUi.push_back(newUI);

			newUI = new UICoreInfo(UICoreInfo::INFO_TIME, Vector3(0.5f, 0.85f));
			UIManager::GetInstance()->AddUI("ui_Info_Time", newUI);
			m_coreUi.push_back(newUI);

			newUI = new UIGameButton(UIGameButton::BUTTON_DAILYREQUIREMENT);
			UIManager::GetInstance()->AddUI("ui_Button_DailyRequirement", newUI);
			m_coreUi.push_back(newUI);

			newUI = new UIGameText(UIGameText::TEXT_DAILYREQUIREMENT);
			//newUI->bActive = false;
			UIManager::GetInstance()->AddUI("ui_Text_DailyRequirement", newUI);
			m_coreUi.push_back(newUI);

			//UpdateSelectedUI();

			if (newstate == G_RESEARCHTREE)
			{
				CSoundEngine::GetInstance()->GetSoundEngine()->setSoundVolume(0.5f);
				newUI = new UIOverlay("", 0.5f, 0.45f);
				UIManager::GetInstance()->AddUI("overlay", newUI);
				m_coreUi.push_back(newUI);
				/*newUI = new UIMenuButton("back", 0.1f, 0.8f);
				UIManager::GetInstance()->AddUI("backButton", newUI);
				//newUI->pos.Set(newUI->pos.x, newUI->pos.y, 5);
				m_coreUi.push_back(newUI);*/

				//first column
				newUI = new UIResearchButton("", 0.3f, 0.45f);
				newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("woodResearch");
				if (SceneData::GetInstance()->bWoodResearch)
					newUI->uiComponents_list[UIResearchButton::COMPONENT_TICK].alpha = 1.f;
				UIManager::GetInstance()->AddUI("WoodResearch", newUI);
				m_coreUi.push_back(newUI);
				newUI = new UIGameButton(UIGameButton::BUTTON_RESEARCH_COST, 0.3f, 0.25f);
				if (SceneData::GetInstance()->bWoodResearch)
					newUI->uiComponents_list[UIGameButton::COMPONENT_TEXT].text = "UNLOCKED";
				else
					newUI->uiComponents_list[UIGameButton::COMPONENT_TEXT].text = "10 WOOD";
				UIManager::GetInstance()->AddUI("WoodResearchCost", newUI);
				m_coreUi.push_back(newUI);

				newUI = new UIResearchButton("", 0.5f, 0.45f);
				newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("stoneResearch");
				if (SceneData::GetInstance()->bStoneResearch)
					newUI->uiComponents_list[UIResearchButton::COMPONENT_TICK].alpha = 1.f;
				UIManager::GetInstance()->AddUI("StoneResearch", newUI);
				m_coreUi.push_back(newUI);
				newUI = new UIGameButton(UIGameButton::BUTTON_RESEARCH_COST, 0.5f, 0.25f);
				if (SceneData::GetInstance()->bStoneResearch)
					newUI->uiComponents_list[UIGameButton::COMPONENT_TEXT].text = "UNLOCKED";
				else
					newUI->uiComponents_list[UIGameButton::COMPONENT_TEXT].text = "10 W, 10 S";
				UIManager::GetInstance()->AddUI("StoneResearchCost", newUI);
				m_coreUi.push_back(newUI);

				newUI = new UIResearchButton("", 0.7f, 0.45f);
				newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("fullStoneResearch");
				if (SceneData::GetInstance()->bFullStoneResearch)
					newUI->uiComponents_list[UIResearchButton::COMPONENT_TICK].alpha = 1.f;
				UIManager::GetInstance()->AddUI("FullStoneResearch", newUI);
				m_coreUi.push_back(newUI);
				newUI = new UIGameButton(UIGameButton::BUTTON_RESEARCH_COST, 0.7f, 0.25f);
				if (SceneData::GetInstance()->bFullStoneResearch)
					newUI->uiComponents_list[UIGameButton::COMPONENT_TEXT].text = "UNLOCKED";
				else
					newUI->uiComponents_list[UIGameButton::COMPONENT_TEXT].text = "20 STONE";
				UIManager::GetInstance()->AddUI("FullStoneResearchCost", newUI);
				m_coreUi.push_back(newUI);

				newUI = new UIGameButton(UIGameButton::BUTTON_RESEARCH_COST, 0.5f, 0.65f);
				newUI->scale.Set(300, 75);
				newUI->uiComponents_list[UIGameButton::COMPONENT_TEXT].text = "Research Tree";
				newUI->uiComponents_list[UIGameButton::COMPONENT_TEXT].pos.Set(0.15f, 0.5f);
				newUI->uiComponents_list[UIGameButton::COMPONENT_TEXT].textSize = newUI->scale.y * 0.4f;
				UIManager::GetInstance()->AddUI("ResearchHeader", newUI);
				m_coreUi.push_back(newUI);

			}
			else if (newstate == G_INGAMEOPTIONS)
			{
				CSoundEngine::GetInstance()->GetSoundEngine()->setSoundVolume(0.5f);
				newUI = new UIOverlay("", 0.5f, 0.45f);
				UIManager::GetInstance()->AddUI("overlay", newUI);
				m_coreUi.push_back(newUI);
				newUI = new UIMenuButton("", 0.5f, 0.45f);
				newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("backbutton");
				UIManager::GetInstance()->AddUI("backbutton", newUI);
				m_coreUi.push_back(newUI);
				newUI = new UIMenuButton("", 0.5f, 0.25f);
				newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("savebutton");
				UIManager::GetInstance()->AddUI("savebutton", newUI);
				m_coreUi.push_back(newUI);
				newUI = new UIMenuButton("", 0.65f, 0.65f);
				newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("resetbutton");
				UIManager::GetInstance()->AddUI("resetbutton", newUI);
				m_coreUi.push_back(newUI);
				newUI = new UIMenuButton("", 0.35f, 0.65f);
				if (SceneData::GetInstance()->AABBRAY)
					newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("objselectionaabb");
				else
					newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("objselectiongrid");
				UIManager::GetInstance()->AddUI("objselectionbutton", newUI);
				m_coreUi.push_back(newUI);
			}
		}
	}
	break;
	case G_LOSESCREEN:
	{
		camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));	// splashscreen

		newUI = new UIMenuButton("", 0.25f, 0.3f);
		newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("restartbutton");
		UIManager::GetInstance()->AddUI("restartbutton", newUI);
		m_coreUi.push_back(newUI);
		newUI = new UIMenuButton("", 0.5f, 0.3f);
		newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("loadbutton");
		UIManager::GetInstance()->AddUI("loadbutton", newUI);
		m_coreUi.push_back(newUI);
		newUI = new UIMenuButton("", 0.75f, 0.3f);
		newUI->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("quitbutton");
		UIManager::GetInstance()->AddUI("quitbutton", newUI);
		m_coreUi.push_back(newUI);
	}
	break;
	default:
	{

	}
	break;
	}

	Vector3 dir = camera.target - camera.position;
	dir.Normalize();
	Vector3 right(1, 0, 0);
	camera.up = right.Cross(dir);
	camera.fDistance = (camera.target - camera.position).Length();

	game_state = newstate;
}

//bool SceneSP::isTheCoastClear(GameObject* go, GridPt next, Grid::DIRECTION dir)
//{
//	if (go->iGridX > 0 && go->iGridZ > 0)
//	{
//		int iHalfGridX = go->iGridX * 0.5f;
//		int iHalfGridZ = go->iGridZ * 0.5f;
//		GridPt ptAhead;
//		switch (dir)
//		{
//		case Grid::DIR_LEFT:
//			for (int zAxis = 1; zAxis <= iHalfGridZ; ++zAxis)
//			{
//				ptAhead.Set(next.x - iHalfGridX, next.z + zAxis);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			for (int zAxis = 0; zAxis < (go->iGridZ - iHalfGridZ); ++zAxis)
//			{
//				ptAhead.Set(next.x - iHalfGridX, next.z - zAxis);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			break;
//		case Grid::DIR_RIGHT:
//			for (int zAxis = 1; zAxis <= iHalfGridZ; ++zAxis)
//			{
//				ptAhead.Set(next.x + iHalfGridX, next.z + zAxis);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			for (int zAxis = 0; zAxis < (go->iGridZ - iHalfGridZ); ++zAxis)
//			{
//				ptAhead.Set(next.x + iHalfGridX, next.z - zAxis);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			break;
//		case Grid::DIR_UP:
//			for (int xAxis = 1; xAxis <= iHalfGridX; ++xAxis)
//			{
//				ptAhead.Set(next.x + xAxis, next.z - iHalfGridZ);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//
//			for (int xAxis = 0; xAxis < (go->iGridX - iHalfGridX); ++xAxis)
//			{
//				ptAhead.Set(next.x - xAxis, next.z - iHalfGridZ);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			break;
//		case Grid::DIR_DOWN:
//			for (int xAxis = 1; xAxis <= iHalfGridX; ++xAxis)
//			{
//				ptAhead.Set(next.x + xAxis, next.z + iHalfGridZ);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			for (int xAxis = 0; xAxis < (go->iGridX - iHalfGridX); ++xAxis)
//			{
//				ptAhead.Set(next.x - xAxis, next.z + iHalfGridZ);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			break;
//		case Grid::DIR_LEFTUP:
//			//Checking Left
//			for (int zAxis = 1; zAxis <= iHalfGridZ; ++zAxis)
//			{
//				ptAhead.Set(next.x - iHalfGridX, next.z + zAxis);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			for (int zAxis = 0; zAxis < (go->iGridZ - iHalfGridZ); ++zAxis)
//			{
//				ptAhead.Set(next.x - iHalfGridX, next.z - zAxis);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			//Checking Up
//			for (int xAxis = 1; xAxis <= iHalfGridX; ++xAxis)
//			{
//				ptAhead.Set(next.x + xAxis, next.z - iHalfGridZ);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			for (int xAxis = 0; xAxis < (go->iGridX - iHalfGridX); ++xAxis)
//			{
//				ptAhead.Set(next.x - xAxis, next.z - iHalfGridZ);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			break;
//		case Grid::DIR_RIGHTUP:
//			//Checking Right
//			for (int zAxis = 1; zAxis <= iHalfGridZ; ++zAxis)
//			{
//				ptAhead.Set(next.x + iHalfGridX, next.z + zAxis);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			for (int zAxis = 0; zAxis < (go->iGridZ - iHalfGridZ); ++zAxis)
//			{
//				ptAhead.Set(next.x + iHalfGridX, next.z - zAxis);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			//Checking Up
//			for (int xAxis = 1; xAxis <= iHalfGridX; ++xAxis)
//			{
//				ptAhead.Set(next.x + xAxis, next.z - iHalfGridZ);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			for (int xAxis = 0; xAxis < (go->iGridX - iHalfGridX); ++xAxis)
//			{
//				ptAhead.Set(next.x - xAxis, next.z - iHalfGridZ);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			break;
//		case Grid::DIR_LEFTDOWN:
//			//Checking Left
//			for (int zAxis = 1; zAxis <= iHalfGridZ; ++zAxis)
//			{
//				ptAhead.Set(next.x - iHalfGridX, next.z + zAxis);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			for (int zAxis = 0; zAxis < (go->iGridZ - iHalfGridZ); ++zAxis)
//			{
//				ptAhead.Set(next.x - iHalfGridX, next.z - zAxis);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			//Checking Down
//			for (int xAxis = 1; xAxis <= iHalfGridX; ++xAxis)
//			{
//				ptAhead.Set(next.x + xAxis, next.z + iHalfGridZ);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			for (int xAxis = 0; xAxis < (go->iGridX - iHalfGridX); ++xAxis)
//			{
//				ptAhead.Set(next.x - xAxis, next.z + iHalfGridZ);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			break;
//		case Grid::DIR_RIGHTDOWN:
//			//Checking Right
//			for (int zAxis = 1; zAxis <= iHalfGridZ; ++zAxis)
//			{
//				ptAhead.Set(next.x + iHalfGridX, next.z + zAxis);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			for (int zAxis = 0; zAxis < (go->iGridZ - iHalfGridZ); ++zAxis)
//			{
//				ptAhead.Set(next.x + iHalfGridX, next.z - zAxis);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			//Checking Down
//			for (int xAxis = 1; xAxis <= iHalfGridX; ++xAxis)
//			{
//				ptAhead.Set(next.x + xAxis, next.z + iHalfGridZ);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			for (int xAxis = 0; xAxis < (go->iGridX - iHalfGridX); ++xAxis)
//			{
//				ptAhead.Set(next.x - xAxis, next.z + iHalfGridZ);
//				if (isPointInGrid(ptAhead))
//				{
//					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
//					{
//						return false;
//					}
//				}
//			}
//			break;
//		default:
//			break;
//		}
//	}
//	return true;
//}

void SceneSP::Init()
{
	fYPos = 0.12f;
	SceneData::GetInstance()->SetNoGrid(15);
	SceneData::GetInstance()->SetGridSize(1.f);
	SceneData::GetInstance()->SetGridOffset(0.5f);
	SceneBase::Init();

	m_grid.resize(SceneData::GetInstance()->GetNoGrid() * SceneData::GetInstance()->GetNoGrid());
	std::fill(m_grid.begin(), m_grid.end(), Grid::TILE_EMPTY);
	bShowGrid = false;
	bGodlights = true;
	Application::GetInstance().SetMouseVisiblity(true);
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	SceneData::GetInstance()->SetWorldHeight(m_worldHeight);
	SceneData::GetInstance()->SetWorldWidth(m_worldWidth);
	SceneData::GetInstance()->SetElapsedTime(0);
	SceneData::GetInstance()->SetReligionValue(0.f);
	SceneData::GetInstance()->SetMaxReligionValue(100.f);
	SceneData::GetInstance()->SetGOList(&m_goList);
	PostOffice::GetInstance()->Register("Scene", this);

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	selected = NULL;
	hovered = NULL;
	goChiefHut = NULL;
	goAltar = NULL;
	goResearchLab = NULL;
	tempCamera.Init(Vector3(0, 2, 2), Vector3(0, 0, 0), Vector3(0, 1, 0));

	iCurrItrVillagers = 0;

	bWorldEnd = false;
	fWaterLevel = 0.7f;

	/*
	//Objects from maya, bottom of object to be translated down
	goVillager = FetchGO(GameObject::GO_VILLAGER);
	goVillager->scale.y = 1.f;
	goVillager->pos = GetGridPos(GridPt(5, 5));
	goVillager->pos.y = goVillager->scale.y * 0.5f;
	goVillager->GiveAnimation(new AnimationJump());

	goPig = FetchGO(GameObject::GO_PIG);
	goPig->scale.y = 1.f;
	goPig->pos = GetGridPos(GridPt(3, 5));
	goPig->pos.y = goVillager->scale.y * 0.5f;
	goPig->GiveAnimation(new AnimationJump());

	goChiefHut = FetchGO(GameObject::GO_CHIEFHUT);
	goChiefHut->pos = GetGridPos(GridPt(8, 7));
	goChiefHut->pos.y = goChiefHut->scale.y * 0.5f;
	static_cast<Building*>(goChiefHut)->bBuilt = true;

	goHouse = FetchGO(GameObject::GO_HOUSE);
	goHouse->pos = GetGridPos(GridPt(5, 7));
	goHouse->pos.y = goHouse->scale.y * 0.5f;
	static_cast<Building*>(goHouse)->bBuilt = true;

	goGranary = FetchGO(GameObject::GO_GRANARY);
	goGranary->pos = GetGridPos(GridPt(3, 5));
	goGranary->pos.y = goGranary->scale.y * 0.5f;
	static_cast<Building*>(goGranary)->bBuilt = true;

	goWoodshed = FetchGO(GameObject::GO_WOODSHED);
	goWoodshed->pos = GetGridPos(GridPt(7, 13));
	goWoodshed->pos.y = goWoodshed->scale.y * 0.5f;
	static_cast<Building*>(goWoodshed)->bBuilt = true;

	goLogs = FetchGO(GameObject::GO_LOGS);
	goLogs->pos = GetGridPos(GridPt(10, 7));
	goLogs->pos.y = goLogs->scale.y * 0.5f;
	static_cast<Building*>(goLogs)->bBuilt = true;

	goResearchLab = FetchGO(GameObject::GO_RESEARCHLAB);
	goResearchLab->pos = GetGridPos(GridPt(8, 2));
	goResearchLab->pos.y = goResearchLab->scale.y * 0.5f;
	static_cast<Building*>(goResearchLab)->bBuilt = true;

	goAltar = FetchGO(GameObject::GO_ALTAR);
	goAltar->pos = GetGridPos(GridPt(6, 2));
	goAltar->pos.y = goAltar->scale.y * 0.5f;
	Altar* altar = static_cast<Altar*>(goAltar);
	altar->bBuilt = true;
	altar->iFoodOffered = 100;

	goBush = FetchGO(GameObject::GO_BUSH);
	goBush->pos = GetGridPos(GridPt(1, 1));
	goBush->pos.y = goBush->scale.y * 0.5f;
	Bush* bGo = static_cast<Bush*>(goBush);
	bGo->eCurrState = Bush::LUSH;
	bGo->fTimer = 5;
	bGo->iFoodAmount = 10;

	goTree = FetchGO(GameObject::GO_TREE);
	goTree->pos = GetGridPos(GridPt(2, 8));
	goTree->pos.y = goTree->scale.y * 0.5f;
	goTree->iGridX = 1;
	goTree->iGridZ = 1;
	Tree* tGo = static_cast<Tree*>(goTree);
	tGo->eCurrState = Tree::FULL;
	tGo->fTimer = 5;
	tGo->iWoodAmount = 10;

	goTree2 = FetchGO(GameObject::GO_TREE);
	goTree2->pos = GetGridPos(GridPt(2, 9));
	goTree2->pos.y = goTree2->scale.y * 0.5f;
	goTree2->iGridX = 1;
	goTree2->iGridZ = 1;
	tGo = static_cast<Tree*>(goTree2);
	tGo->eCurrState = Tree::FULL;
	tGo->fTimer = 2;
	tGo->iWoodAmount = 10;

	goMountain = FetchGO(GameObject::GO_MOUNTAIN);
	goMountain->pos = GetGridPos(GridPt(3, 8));
	goMountain->pos.y = goMountain->scale.y * 0.5f;
	goMountain->iGridX = 1;
	goMountain->iGridZ = 1;
	Mountain* mGo = static_cast<Mountain*>(goMountain);
	mGo->iStoneAmount = 11;
	mGo->iStoneGain = 5;
	mGo->fTimer = 4;
	*/

	SceneData* SD = SceneData::GetInstance();
	SD->SetFood(0);
	SD->SetFoodLimit(0);
	SD->SetPopulation(0);
	SD->SetPopulationLimit(0);
	SD->SetWood(0);
	SD->SetWoodLimit(0);
	SD->SetStone(0);
	SD->SetStoneLimit(0);

	SD->SetCurrMonth(1);
	SD->SetCurrDay(1);
	SD->SetTimeOfDay(8);

	bDay = true; // day
	fTimeOfDay = SD->GetTimeOfDay();
	bGoalAchieved = false;

	//research
	SceneData::GetInstance()->bWoodResearch = false;
	SceneData::GetInstance()->bStoneResearch = false;
	SceneData::GetInstance()->bFullStoneResearch = false;

	//go->vel.Set(1, 0, 0);
	MousePicker::GetInstance()->Init();
	MousePicker::GetInstance()->SetProjectionStack(projectionStack);
	MousePicker::GetInstance()->SetViewStack(viewStack);

	UIManager::GetInstance()->Init();

	reticle = new EffectReticle(EffectReticle::RETICLE_OUTER);
	hand = new EffectHand(&camera);
	EffectReticle* reticle_cross = new EffectReticle(EffectReticle::RETICLE_INNER);
	reticle_cross->mesh = meshList[GEO_RETICLE_CROSS];

	EffectManager::GetInstance()->Init();
	EffectManager::GetInstance()->AddEffect(new EffectTrail(hand));
	EffectManager::GetInstance()->AddEffect(reticle);
	EffectManager::GetInstance()->AddEffect(reticle_cross);
	EffectManager::GetInstance()->AddEffect(hand);
	EffectManager::GetInstance()->SetCamera(&camera);

	CalamityManager::GetInstance()->Init();

	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("bg",			"Audio//bgmusic.mp3");
	CSoundEngine::GetInstance()->AddSound("building",	"Audio//building.wav");
	CSoundEngine::GetInstance()->AddSound("chopping",	"Audio//chopping.wav");
	CSoundEngine::GetInstance()->AddSound("death",		"Audio//death.wav");
	CSoundEngine::GetInstance()->AddSound("earthquake",	"Audio//earthquake.wav");
	CSoundEngine::GetInstance()->AddSound("gasp",		"Audio//gasp.wav");
	CSoundEngine::GetInstance()->AddSound("grunt",		"Audio//grunt.wav");
	CSoundEngine::GetInstance()->AddSound("hunting",	"Audio//hunting.wav");
	CSoundEngine::GetInstance()->AddSound("jump",		"Audio//jump.wav");
	CSoundEngine::GetInstance()->AddSound("mining",		"Audio//mining.wav");
	CSoundEngine::GetInstance()->AddSound("munching",	"Audio//munching.wav");
	CSoundEngine::GetInstance()->AddSound("oink1",		"Audio//oink1.wav");
	CSoundEngine::GetInstance()->AddSound("oink2",		"Audio//oink2.wav");
	CSoundEngine::GetInstance()->AddSound("place",		"Audio//place.wav");
	CSoundEngine::GetInstance()->AddSound("rumble",		"Audio//rumble.wav");
	CSoundEngine::GetInstance()->AddSound("rustling",	"Audio//rustling.wav");
	CSoundEngine::GetInstance()->AddSound("sea",		"Audio//sea.wav");
	CSoundEngine::GetInstance()->AddSound("selection",	"Audio//selection.wav");
	CSoundEngine::GetInstance()->AddSound("step",		"Audio//step.wav");
	CSoundEngine::GetInstance()->AddSound("waves",		"Audio//waves.wav");

	game_state = G_INPLAY; // to save the camera pos
	ChangeState(G_SPLASHSCREEN);
}

bool SceneSP::Handle(Message* message)
{
	SceneData* SD = SceneData::GetInstance();
	MessageWRU* messageWRU = dynamic_cast<MessageWRU*>(message);
	if (messageWRU)
	{
		switch (messageWRU->type)
		{
		case MessageWRU::FIND_CHIEFHUT:
			messageWRU->go->goTarget = goChiefHut;
			break;
		case MessageWRU::FIND_NEAREST_GRANARY:
		{
			GridPt currGrid = messageWRU->go->currentPt;
			int iDistance = INT_MAX;
			GameObject* currTarget = NULL;
			for (auto go : m_goList)
			{
				if (!go->active || go->type != GameObject::GO_GRANARY)
					continue;
				GridPt goGrid = go->currentPt;
				int iTotalDist = (goGrid.x - currGrid.x) * (goGrid.x - currGrid.x) + (goGrid.z - currGrid.z) * (goGrid.z - currGrid.z);
				if (iTotalDist < iDistance)
				{
					iDistance = iTotalDist;
					currTarget = go;
				}
			}
			if (currTarget != NULL)
			{
				messageWRU->go->goTarget = currTarget;
			}
			else
			{
				messageWRU->go->goTarget = goChiefHut;
			}
		}
			break;
		case MessageWRU::FIND_NEAREST_WOODSHED:
		{
			GridPt currGrid = messageWRU->go->currentPt;
			int iDistance = INT_MAX;
			GameObject* currTarget = NULL;
			for (auto go : m_goList)
			{
				if (!go->active || go->type != GameObject::GO_WOODSHED)
					continue;
				GridPt goGrid = go->currentPt;
				int iTotalDist = (goGrid.x - currGrid.x) * (goGrid.x - currGrid.x) + (goGrid.z - currGrid.z) * (goGrid.z - currGrid.z);
				if (iTotalDist < iDistance)
				{
					iDistance = iTotalDist;
					currTarget = go;
				}
			}
			if (currTarget != NULL)
			{
				messageWRU->go->goTarget = currTarget;
			}
			else
			{
				messageWRU->go->goTarget = goChiefHut;
			}
		}
		break;
		case MessageWRU::FIND_NEAREST_STONESHED:
		{
			GridPt currGrid = messageWRU->go->currentPt;
			int iDistance = INT_MAX;
			GameObject* currTarget = NULL;
			for (auto go : m_goList)
			{
				if (!go->active || go->type != GameObject::GO_STONESHED)
					continue;
				GridPt goGrid = go->currentPt;
				int iTotalDist = (goGrid.x - currGrid.x) * (goGrid.x - currGrid.x) + (goGrid.z - currGrid.z) * (goGrid.z - currGrid.z);
				if (iTotalDist < iDistance)
				{
					iDistance = iTotalDist;
					currTarget = go;
				}
			}
			if (currTarget != NULL)
			{
				messageWRU->go->goTarget = currTarget;
			}
			else
			{
				messageWRU->go->goTarget = goChiefHut;
			}
		}
		break;
		case MessageWRU::FIND_NEAREST_LUSH_BUSH:
		{
			GridPt currGrid = messageWRU->go->currentPt;
			int iDistance = INT_MAX;
			GameObject* currTarget = NULL;
			for (auto go : m_goList)
			{
				if (!go->active || go->type != GameObject::GO_BUSH || static_cast<Bush*>(go)->eCurrState != Bush::LUSH)
					continue;
				GridPt goGrid = go->currentPt;
				int iTotalDist = (goGrid.x - currGrid.x) * (goGrid.x - currGrid.x) + (goGrid.z - currGrid.z) * (goGrid.z - currGrid.z);
				if (iTotalDist < iDistance)
				{
					iDistance = iTotalDist;
					currTarget = go;
				}
			}
			if (currTarget != NULL)
			{
				messageWRU->go->goTarget = currTarget;
			}
			else
			{
				messageWRU->go->goTarget = goChiefHut;
			}
		}
		break;
		case MessageWRU::PATH_TO_TARGET:
			AStarSingleGrid(messageWRU->go, GetPoint(messageWRU->go->goTarget->pos));
			break;
		case MessageWRU::PATH_TO_POINT:
			AStarSingleGrid(messageWRU->go, GetPoint(messageWRU->go->target));
			break;
		case MessageWRU::RANDOM_TARGET:
		{
			float fLengthAway = 0;
			int iRandIndex;
			GridPt currGrid = GetPoint(messageWRU->go->pos);
			while (fLengthAway < messageWRU->threshold)
			{
				iRandIndex = Math::RandIntMinMax(0, SceneData::GetInstance()->GetNoGrid() * SceneData::GetInstance()->GetNoGrid() - 1);
				if (m_grid[iRandIndex] != Grid::TILE_USED)
				{
					std::pair<int, int> randPoint = GetPoint(iRandIndex);
					fLengthAway = (randPoint.first - currGrid.x) * (randPoint.first - currGrid.x) + (randPoint.second - currGrid.z) * (randPoint.second - currGrid.z);
				}
			}
			std::pair<int, int> randPoint = GetPoint(iRandIndex);
			messageWRU->go->target = GetGridPos(GridPt(randPoint.first,randPoint.second));
		}
			break;
		default:
			break;
		}
		delete message;
		return true;
	}
	MessageCameraShake* messageCamShake = dynamic_cast<MessageCameraShake*>(message);
	if (messageCamShake)
	{
		camera.SetCamShake(messageCamShake->type + 1, messageCamShake->intensity, messageCamShake->duration);
		delete message;
		return true;
	}
	MessageWarnLane* messageWarnLane = dynamic_cast<MessageWarnLane*>(message);
	if (messageWarnLane)
	{
		for (auto laneNum : messageWarnLane->lanes)
		{

			EffectGridWarning* warningEffect = new EffectGridWarning(
				GetGridPos(GridPt(SD->GetNoGrid() * 0.5f, laneNum)) + Vector3(0,0.002 + laneNum*0.01f)
				, Vector3(SD->GetGridSize() * SD->GetNoGrid(), SD->GetGridSize(), 1)
			);
			EffectManager::GetInstance()->AddEffect(warningEffect);
		}
		delete message;
		return true;
	}
	MessageWarnGrids* messageWarnGrids = dynamic_cast<MessageWarnGrids*>(message);
	if (messageWarnGrids)
	{
		Vector3 gridSize(SD->GetGridSize(), SD->GetGridSize(), 1.f);
		for (auto grid : messageWarnGrids->grids)
		{
			EffectGridWarning* warningEffect = new EffectGridWarning(
				GetGridPos(grid)
				, gridSize
			);
			EffectManager::GetInstance()->AddEffect(warningEffect);
		}
		delete message;
		return true;
	}
	MessageCalamityEnd* messageCalamityEnd = dynamic_cast<MessageCalamityEnd*>(message);
	if (messageCalamityEnd)
	{
		for (auto go : m_goList)
		{
			if (!go->active || go->type != GameObject::GO_VILLAGER)
				continue;
			Villager* goVil = static_cast<Villager*>(go);
			//make villagers calm down
			goVil->eCurrState = (Villager::STATES)(Math::RandIntMinMax((int)Villager::TIRED, (int)Villager::DYING));

			if (goVil->animation != NULL)
			{
				if (goVil->animation->type == AnimationBase::A_PANIC)
				{
					goVil->ClearAnimation();
				}
			}
		}
	}
	MessageCalamityTsunami* messageCalamityTsunami = dynamic_cast<MessageCalamityTsunami*>(message);
	if (messageCalamityTsunami)
	{
		for (auto laneNum : messageCalamityTsunami->lanes)
		{
			GameObject* tsun = FetchGO(GameObject::GO_TSUNAMI);
			Tsunami* tsunami = static_cast<Tsunami*>(tsun);
			tsunami->fPower = 100.f;
			tsunami->collidedObjects.clear(); 
			tsunami->tsunami_direction = Tsunami::DIRECTION_LEFT;
			tsunami->moveSpeed = Math::RandFloatMinMax(3, 3.25f);
			tsunami->pos = GetGridPos(GridPt(SD->GetNoGrid() + 8, laneNum));
		}
		for (auto go : m_goList)
		{
			if (!go->active || go->type != GameObject::GO_VILLAGER)
				continue;
			Villager* goVil = static_cast<Villager*>(go);
			//make villagers panic
			goVil->eCurrState = Villager::PANIC;
			goVil->GiveAnimation(new AnimationPanic());
		}
		delete message;
		return true;
	}
	MessageCalamityMeteorShower* messageCalamityMeteorShower = dynamic_cast<MessageCalamityMeteorShower*>(message);
	if (messageCalamityMeteorShower)
	{
		for (auto grid : messageCalamityMeteorShower->meteorSpawnGrids)
		{
			GameObject* met = FetchGO(GameObject::GO_METEOR);
			Meteor* goMeteor = static_cast<Meteor*>(met);
			goMeteor->fPower = 100.f;
			goMeteor->collidedObjects.clear();
			goMeteor->moveSpeed = Math::RandFloatMinMax(1.5f, 2.5f);
			Vector3 gridPos = GetGridPos(grid);
			Vector3 meteorGeneratedPos = gridPos + Vector3(SD->GetGridSize() * Math::RandFloatMinMax(-2.5f, 2.5f), Math::RandIntMinMax(10, 15) , SD->GetGridSize() * Math::RandFloatMinMax(-2.5f, 2.5f));
			goMeteor->pos = meteorGeneratedPos;
			goMeteor->vel = (gridPos - meteorGeneratedPos).Normalized() * goMeteor->moveSpeed;
		}
		for (auto go : m_goList)
		{
			if (!go->active || go->type != GameObject::GO_VILLAGER)
				continue;
			Villager* goVil = static_cast<Villager*>(go);
			//make villagers panic
			goVil->eCurrState = Villager::PANIC;
			goVil->GiveAnimation(new AnimationPanic());
		}
		delete message;
		return true;
	}
	MessageCalamityWorldEnd* messageCalamityWorldEnd = dynamic_cast<MessageCalamityWorldEnd*>(message);
	if (messageCalamityWorldEnd)
	{
		bWorldEnd = true;
		for (auto go : m_goList)
		{
			if (!go->active || go->type != GameObject::GO_VILLAGER)
				continue;
			Villager* goVil = static_cast<Villager*>(go);
			//make villagers panic
			goVil->eCurrState = Villager::PANIC;
			goVil->GiveAnimation(new AnimationPanic());
		}
		delete message;
		return true;
	}
	MessageCalamityTornado* messageCalamityTornado = dynamic_cast<MessageCalamityTornado*>(message);
	if (messageCalamityTornado)
	{
		GameObject* tor = FetchGO(GameObject::GO_TORNADO);
		Tornado* tornado = static_cast<Tornado*>(tor);
		tornado->fPower = 100.f;
		tornado->collidedObjects.clear();
		tornado->moveSpeed = Math::RandFloatMinMax(2, 2.5f);
		tornado->pos = GetGridPos(GridPt(SD->GetNoGrid() + 8, 3));
		tornado->fElapsedTime = 0;
		for (auto go : m_goList)
		{
			if (!go->active || go->type != GameObject::GO_VILLAGER)
				continue;
			Villager* goVil = static_cast<Villager*>(go);
			//make villagers panic
			goVil->eCurrState = Villager::PANIC;
			goVil->GiveAnimation(new AnimationPanic());
		}
		delete message;
		return true;
	}
	MessageCalamityEarthquake* messageCalamityEarthquake = dynamic_cast<MessageCalamityEarthquake*>(message);
	if (messageCalamityEarthquake)
	{
		if (messageCalamityEarthquake->type == MessageCalamityEarthquake::INTENSE)
		{
			//Chance to destroy buildings
			for (auto go : m_goList)
			{
				if (!go->active)
					continue;
				Building* goBuilding = dynamic_cast<Building*>(go);
				if (goBuilding)
				{
					float dmgReduction = 0;
					switch (goBuilding->eCurrTier)
					{
					case Building::STRAW:
						dmgReduction += 0;
						break;
					case Building::WOOD:
						dmgReduction += 10;
						break;
					case Building::STONE:
						dmgReduction += 25;
						break;
					case Building::FULL_STONE:
						dmgReduction += 50;
						break;
					}

					if (Math::RandFloatMinMax(0.f, 100.f) + dmgReduction < messageCalamityEarthquake->fPower)
					{
						switch (goBuilding->eCurrState)
						{
						case Building::COMPLETED:
							goBuilding->eCurrState = Building::BROKEN;
							break;
						case Building::BROKEN:
							goBuilding->active = false;
							break;
						default:
							break;
						}
					}
				}
				Villager* goVil = dynamic_cast<Villager*>(go);
				if (goVil)
				{
					//make villagers panic
					goVil->eCurrState = Villager::PANIC;
					goVil->GiveAnimation(new AnimationPanic());
				}
			}
		}
		else if (messageCalamityEarthquake->type == MessageCalamityEarthquake::STOPPING)
		{
			for (auto go : m_goList)
			{
				if (!go->active)
					continue;
				Villager* goVil = dynamic_cast<Villager*>(go);
				if (goVil)
				{
					//make villagers calm down
					goVil->eCurrState = (Villager::STATES)(Math::RandIntMinMax((int)Villager::TIRED, (int) Villager::DYING));

					if (goVil->animation != NULL)
					{
						if (goVil->animation->type == AnimationBase::A_PANIC)
						{
							goVil->ClearAnimation();
						}
					}
				}
			}
		}
		delete message;
		return true;
	}
	MessageCalamityBlizzard* messageCalamityBlizzard = dynamic_cast<MessageCalamityBlizzard*>(message);
	if (messageCalamityBlizzard)
	{
		if (messageCalamityBlizzard->type == MessageCalamityBlizzard::INTENSE)
		{

		}
		else if (messageCalamityBlizzard->type == MessageCalamityBlizzard::STOPPING)
		{

		}
		delete message;
		return true;
	}
	MessageDisplayDailyRequirement* messageDisplayReq = dynamic_cast<MessageDisplayDailyRequirement*>(message);
	if (messageDisplayReq)
	{
		UIBase* dailyReqTextUI = UIManager::GetInstance()->GetUI("ui_Text_DailyRequirement");
		if (dailyReqTextUI == NULL)
		{
			delete message;
			return false;
		}
		dailyReqTextUI->bActive = !dailyReqTextUI->bActive;
		if (dailyReqTextUI->bActive)
		{
			messageDisplayReq->ui->SetText("Current Goal ^");
		}
		else
		{
			messageDisplayReq->ui->SetText("Current Goal v");
		}
		delete message;
		return true;
	}
	MessageAltarOffer* messageAltarOffer = dynamic_cast<MessageAltarOffer*>(message);
	if (messageAltarOffer)
	{
		if (messageAltarOffer->type == MessageAltarOffer::OFFER_FOOD)
		{
			if (SD->GetFood() > 0)
			{
				static_cast<Altar*>(goAltar)->iFoodOffered += 10;
				SD->SetFood(SD->GetFood() - 1);
			}
		}
		delete message;
		return true;
	}
	MessageResearch* messageResearch = dynamic_cast<MessageResearch*>(message);
	if (messageResearch)
	{
		switch (game_state)
		{
		case G_RESEARCHTREE:
			ChangeState(G_INPLAY);
			break;
		case G_INPLAY:
			ChangeState(G_RESEARCHTREE);
			break;
		}
		delete message;
		return true;
	}
	MessageMoveButton* messageMoveButton = dynamic_cast<MessageMoveButton*>(message);
	if (messageMoveButton)
	{
		if (selected != NULL)
		{
			Building* goBuilding = dynamic_cast<Building*>(selected);
			if (goBuilding)
			{
				//Only Completed and non broken buildings can be moved again
				if (goBuilding->eCurrState == Building::COMPLETED)
				{
					goBuilding->eCurrState = Building::BLUEPRINT;
					GridPt currentGrid = GetPoint(mousePos);
					selected->pos = GetGridPos(currentGrid);
					selected->pos.y += selected->scale.y * 0.5f;

					bShowGrid = true;
				}
			}
		}
		else
		{
			std::cout << "move bttn error" << std::endl;
		}
		delete message;
		return true;
	}
	MessageBuildBuildings* messageBuildBuildings = dynamic_cast<MessageBuildBuildings*>(message);
	if (messageBuildBuildings)
	{
		//selected = FetchGO(GameObject::GO_HOUSE);
		selected = FetchGO(messageBuildBuildings->type);
		GridPt currentGrid = GetPoint(mousePos);
		selected->pos = GetGridPos(currentGrid);
		selected->pos.y = selected->scale.y * 0.5f;
		static_cast<Building*>(selected)->eCurrState = Building::BLUEPRINT;

		bShowGrid = true;
		delete message;
		return true;
	}
	MessageCreateBuildUIs* messageCreateBuildUIs = dynamic_cast<MessageCreateBuildUIs*>(message);
	if (messageCreateBuildUIs)
	{
		UIBase* buildButtonUI = UIManager::GetInstance()->GetUI("uiSelected_Chiefhut_Build");
		if (!buildButtonUI->bActive)
		{
			delete message;
			return false;
		}
		std::vector<UIBase*> m_buildUIs;
		UIBase* newUI = new UIGameButton(UIGameButton::BUTTON_BUILD_LOGS, 0, 0, 0);
		UIManager::GetInstance()->AddUI("uiBuild_Logs", newUI);
		m_selectedUi.push_back(newUI);
		m_buildUIs.push_back(newUI);
		newUI = new UIGameButton(UIGameButton::BUTTON_BUILD_GRANARY, 0, 0, 1);
		UIManager::GetInstance()->AddUI("uiBuild_Granary", newUI);
		m_selectedUi.push_back(newUI);
		m_buildUIs.push_back(newUI);
		newUI = new UIGameButton(UIGameButton::BUTTON_BUILD_HOUSE, 0, 0, 2);
		UIManager::GetInstance()->AddUI("uiBuild_House", newUI);
		m_selectedUi.push_back(newUI);
		m_buildUIs.push_back(newUI);
		newUI = new UIGameButton(UIGameButton::BUTTON_BUILD_WOODSHED, 0, 0, 3);
		UIManager::GetInstance()->AddUI("uiBuild_Woodshed", newUI);
		m_selectedUi.push_back(newUI);
		m_buildUIs.push_back(newUI);
		newUI = new UIGameButton(UIGameButton::BUTTON_BUILD_STONESHED, 0, 0, 4);
		UIManager::GetInstance()->AddUI("uiBuild_StoneShed", newUI);
		m_selectedUi.push_back(newUI);
		m_buildUIs.push_back(newUI);
		int increment = 0;
		for (auto UI : m_buildUIs)
		{
			Vector3 origPos = UI->pos;
			//UI->pos.Set(UI->pos.x, 1.3f, UI->pos.z);
			UI->pos.Set(1.35f, UI->pos.y, UI->pos.z);
			UITween* newTween = new UITween(
				UI
				, 0.35f
				, UITween::ES_CUBIC
				, UITween::ED_OUT
				, 0
				, increment * 0.075f
			);
			newTween->properties_goal["pos"] = origPos;
			UI->AddTween(newTween);
			++increment;
		}
		UIManager::GetInstance()->GetUI("uiSelected_Chiefhut_Build")->bActive = false;

		delete message;
		return true;
	}
	delete message;
	return false;
}

GameObject* SceneSP::FetchGO(GameObject::GAMEOBJECT_TYPE type)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active && go->type == type)
		{
			go->active = true;
			++m_objectCount;
			switch (type)
			{
			case GameObject::GO_VILLAGER:
			{
				Villager* goVil = static_cast<Villager*>(go);
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * .7f, .5f, SceneData::GetInstance()->GetGridSize() * .7f);
				goVil->fEfficiency = 1.f;
				goVil->eCurrState = Villager::HEALTHY;
				goVil->iFoodStored = 0;
				goVil->iWoodStored = 0;
				goVil->iStoneStored = 0;
				goVil->iMaxFoodStored = 5;
				goVil->iMaxWoodStored = 5;
				goVil->iMaxStoneStored = 5;
				for (int i = 0; i < Villager::STAT_TOTAL; ++i)
				{
					goVil->fStats[i] = 1.f;
				}
				goVil->fActionTimer = 0.f;
				goVil->mEquipment = NULL;
				go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Idle");
			}
				break;
			case GameObject::GO_PIG:
			{
				Pig* goPig = static_cast<Pig*>(go);
				goPig->fEnergy = 50;
				goPig->fActionTimer = 0.f;
				goPig->state = Pig::WILD;
				goPig->movement = Pig::WALKING;
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * .7f, .5f, SceneData::GetInstance()->GetGridSize() * .7f);
				go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Idle");
			}
			break;
			case GameObject::GO_GRANARY:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * .7f, 1.f, SceneData::GetInstance()->GetGridSize() * .7f);
				break;
			case GameObject::GO_WOODSHED:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * .7f, 1.f, SceneData::GetInstance()->GetGridSize() * .7f);
				break;
			case GameObject::GO_STONESHED:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * .7f, 1.f, SceneData::GetInstance()->GetGridSize() * .7f);
				break;
			case GameObject::GO_HOUSE:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * .7f, 1.f, SceneData::GetInstance()->GetGridSize() * .7f);
				break;
			case GameObject::GO_BUSH:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * 0.75f, 1.f, SceneData::GetInstance()->GetGridSize() * 0.75f);
				break;
			case GameObject::GO_CHIEFHUT:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * 1.f, 0.5f, SceneData::GetInstance()->GetGridSize() * 1.f);
				break;
			case GameObject::GO_TREE:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * 1.f, 1.2f, SceneData::GetInstance()->GetGridSize() * 1.f);
				break;
			case GameObject::GO_ALTAR:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * 1.f, 1.f, SceneData::GetInstance()->GetGridSize() * 1.f);
				break;
			case GameObject::GO_RESEARCHLAB:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * 1.f, 2.f, SceneData::GetInstance()->GetGridSize() * 1.f);
				break;
			case GameObject::GO_MOUNTAIN:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * 1.f, 1.f, SceneData::GetInstance()->GetGridSize() * 1.f);
				break;
			case GameObject::GO_LOGS:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * 1.f, 1.f, SceneData::GetInstance()->GetGridSize() * 1.f);
				break;
			case GameObject::GO_TSUNAMI:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * 1.f, 1.f, SceneData::GetInstance()->GetGridSize() * 1.f);
				break;
			case GameObject::GO_TORNADO:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * 1.f, 1.f, SceneData::GetInstance()->GetGridSize() * 1.f);
				break;
			case GameObject::GO_METEOR:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * 1.f, 1.f, SceneData::GetInstance()->GetGridSize() * 1.f);
				break;
			}

			go->goTarget = NULL;
			return go;
		}
	}
	for (unsigned i = 0; i < 5; ++i)
	{
		GameObject *go = nullptr;
		switch (type)
		{
		case GameObject::GO_VILLAGER:
			go = new Villager(type);
			go->smID = "VillagerSM";
			go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Idle");
			break;
		case GameObject::GO_PIG:
			go = new Pig(type);
			go->smID = "PigSM";
			go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Idle");
			break;
		case GameObject::GO_BUILDING:
			go = new Building(type);
			break;
		case GameObject::GO_HOUSE:
			go = new House(type);
			break;
		case GameObject::GO_GRANARY:
			go = new Granary(type);
			break;
		case GameObject::GO_WOODSHED:
			go = new WoodShed(type);
			break;
		case GameObject::GO_STONESHED:
			go = new StoneShed(type);
			break;
		case GameObject::GO_CHIEFHUT:
			go = new ChiefHut(type);
			break;
		case GameObject::GO_ALTAR:
			go = new Altar(type);
			break;
		case GameObject::GO_RESEARCHLAB:
			go = new ResearchLab(type);
			break;
		case GameObject::GO_BUSH:
			go = new Bush(type);
			break;
		case GameObject::GO_TREE:
			go = new Tree(type);
			break;
		case GameObject::GO_MOUNTAIN:
			go = new Mountain(type);
			break;
		case GameObject::GO_LOGS:
			go = new Logs(type);
			break;
		case GameObject::GO_TSUNAMI:
			go = new Tsunami(type);
			break;
		case GameObject::GO_TORNADO:
			go = new Tornado(type);
			break;
		case GameObject::GO_METEOR:
			go = new Meteor(type);
			break;
		default:
			go = new GameObject(type);
			break;
		}
		if (go != nullptr)
			m_goList.push_back(go);
	}
	return FetchGO(type);
}

struct Compare2
{
	bool operator()(std::pair<GridPt, std::pair<int, int>> pair1, std::pair<GridPt, std::pair<int, int>> pair2)
	{
		return pair1.second.second < pair2.second.second;
	}
};

struct Compare3
{
	bool operator()(std::pair<GridPt, std::pair<float, float>> pair1, std::pair<GridPt, std::pair<float, float>> pair2)
	{
		return pair1.second.second < pair2.second.second;
	}
};


void SceneSP::AStarSingleGrid(GameObject * go, GridPt target)
{

	//first = curr point, second.first = tile cost, second.second = total cost(tile cost + distance to final destination)
	if (go->currentPt == target)
		return;
	std::vector<std::pair<GridPt, std::pair<float, float>>>priority_Queue;
	std::sort(priority_Queue.begin(), priority_Queue.end(), Compare3());
	while (!m_shortestPath.empty())
	{
		m_shortestPath.pop_back();
	}
	SceneData* SD = SceneData::GetInstance();
	go->m_visited.resize(SD->GetNoGrid() * SD->GetNoGrid());
	std::fill(go->m_visited.begin(), go->m_visited.end(), false);
	//keeping track of the parent node
	m_previous.resize(SD->GetNoGrid() * SD->GetNoGrid());
	std::fill(m_previous.begin(), m_previous.end(), NULL);

	GridPt curr = go->currentPt;
	if (!isPointInGrid(curr))
		return;

	GridPt bestStart = curr; //Closest gridPt to start from
	float fDistanceToTarget = (target.x - curr.x) * (target.x - curr.x) + (target.z - curr.z) * (target.z - curr.z);
	Vector3 ptPos = GetGridPos(curr); //Position of gridPt in world space

	//Check if goPos is left of or right of gridPt
	if (go->pos.x < ptPos.x)
	{
		//Check left of gridPt
		GridPt temp = GridPt(curr.x - 1, curr.z);
		if (isPointInGrid(temp))
		{
			float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
			if (distance < fDistanceToTarget)
			{
				fDistanceToTarget = distance;
				bestStart = temp;
			}
		}
		//Check if goPos is above or below center of gridPt
		if (go->pos.z < ptPos.z)
		{
			//Check above of gridPt
			temp = GridPt(curr.x, curr.z - 1);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
			//Check diagonal
			temp = GridPt(curr.x - 1, curr.z - 1);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
		}
		else
		{
			//Check below of gridPt
			temp = GridPt(curr.x, curr.z + 1);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
			//Check diagonal
			temp = GridPt(curr.x - 1, curr.z + 1);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
		}
	}
	else
	{
		GridPt temp;
		//Check right of gridPt
		if (go->pos.x > ptPos.x)
		{
			temp = GridPt(curr.x + 1, curr.z);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
		}
		//Check if goPos is above or below center of gridPt
		if (go->pos.z < ptPos.z)
		{
			//Check above of gridPt
			temp = GridPt(curr.x, curr.z - 1);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
			//Check diagonal
			if (go->pos.x > ptPos.x)
			{
				temp = GridPt(curr.x + 1, curr.z - 1);
				if (isPointInGrid(temp))
				{
					float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
					if (distance < fDistanceToTarget)
					{
						fDistanceToTarget = distance;
						bestStart = temp;
					}
				}
			}
		}
		else if (go->pos.z > ptPos.z)
		{
			//Check below of gridPt
			temp = GridPt(curr.x, curr.z + 1);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
			//Check diagonal
			temp = GridPt(curr.x + 1, curr.z + 1);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
		}
	}

	float G = 0;
	priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(bestStart, std::pair<float, float>(G, 0)));
	go->m_visited[GetGridIndex(bestStart)] = true;
	float nearestDistance = FLT_MAX;
	GridPt nearestTile = bestStart;
	GridPt next = bestStart;
	int currIndex = 0;

	for (int loop = 0; loop < SD->GetNoGrid() * SD->GetNoGrid() && !priority_Queue.empty(); ++loop)
	{
		//std::cout << "One Round of Loop" << std::endl;
		curr = priority_Queue.begin()->first;
		//m_queue.pop();
		if (curr == target)
		{
			break;
		}
		if ((target.x - curr.x) * (target.x - curr.x) <= 1 && (target.z - curr.z) * (target.z - curr.z) <= 1)
		{
			if (target != curr)
				m_previous[GetGridIndex(target)] = curr;
			priority_Queue.begin()->first = target;
			break;
		}

		//Check Up
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x, curr.z - 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
					go->m_visited[currIndex] = true;

					//Check if it is the nearest Tile
					if (distanceSquared < nearestDistance)
					{
						nearestDistance = distanceSquared;
						nearestTile = next;
					}
				}
				else if (distanceSquared != NULL)
				{
					//Checks if visited node is in queue
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		//Check Down
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x, curr.z + 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
					go->m_visited[currIndex] = true;

					//Check if it is the nearest Tile
					if (distanceSquared < nearestDistance)
					{
						nearestDistance = distanceSquared;
						nearestTile = next;
					}
				}
				else if (distanceSquared != NULL)
				{
					//Checks if visited node is in queue
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		//Check Left
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x - 1, curr.z);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
					go->m_visited[currIndex] = true;

					//Check if it is the nearest Tile
					if (distanceSquared < nearestDistance)
					{
						nearestDistance = distanceSquared;
						nearestTile = next;
					}
				}
				else if (distanceSquared != NULL)
				{
					//Checks if visited node is in queue
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		//Check Right
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x + 1, curr.z);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
					go->m_visited[currIndex] = true;

					//Check if it is the nearest Tile
					if (distanceSquared < nearestDistance)
					{
						nearestDistance = distanceSquared;
						nearestTile = next;
					}
				}
				else if (distanceSquared != NULL)
				{
					//Checks if visited node is in queue
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		//Check Up Left
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x - 1, curr.z - 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += sqrtf(2);
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
					go->m_visited[currIndex] = true;

					//Check if it is the nearest Tile
					if (distanceSquared < nearestDistance)
					{
						nearestDistance = distanceSquared;
						nearestTile = next;
					}
				}
				else if (distanceSquared != NULL)
				{
					//Checks if visited node is in queue
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		//Check Up Right
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x + 1, curr.z - 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += sqrtf(2);
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
					go->m_visited[currIndex] = true;

					//Check if it is the nearest Tile
					if (distanceSquared < nearestDistance)
					{
						nearestDistance = distanceSquared;
						nearestTile = next;
					}
				}
				else if (distanceSquared != NULL)
				{
					//Checks if visited node is in queue
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		//Check Down Left
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x - 1, curr.z + 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += sqrtf(2);
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
					go->m_visited[currIndex] = true;

					//Check if it is the nearest Tile
					if (distanceSquared < nearestDistance)
					{
						nearestDistance = distanceSquared;
						nearestTile = next;
					}
				}
				else if (distanceSquared != NULL)
				{
					//Checks if visited node is in queue
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		//Check Down Right
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x + 1, curr.z + 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += sqrtf(2);
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
					go->m_visited[currIndex] = true;

					//Check if it is the nearest Tile
					if (distanceSquared < nearestDistance)
					{
						nearestDistance = distanceSquared;
						nearestTile = next;
					}
				}
				else if (distanceSquared != NULL)
				{
					//Checks if visited node is in queue
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		priority_Queue.erase(priority_Queue.begin());
		std::sort(priority_Queue.begin(), priority_Queue.end(), Compare3());
	}

	if (priority_Queue.size() > 0)
	{
		if (priority_Queue.begin()->first == target)
		{
			//If manage to reach target
			curr = target;
			m_shortestPath.push_back(curr);
			curr = m_previous[GetGridIndex(curr)];
			while (curr != NULL)
			{
				m_shortestPath.push_back(curr);
				curr = m_previous[GetGridIndex(curr)];
			}
		}
		else
		{
			//If unable to reach target
			curr = nearestTile;
			m_shortestPath.push_back(curr);
			curr = m_previous[GetGridIndex(curr)];
			while (curr != NULL)
			{
				m_shortestPath.push_back(curr);
				curr = m_previous[GetGridIndex(curr)];
			}
		}
	}
	else
	{
		//If something happened
		curr = nearestTile;
		m_shortestPath.push_back(curr);
		curr = m_previous[GetGridIndex(curr)];
		while (curr != NULL)
		{
			m_shortestPath.push_back(curr);
			curr = m_previous[GetGridIndex(curr)];
		}
	}

	//Just take based on range of movement 
	while (!m_shortestPath.empty())
	{
		go->path.push_back(m_shortestPath.back());
		m_shortestPath.pop_back();
	}
	if (go->path.size() > 1)
	{
		std::reverse(go->path.begin(), go->path.end());
	}
	//std::cout << priority_Queue.begin()->second.first << std::endl;
	//return priority_Queue.begin()->second.first;
}

//
//void SceneSP::AStarMultiGrid(GameObject * go, GridPt target)
//{
//
//	//first = curr point, second.first = tile cost, second.second = total cost(tile cost + distance to final destination)
//	if (go->currentPt == target)
//		return;
//	std::vector<std::pair<GridPt, std::pair<int, int>>>priority_Queue;
//	std::sort(priority_Queue.begin(), priority_Queue.end(), Compare2());
//	while (!m_shortestPath.empty())
//	{
//		m_shortestPath.pop_back();
//	}
//	SceneData* SD = SceneData::GetInstance();
//	go->m_visited.resize(SD->GetNoGrid() * SD->GetNoGrid());
//	std::fill(go->m_visited.begin(), go->m_visited.end(), false);
//	//keeping track of the parent node
//	m_previous.resize(SD->GetNoGrid() * SD->GetNoGrid());
//	std::fill(m_previous.begin(), m_previous.end(), NULL);
//
//	GridPt curr = go->currentPt;
//	if (!isPointInGrid(curr))
//		return;
//
//	GridPt bestStart = curr; //Closest gridPt to start from
//	float fDistanceToTarget = (target.x - curr.x) * (target.x - curr.x) + (target.z - curr.z) * (target.z - curr.z);
//	Vector3 ptPos = GetGridPos(curr); //Position of gridPt in world space
//
//	//Check if goPos is left of or right of gridPt
//	if (go->pos.x < ptPos.x)
//	{
//		//Check left of gridPt
//		GridPt temp = GridPt(curr.x - 1, curr.z);
//		if (isPointInGrid(temp))
//		{
//			if (isTheCoastClear(go, temp, Grid::DIR_LEFT))
//			{
//				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
//				if (distance < fDistanceToTarget)
//				{
//					fDistanceToTarget = distance;
//					bestStart = temp;
//				}
//			}
//		}
//		//Check if goPos is above or below center of gridPt
//		if (go->pos.z < ptPos.z)
//		{
//			//Check above of gridPt
//			temp = GridPt(curr.x, curr.z - 1);
//			if (isPointInGrid(temp))
//			{
//				if (isTheCoastClear(go, temp, Grid::DIR_UP))
//				{
//					float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
//					if (distance < fDistanceToTarget)
//					{
//						fDistanceToTarget = distance;
//						bestStart = temp;
//					}
//				}
//			}
//			//Check diagonal
//			temp = GridPt(curr.x - 1, curr.z - 1);
//			if (isPointInGrid(temp))
//			{
//				if (isTheCoastClear(go, temp, Grid::DIR_LEFTUP))
//				{
//					float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
//					if (distance < fDistanceToTarget)
//					{
//						fDistanceToTarget = distance;
//						bestStart = temp;
//					}
//				}
//			}
//		}
//		else
//		{
//			//Check below of gridPt
//			temp = GridPt(curr.x, curr.z + 1);
//			if (isPointInGrid(temp))
//			{
//				if (isTheCoastClear(go, temp, Grid::DIR_DOWN))
//				{
//					float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
//					if (distance < fDistanceToTarget)
//					{
//						fDistanceToTarget = distance;
//						bestStart = temp;
//					}
//				}
//			}
//			//Check diagonal
//			temp = GridPt(curr.x - 1, curr.z + 1);
//			if (isPointInGrid(temp))
//			{
//				if (isTheCoastClear(go, temp, Grid::DIR_LEFTDOWN))
//				{
//					float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
//					if (distance < fDistanceToTarget)
//					{
//						fDistanceToTarget = distance;
//						bestStart = temp;
//					}
//				}
//			}
//		}
//	}
//	else
//	{
//		GridPt temp;
//		//Check right of gridPt
//		if (go->pos.x > ptPos.x)
//		{
//			temp = GridPt(curr.x + 1, curr.z);
//			if (isPointInGrid(temp))
//			{
//				if (isTheCoastClear(go, temp, Grid::DIR_RIGHT))
//				{
//					float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
//					if (distance < fDistanceToTarget)
//					{
//						fDistanceToTarget = distance;
//						bestStart = temp;
//					}
//				}
//			}
//		}
//		//Check if goPos is above or below center of gridPt
//		if (go->pos.z < ptPos.z)
//		{
//			//Check above of gridPt
//			temp = GridPt(curr.x, curr.z - 1);
//			if (isPointInGrid(temp))
//			{
//				if (isTheCoastClear(go, temp, Grid::DIR_UP))
//				{
//					float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
//					if (distance < fDistanceToTarget)
//					{
//						fDistanceToTarget = distance;
//						bestStart = temp;
//					}
//				}
//			}
//			//Check diagonal
//			if (go->pos.x > ptPos.x)
//			{
//				temp = GridPt(curr.x + 1, curr.z - 1);
//				if (isPointInGrid(temp))
//				{
//					if (isTheCoastClear(go, temp, Grid::DIR_RIGHTUP))
//					{
//						float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
//						if (distance < fDistanceToTarget)
//						{
//							fDistanceToTarget = distance;
//							bestStart = temp;
//						}
//					}
//				}
//			}
//		}
//		else if (go->pos.z > ptPos.z)
//		{
//			//Check below of gridPt
//			temp = GridPt(curr.x, curr.z + 1);
//			if (isPointInGrid(temp))
//			{
//				if (isTheCoastClear(go, temp, Grid::DIR_DOWN))
//				{
//					float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
//					if (distance < fDistanceToTarget)
//					{
//						fDistanceToTarget = distance;
//						bestStart = temp;
//					}
//				}
//			}
//			//Check diagonal
//			temp = GridPt(curr.x + 1, curr.z + 1);
//			if (isPointInGrid(temp))
//			{
//				if (isTheCoastClear(go, temp, Grid::DIR_RIGHTDOWN))
//				{
//					float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
//					if (distance < fDistanceToTarget)
//					{
//						fDistanceToTarget = distance;
//						bestStart = temp;
//					}
//				}
//			}
//		}
//	}
//
//	int G = 0;
//	priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(bestStart, std::pair<int, int>(G, 0)));
//	go->m_visited[GetGridIndex(bestStart)] = true;
//	float nearestDistance = FLT_MAX;
//	GridPt nearestTile = bestStart;
//	GridPt next = bestStart;
//	int currIndex = 0;
//
//	//Since this value is rounded down, this value is for the closer side of GridPt, refer to gridImage in beside solution file
//	int iHalfGridX = go->iGridX * 0.5f;
//	int iHalfGridZ = go->iGridZ * 0.5f;
//
//
//	for (int loop = 0; loop < SD->GetNoGrid() * SD->GetNoGrid() && !priority_Queue.empty(); ++loop)
//	{
//		//std::cout << "One Round of Loop" << std::endl;
// 		curr = priority_Queue.begin()->first;
//		//m_queue.pop();
//
//
//		if (go->iGridX > 0 && go->iGridZ > 0)
//		{
//			if ((target.x - curr.x) * (target.x - curr.x) <= (go->iGridX - iHalfGridX + 1) * (go->iGridX - iHalfGridX + 1) && (target.z - curr.z) * (target.z - curr.z) <= (go->iGridZ - iHalfGridZ + 1) * (go->iGridZ - iHalfGridZ + 1))
//			{
//				if(target != curr)
//					m_previous[GetGridIndex(target)] = curr;
//				priority_Queue.begin()->first = target;
//				break;
//			}
//		}
//		else
//		{
//			if (curr == target)
//			{
//				break;
//			}
//		}
//
//		//Check Up
//		G = priority_Queue.begin()->second.first;
//		next.Set(curr.x, curr.z - 1);
//		currIndex = GetGridIndex(next);
//		if (isPointInGrid(next))
//		{
//			float distanceSquared = NULL;
//			//Cost of Empty Tile
//			G += 1;
//			//Calculate distance to end
//			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
//			if (isTheCoastClear(go, next, Grid::DIR_UP))
//			{
//				if (!go->m_visited[currIndex])
//				{
//					//Update node cost
//					m_previous[currIndex] = curr;
//					priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//					go->m_visited[currIndex] = true;
//
//					//Check if it is the nearest Tile
//					if (distanceSquared < nearestDistance)
//					{
//						nearestDistance = distanceSquared;
//						nearestTile = next;
//					}
//				}
//				else if (distanceSquared != NULL)
//				{
//					//Checks if visited node is in queue
//					for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
//					{
//						std::pair<GridPt, std::pair<int, int>> path = *it;
//						if (path.first == next)
//						{
//							//If in queue, check if new path is shorter thn old path
//							if (path.second.second > G + distanceSquared)
//							{
//								//Update if new path is shorter
//								priority_Queue.erase(it);
//								priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//								m_previous[currIndex] = curr;
//							}
//							break;
//						}
//					}
//					//If not in queue, node is already in optimal path
//				}
//			}
//		}
//
//		//Check Down
//		G = priority_Queue.begin()->second.first;
//		next.Set(curr.x, curr.z + 1);
//		currIndex = GetGridIndex(next);
//		if (isPointInGrid(next))
//		{
//			float distanceSquared = NULL;
//			//Cost of Empty Tile
//			G += 1;
//			//Calculate distance to end
//			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
//			if (isTheCoastClear(go, next, Grid::DIR_DOWN))
//			{
//				if (!go->m_visited[currIndex])
//				{
//					//Update node cost
//					m_previous[currIndex] = curr;
//					priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//					go->m_visited[currIndex] = true;
//
//					//Check if it is the nearest Tile
//					if (distanceSquared < nearestDistance)
//					{
//						nearestDistance = distanceSquared;
//						nearestTile = next;
//					}
//				}
//				else if (distanceSquared != NULL)
//				{
//					//Checks if visited node is in queue
//					for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
//					{
//						std::pair<GridPt, std::pair<int, int>> path = *it;
//						if (path.first == next)
//						{
//							//If in queue, check if new path is shorter thn old path
//							if (path.second.second > G + distanceSquared)
//							{
//								//Update if new path is shorter
//								priority_Queue.erase(it);
//								priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//								m_previous[currIndex] = curr;
//							}
//							break;
//						}
//					}
//					//If not in queue, node is already in optimal path
//				}
//			}
//		}
//
//		//Check Left
//		G = priority_Queue.begin()->second.first;
//		next.Set(curr.x - 1, curr.z);
//		currIndex = GetGridIndex(next);
//		if (isPointInGrid(next))
//		{
//			float distanceSquared = NULL;
//			//Cost of Empty Tile
//			G += 1;
//			//Calculate distance to end
//			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
//			if (isTheCoastClear(go, next, Grid::DIR_LEFT))
//			{
//				if (!go->m_visited[currIndex])
//				{
//					//Update node cost
//					m_previous[currIndex] = curr;
//					priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//					go->m_visited[currIndex] = true;
//
//					//Check if it is the nearest Tile
//					if (distanceSquared < nearestDistance)
//					{
//						nearestDistance = distanceSquared;
//						nearestTile = next;
//					}
//				}
//				else if (distanceSquared != NULL)
//				{
//					//Checks if visited node is in queue
//					for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
//					{
//						std::pair<GridPt, std::pair<int, int>> path = *it;
//						if (path.first == next)
//						{
//							//If in queue, check if new path is shorter thn old path
//							if (path.second.second > G + distanceSquared)
//							{
//								//Update if new path is shorter
//								priority_Queue.erase(it);
//								priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//								m_previous[currIndex] = curr;
//							}
//							break;
//						}
//					}
//					//If not in queue, node is already in optimal path
//				}
//			}
//		}
//
//		//Check Right
//		G = priority_Queue.begin()->second.first;
//		next.Set(curr.x + 1, curr.z);
//		currIndex = GetGridIndex(next);
//		if (isPointInGrid(next))
//		{
//			float distanceSquared = NULL;
//			//Cost of Empty Tile
//			G += 1;
//			//Calculate distance to end
//			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
//			if (isTheCoastClear(go, next, Grid::DIR_RIGHT))
//			{
//				if (!go->m_visited[currIndex])
//				{
//					//Update node cost
//					m_previous[currIndex] = curr;
//					priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//					go->m_visited[currIndex] = true;
//
//					//Check if it is the nearest Tile
//					if (distanceSquared < nearestDistance)
//					{
//						nearestDistance = distanceSquared;
//						nearestTile = next;
//					}
//				}
//				else if (distanceSquared != NULL)
//				{
//					//Checks if visited node is in queue
//					for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
//					{
//						std::pair<GridPt, std::pair<int, int>> path = *it;
//						if (path.first == next)
//						{
//							//If in queue, check if new path is shorter thn old path
//							if (path.second.second > G + distanceSquared)
//							{
//								//Update if new path is shorter
//								priority_Queue.erase(it);
//								priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//								m_previous[currIndex] = curr;
//							}
//							break;
//						}
//					}
//					//If not in queue, node is already in optimal path
//				}
//			}
//		}
//
//		//Check Up Left
//		G = priority_Queue.begin()->second.first;
//		next.Set(curr.x - 1, curr.z - 1);
//		currIndex = GetGridIndex(next);
//		if (isPointInGrid(next))
//		{
//			float distanceSquared = NULL;
//			//Cost of Empty Tile
//			G += 1;
//			//Calculate distance to end
//			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
//			if (isTheCoastClear(go, next, Grid::DIR_LEFTUP))
//			{
//				if (!go->m_visited[currIndex])
//				{
//					//Update node cost
//					m_previous[currIndex] = curr;
//					priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//					go->m_visited[currIndex] = true;
//
//					//Check if it is the nearest Tile
//					if (distanceSquared < nearestDistance)
//					{
//						nearestDistance = distanceSquared;
//						nearestTile = next;
//					}
//				}
//				else if (distanceSquared != NULL)
//				{
//					//Checks if visited node is in queue
//					for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
//					{
//						std::pair<GridPt, std::pair<int, int>> path = *it;
//						if (path.first == next)
//						{
//							//If in queue, check if new path is shorter thn old path
//							if (path.second.second > G + distanceSquared)
//							{
//								//Update if new path is shorter
//								priority_Queue.erase(it);
//								priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//								m_previous[currIndex] = curr;
//							}
//							break;
//						}
//					}
//					//If not in queue, node is already in optimal path
//				}
//			}
//		}
//
//		//Check Up Right
//		G = priority_Queue.begin()->second.first;
//		next.Set(curr.x + 1, curr.z - 1);
//		currIndex = GetGridIndex(next);
//		if (isPointInGrid(next))
//		{
//			float distanceSquared = NULL;
//			//Cost of Empty Tile
//			G += 1;
//			//Calculate distance to end
//			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
//			if (isTheCoastClear(go, next, Grid::DIR_RIGHTUP))
//			{
//				if (!go->m_visited[currIndex])
//				{
//					//Update node cost
//					m_previous[currIndex] = curr;
//					priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//					go->m_visited[currIndex] = true;
//
//					//Check if it is the nearest Tile
//					if (distanceSquared < nearestDistance)
//					{
//						nearestDistance = distanceSquared;
//						nearestTile = next;
//					}
//				}
//				else if (distanceSquared != NULL)
//				{
//					//Checks if visited node is in queue
//					for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
//					{
//						std::pair<GridPt, std::pair<int, int>> path = *it;
//						if (path.first == next)
//						{
//							//If in queue, check if new path is shorter thn old path
//							if (path.second.second > G + distanceSquared)
//							{
//								//Update if new path is shorter
//								priority_Queue.erase(it);
//								priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//								m_previous[currIndex] = curr;
//							}
//							break;
//						}
//					}
//					//If not in queue, node is already in optimal path
//				}
//			}
//		}
//
//		//Check Down Left
//		G = priority_Queue.begin()->second.first;
//		next.Set(curr.x - 1, curr.z + 1);
//		currIndex = GetGridIndex(next);
//		if (isPointInGrid(next))
//		{
//			float distanceSquared = NULL;
//			//Cost of Empty Tile
//			G += 1;
//			//Calculate distance to end
//			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
//			if (isTheCoastClear(go, next, Grid::DIR_LEFTDOWN))
//			{
//				if (!go->m_visited[currIndex])
//				{
//					//Update node cost
//					m_previous[currIndex] = curr;
//					priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//					go->m_visited[currIndex] = true;
//
//					//Check if it is the nearest Tile
//					if (distanceSquared < nearestDistance)
//					{
//						nearestDistance = distanceSquared;
//						nearestTile = next;
//					}
//				}
//				else if (distanceSquared != NULL)
//				{
//					//Checks if visited node is in queue
//					for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
//					{
//						std::pair<GridPt, std::pair<int, int>> path = *it;
//						if (path.first == next)
//						{
//							//If in queue, check if new path is shorter thn old path
//							if (path.second.second > G + distanceSquared)
//							{
//								//Update if new path is shorter
//								priority_Queue.erase(it);
//								priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//								m_previous[currIndex] = curr;
//							}
//							break;
//						}
//					}
//					//If not in queue, node is already in optimal path
//				}
//			}
//		}
//
//		//Check Down Right
//		G = priority_Queue.begin()->second.first;
//		next.Set(curr.x + 1, curr.z + 1);
//		currIndex = GetGridIndex(next);
//		if (isPointInGrid(next))
//		{
//			float distanceSquared = NULL;
//			//Cost of Empty Tile
//			G += 1;
//			//Calculate distance to end
//			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
//			if (isTheCoastClear(go, next, Grid::DIR_RIGHTDOWN))
//			{
//				if (!go->m_visited[currIndex])
//				{
//					//Update node cost
//					m_previous[currIndex] = curr;
//					priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//					go->m_visited[currIndex] = true;
//
//					//Check if it is the nearest Tile
//					if (distanceSquared < nearestDistance)
//					{
//						nearestDistance = distanceSquared;
//						nearestTile = next;
//					}
//				}
//				else if (distanceSquared != NULL)
//				{
//					//Checks if visited node is in queue
//					for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
//					{
//						std::pair<GridPt, std::pair<int, int>> path = *it;
//						if (path.first == next)
//						{
//							//If in queue, check if new path is shorter thn old path
//							if (path.second.second > G + distanceSquared)
//							{
//								//Update if new path is shorter
//								priority_Queue.erase(it);
//								priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
//								m_previous[currIndex] = curr;
//							}
//							break;
//						}
//					}
//					//If not in queue, node is already in optimal path
//				}
//			}
//		}
//
//		priority_Queue.erase(priority_Queue.begin());
//		std::sort(priority_Queue.begin(), priority_Queue.end(), Compare2());
//	}
//
//	if (priority_Queue.size() > 0)
//	{
//		if (priority_Queue.begin()->first == target)
//		{
//			//If manage to reach target
//			curr = target;
//			m_shortestPath.push_back(curr);
//			curr = m_previous[GetGridIndex(curr)];
//			while (curr != NULL)
//			{
//				m_shortestPath.push_back(curr);
//				curr = m_previous[GetGridIndex(curr)];
//				std::cout << "hi" << std::endl;
//			}
//		}
//		else
//		{
//			//If unable to reach target
//			curr = nearestTile;
//			m_shortestPath.push_back(curr);
//			curr = m_previous[GetGridIndex(curr)];
//			while (curr != NULL)
//			{
//				m_shortestPath.push_back(curr);
//				curr = m_previous[GetGridIndex(curr)];
//				std::cout << "hi" << std::endl;
//			}
//		}
//	}
//	else
//	{
//		//If something happened
//		curr = nearestTile;
//		m_shortestPath.push_back(curr);
//		curr = m_previous[GetGridIndex(curr)];
//		while (curr != NULL)
//		{
//			m_shortestPath.push_back(curr);
//			curr = m_previous[GetGridIndex(curr)];
//			std::cout << "hi" << std::endl;
//		}
//	}
//
//	//Just take based on range of movement 
//	while (!m_shortestPath.empty())
//	{
//		go->path.push_back(m_shortestPath.back());
//		m_shortestPath.pop_back();
//	}
//	if (go->path.size() > 1)
//	{
//		std::reverse(go->path.begin(), go->path.end());
//	}
//	//std::cout << priority_Queue.begin()->second.first << std::endl;
//	//return priority_Queue.begin()->second.first;
//}

//void SceneSP::AStar(GameObject * go, Vector3 target)
//{
//	go->m_visited.resize(m_graph.nodeID);
//	std::fill(go->m_visited.begin(), go->m_visited.end(), false);
//	go->gPath.clear();
//	node* nearestToTarget = m_graph.NearestNode(target);
//
//	//node* is the current node, first float is cost to node, second float is cost to node + distance to end
//	std::vector<std::pair<node*, std::pair<float, float>>> m_queue;
//	m_queue.push_back(std::pair<node*, std::pair<float, float>>(go->currNode, std::pair<float, float>(0.f, (nearestToTarget->pos - go->currNode->pos).LengthSquared())));
//	while (m_queue.front().first != nearestToTarget && !m_queue.empty())
//	{
//
//		std::pair<node*, std::pair<float, float>> curr = m_queue.front();
//		node* currNode = curr.first;
//
//		for (auto edges : currNode->edges)
//		{
//			float cost = curr.second.first + edges->length;
//			float costToEnd = cost + (nearestToTarget->pos - edges->dst->pos).LengthSquared();
//			std::pair<node*, std::pair<float, float>> value(edges->dst, std::pair<float, float>(cost, costToEnd));
//			if (!go->m_visited[edges->dst->ID - 1])
//			{
//				edges->dst->previous = currNode;
//				go->m_visited[edges->dst->ID - 1] = true;
//				bool inserted = false;
//				for (std::vector<std::pair<node*, std::pair<float, float>>>::iterator it = m_queue.begin(); it != m_queue.end(); ++it)
//				{
//					std::pair<node*, std::pair<float, float>> temp = *it;
//					if (value.second.second < temp.second.second)
//					{
//						m_queue.insert(it,value);
//						inserted = true;
//						break;
//					}
//				}
//				if (!inserted)
//				{
//					m_queue.push_back(value);
//				}
//			}
//			else
//			{
//				for (auto go : m_queue)
//				{
//					if (go.first == value.first)
//					{
//						edges->dst->previous = currNode;
//						go = value;
//						break;
//					}
//				}
//			}
//		}
//		std::vector<std::pair<node*, std::pair<float, float>>>::iterator tempit;
//		tempit = std::find(m_queue.begin(), m_queue.end(), curr);
//		m_queue.erase(tempit);
//	}
//
//	node* curr = nearestToTarget;
//	while (curr != go->currNode)
//	{
//		go->gPath.push_back(curr->pos);
//		curr = curr->previous;
//	}
//}
//
//void SceneSP::DFSOnce(GameObject * go)
//{
//	go->gStack.push_back(go->currNode);
//	go->m_visited[go->currNode->ID - 1] = true;
//	for (auto edge : go->currNode->edges)
//	{
//		if (!go->m_visited[edge->dst->ID - 1])
//		{
//			go->currNode = edge->dst;
//			return;
//		}
//	}
//	go->gStack.pop_back();
//	if (!go->gStack.empty())
//	{
//		go->currNode = go->gStack.back();
//		go->gStack.pop_back();
//	}
//	return;
//}

void SceneSP::UpdateSelectedUI()
{
	int increment = 0;
	for (auto UI : m_selectedUi)
	{
		//UI->bIsDone = true;
		UITween* newTween = new UITween(
			UI
			, 0.5f
			, UITween::ES_BACK
			, UITween::ED_IN
			, 0
			, increment * 0.05f
			, UITween::CALLBACK_SET_UI_DONE
		);
		newTween->properties_goal["pos"] = Vector3(UI->pos.x + 0.5f, UI->pos.y, UI->pos.z);
		UI->AddTween(newTween);
		++increment;
	}
	/*for (auto UI : m_selectedUi)
		UI->bIsDone = true;*/
	m_selectedUi.clear();
	reticle->selected = selected;
	if (selected == NULL) return;
	if (selected->type == GameObject::GO_ALTAR)
	{
		Building* selectedBuilding = static_cast<Building*>(selected);
		UIBase* newUI = new UIGameText(UIGameText::TEXT_SELECTED_ALTAR);
		UIManager::GetInstance()->AddUI("uiSelected_Altar_Info", newUI);
		m_selectedUi.push_back(newUI);
		if (selectedBuilding->eCurrState == Building::STATES::COMPLETED)
		{
			newUI = new UIGameButton(UIGameButton::BUTTON_SELECTED_GENERAL_MOVE, 0, 0, 0);
			UIManager::GetInstance()->AddUI("uiSelected_Altar_Move", newUI);
			m_selectedUi.push_back(newUI);
		}
		newUI = new UIGameButton(UIGameButton::BUTTON_SELECTED_ALTAR_OFFER, 0, 0, 1);
		UIManager::GetInstance()->AddUI("uiSelected_Altar_Offer", newUI);
		m_selectedUi.push_back(newUI);
	}
	/*else if (selected->type == GameObject::GO_RESEARCHLAB)
	{
		UIBase* newUI = new UIGameText(UIGameText::TEXT_SELECTED_RLAB);
		UIManager::GetInstance()->AddUI("uiSelected_RLab_Info", newUI);
		m_selectedUi.push_back(newUI);
		newUI = new UIGameButton(UIGameButton::BUTTON_SELECTED_GENERAL_MOVE, 0, 0, 0);
		UIManager::GetInstance()->AddUI("uiSelected_RLab_Move", newUI);
		m_selectedUi.push_back(newUI);
	}*/
	else if (dynamic_cast<Building*>(selected))
	{
		UIBase* newUI = new UIGameText(UIGameText::TEXT_SELECTED_BUILDING, 0, 0, selected);
		UIManager::GetInstance()->AddUI("uiSelected_Building_Info", newUI);
		m_selectedUi.push_back(newUI);
		if (static_cast<Building*>(selected)->eCurrState == Building::STATES::COMPLETED)
		{
			newUI = new UIGameButton(UIGameButton::BUTTON_SELECTED_GENERAL_MOVE, 0, 0, 0, selected);
			UIManager::GetInstance()->AddUI("uiSelected_Building_Move", newUI);
			m_selectedUi.push_back(newUI);
		}
		if (selected->type == GameObject::GO_CHIEFHUT)
		{
			newUI = new UIGameButton(UIGameButton::BUTTON_SELECTED_CHIEFHUT_BUILD, 0, 0, 1, selected);
			UIManager::GetInstance()->AddUI("uiSelected_Chiefhut_Build", newUI);
			m_selectedUi.push_back(newUI);
		}
		else if (selected->type == GameObject::GO_RESEARCHLAB)
		{
			newUI = new UIGameButton(UIGameButton::BUTTON_SELECTED_RLAB, 0, 0, 1);
			UIManager::GetInstance()->AddUI("uiSelected_RLab", newUI);
			m_selectedUi.push_back(newUI);
		}
	}
	else if (dynamic_cast<Villager*>(selected))
	{
		UIBase* newUI = new UIGameText(UIGameText::TEXT_SELECTED_VILLAGER, 0, 0, selected);
		UIManager::GetInstance()->AddUI("uiSelected_Villager_Info", newUI);
		m_selectedUi.push_back(newUI);
	}
	else if (dynamic_cast<Pig*>(selected))
	{
		UIBase* newUI = new UIGameText(UIGameText::TEXT_SELECTED_PIG, 0, 0, selected);
		UIManager::GetInstance()->AddUI("uiSelected_Pig_Info", newUI);
		m_selectedUi.push_back(newUI);
	}
	else if (dynamic_cast<Environment*>(selected))
	{
		UIBase* newUI = new UIGameText(UIGameText::TEXT_SELECTED_ENVIRONMENT, 0, 0, selected);
		UIManager::GetInstance()->AddUI("uiSelected_Environment_Info", newUI);
		m_selectedUi.push_back(newUI);
	}
	this->DoSelectedUITween();
}

void SceneSP::DoSelectedUITween()
{
	// apply tweens to the selected UI << START >>
	int increment = 0;
	for (auto UI : m_selectedUi)
	{
		Vector3 origPos = UI->pos;
		UI->pos.Set(1.35f, UI->pos.y, UI->pos.z);
		UITween* newTween = new UITween(
			UI
			, 0.3f
			, UITween::ES_CUBIC
			, UITween::ED_OUT
			, 0
			, increment * 0.075f
		);
		newTween->properties_goal["pos"] = origPos;
		UI->AddTween(newTween);
		++increment;
	}
	// apply tweens to the selected UI << END >>
}

GameObject * SceneSP::GetChiefHut()
{
	return this->goChiefHut;
}

GameObject * SceneSP::GetAltar()
{
	return this->goAltar;
}

GameObject * SceneSP::GetResearchLab()
{
	return this->goResearchLab;
}

void SceneSP::Reset()
{
	//Cleanup GameObjects
	/*while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	*/
	m_speed = 1.f;
	selected = NULL;
	for (auto UI : m_coreUi)
		UI->bIsDone = true;
	m_coreUi.clear();
	for (auto UI : m_selectedUi)
		UI->bIsDone = true;
	m_selectedUi.clear();
	CSoundEngine::GetInstance()->GetSoundEngine()->stopAllSounds();
	ChangeState(G_MAINMENU);
}

void SceneSP::ChangeTimeOfDay()
{
	bDay = !bDay;
	if (bDay) // daytime reset
	{
		//light
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		lights[0].position.Set((12.f - fTimeOfDay), /*(-0.25f * pow((12.f - fTimeOfDay), 2)) + */9, 5.f);
		lights[0].color.Set(1, 1, 1);
		lights[0].power = 1.f;
		lights[0].kC = 1.f;
		lights[0].kL = 0.01f;
		lights[0].kQ = 0.001f;
		lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
		lights[0].cosInner = cos(Math::DegreeToRadian(30));
		lights[0].exponent = 3.f;
		lights[0].spotDirection.Set(0.f, 1.f, 0.f);

		//bGodlights = false;
		SceneData* SD = SceneData::GetInstance();
		SD->SetCurrDay(SD->GetCurrDay() + 1);
		int iAmountOfTrees, iAmountOfBushes, iAmountOfMountains, iAmountOfPigs;
		iAmountOfTrees = iAmountOfBushes = iAmountOfMountains = iAmountOfPigs = 0;
		//Resetting objects that need to be reset
		for (auto go : m_goList)
		{
			if (!go->active)
				continue;
			if (go->type == GameObject::GO_BUSH)
			{
				++iAmountOfBushes;
				if (static_cast<Bush*>(go)->eCurrState == Bush::DEPLETED)
					static_cast<Bush*>(go)->eCurrState = Bush::LUSH;
			}
			else if (go->type == GameObject::GO_MOUNTAIN)
			{
				++iAmountOfMountains;
			}
			else if (go->type == GameObject::GO_TREE)
			{
				++iAmountOfTrees;
			}
			else if (go->type == GameObject::GO_PIG)
			{
				++iAmountOfPigs;
			}
			else if (go->type == GameObject::GO_GRANARY)
			{
				if (static_cast<Building*>(go)->eCurrState == Building::COMPLETED)
				{
					SD->SetFood(Math::Min(SD->GetFoodLimit(), SD->GetFood() + 4));
				}
			}
		}

		//Spawning of Villagers
		if (SD->GetPopulation() < SD->GetPopulationLimit())
		{
			int iDifference = SD->GetPopulationLimit() - SD->GetPopulation();
			GameObject* go = NULL;
			Villager* goVil = NULL;
			//If more then 10 empty spaces, at least 1 will spawn
			if (iDifference > 10)
			{
				go = FetchGO(GameObject::GO_VILLAGER);
				
				go->scale.y = Math::RandFloatMinMax(0.5f, 0.9f);
				GridPt tempPt;
				tempPt.Set(Math::RandIntMinMax(0, SD->GetNoGrid() - 1), Math::RandIntMinMax(0, SD->GetNoGrid() - 1));
				while (!m_grid[GetGridIndex(tempPt)] == Grid::TILE_EMPTY)
				{
					tempPt.Set(Math::RandIntMinMax(0, SD->GetNoGrid() - 1), Math::RandIntMinMax(0, SD->GetNoGrid() - 1));
				}
				go->pos = GetGridPos(tempPt);
				go->pos.y = go->scale.y * 0.5f;
				go->GiveAnimation(new AnimationJump());
				goVil = dynamic_cast<Villager*>(go);
				for (int i = 0; i < Villager::STAT_TOTAL; ++i)
				{
					goVil->fStats[i] = Math::RandFloatMinMax(0.5f, 1.5f);
				}
				EffectManager::GetInstance()->DoPrefabEffect(EffectManager::PREFAB_PLACEOBJECT, go->pos);
			}

			int rand = Math::RandInt();
			if (rand % 2 == 0)
			{
				go = FetchGO(GameObject::GO_VILLAGER);
				
				go->scale.y = Math::RandFloatMinMax(0.5f, 0.9f);
				GridPt tempPt;
				tempPt.Set(Math::RandIntMinMax(0, SD->GetNoGrid() - 1), Math::RandIntMinMax(0, SD->GetNoGrid() - 1));
				while (!m_grid[GetGridIndex(tempPt)] == Grid::TILE_EMPTY)
				{
					tempPt.Set(Math::RandIntMinMax(0, SD->GetNoGrid() - 1), Math::RandIntMinMax(0, SD->GetNoGrid() - 1));
				}
				go->pos = GetGridPos(tempPt);
				go->pos.y = go->scale.y * 0.5f;
				go->GiveAnimation(new AnimationJump());
				goVil = dynamic_cast<Villager*>(go);
				for (int i = 0; i < Villager::STAT_TOTAL; ++i)
				{
					goVil->fStats[i] = Math::RandFloatMinMax(0.5f, 1.5f);
				}
				EffectManager::GetInstance()->DoPrefabEffect(EffectManager::PREFAB_PLACEOBJECT, go->pos);
			}
		}

		int totalGridAmts = SD->GetNoGrid() * SD->GetNoGrid();

		if (iAmountOfPigs < 4)
		{
			for (int i = 0; i < Math::RandIntMinMax(-1, 3); ++i)
			{
				GridPt tempPt;
				int trySpawnCount = 0;
				do
				{
					tempPt.Set(Math::RandIntMinMax(0, SD->GetNoGrid() - 1), Math::RandIntMinMax(0, SD->GetNoGrid() - 1));
					trySpawnCount++;
				} while (!m_grid[GetGridIndex(tempPt)] == Grid::TILE_EMPTY && trySpawnCount < totalGridAmts);
				if (trySpawnCount >= totalGridAmts)
					break;

/*				Tree* spawnedGO = dynamic_cast<Tree*>(FetchGO(GameObject::GO_TREE));
				spawnedGO->eCurrState = Tree::FULL;
				spawnedGO->fTimer = Math::RandFloatMinMax(3.5f, 6.5f);
				spawnedGO->iWoodAmount = Math::RandIntMinMax(8, 14);
				spawnedGO->pos = GetGridPos(tempPt);
				spawnedGO->pos.y = spawnedGO->scale.y * 0.5f;*/
				Pig* spawnedGO = dynamic_cast<Pig*>(FetchGO(GameObject::GO_PIG));
				spawnedGO->scale.y = 0.75f;
				spawnedGO->pos = GetGridPos(tempPt);
				spawnedGO->pos.y = spawnedGO->scale.y * 0.5f;
				spawnedGO->GiveAnimation(new AnimationJump());
				spawnedGO->iFoodAmount = Math::RandIntMinMax(1, 15);
				spawnedGO->fTimer = Math::RandFloatMinMax(0.5f, 1.5f);
				EffectManager::GetInstance()->DoPrefabEffect(EffectManager::PREFAB_PLACEOBJECT, spawnedGO->pos);
			}
		}

		if (iAmountOfTrees < 8)
		{
			for (int i = 0; i < Math::RandIntMinMax(-1, 3); ++i)
			{
				GridPt tempPt;
				int trySpawnCount = 0;
				do
				{
					tempPt.Set(Math::RandIntMinMax(0, SD->GetNoGrid() - 1), Math::RandIntMinMax(0, SD->GetNoGrid() - 1));
					trySpawnCount++;
				} while (!m_grid[GetGridIndex(tempPt)] == Grid::TILE_EMPTY && trySpawnCount < totalGridAmts);
				if (trySpawnCount >= totalGridAmts)
					break;

				Tree* spawnedGO = dynamic_cast<Tree*>(FetchGO(GameObject::GO_TREE));
				spawnedGO->eCurrState = Tree::FULL;
				spawnedGO->fTimer = Math::RandFloatMinMax(3.5f, 6.5f);
				spawnedGO->iWoodAmount = Math::RandIntMinMax(8, 14);
				spawnedGO->pos = GetGridPos(tempPt);
				spawnedGO->pos.y = spawnedGO->scale.y * 0.5f;
				EffectManager::GetInstance()->DoPrefabEffect(EffectManager::PREFAB_PLACEOBJECT, spawnedGO->pos);
			}
		}
		if (iAmountOfMountains < 8)
		{
			for (int i = 0; i < Math::RandIntMinMax(-1, 2); ++i)
			{
				GridPt tempPt;
				int trySpawnCount = 0;
				do
				{
					tempPt.Set(Math::RandIntMinMax(0, SD->GetNoGrid() - 1), Math::RandIntMinMax(0, SD->GetNoGrid() - 1));
					trySpawnCount++;
				} while (!m_grid[GetGridIndex(tempPt)] == Grid::TILE_EMPTY && trySpawnCount < totalGridAmts);
				if (trySpawnCount >= totalGridAmts)
					break;

				Mountain* spawnedGO = dynamic_cast<Mountain*>(FetchGO(GameObject::GO_MOUNTAIN));
				spawnedGO->iStoneAmount = Math::RandIntMinMax(8, 15);
				spawnedGO->iStoneGain = Math::RandIntMinMax(5, 8);
				spawnedGO->fTimer = Math::RandFloatMinMax(3.5f, 7.5f);
				spawnedGO->pos = GetGridPos(tempPt);
				spawnedGO->pos.y = spawnedGO->scale.y * 0.5f;
				EffectManager::GetInstance()->DoPrefabEffect(EffectManager::PREFAB_PLACEOBJECT, spawnedGO->pos);
			}
		}
		if (iAmountOfBushes < 4)
		{
			for (int i = 0; i < Math::RandIntMinMax(-1, 2); ++i)
			{
				GridPt tempPt;
				int trySpawnCount = 0;
				do
				{
					tempPt.Set(Math::RandIntMinMax(0, SD->GetNoGrid() - 1), Math::RandIntMinMax(0, SD->GetNoGrid() - 1));
					trySpawnCount++;
				} while (!m_grid[GetGridIndex(tempPt)] == Grid::TILE_EMPTY && trySpawnCount < totalGridAmts);
				if (trySpawnCount >= totalGridAmts)
					break;

				Bush* spawnedGO = dynamic_cast<Bush*>(FetchGO(GameObject::GO_BUSH));
				spawnedGO->eCurrState = Bush::LUSH;
				spawnedGO->fTimer = Math::RandFloatMinMax(3.5f, 6.5f);
				spawnedGO->iFoodAmount = Math::RandIntMinMax(5, 10);
				spawnedGO->pos = GetGridPos(tempPt);
				spawnedGO->pos.y = spawnedGO->scale.y * 0.5f;
				EffectManager::GetInstance()->DoPrefabEffect(EffectManager::PREFAB_PLACEOBJECT, spawnedGO->pos);
			}
		}

	}
	else // nighttime reset
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		lights[0].position.Set(0, 65, 0);
		lights[0].color.Set(0, 0, 1);
		lights[0].power = 1.f;
		lights[0].kC = 1.f;
		lights[0].kL = 0.01f;
		lights[0].kQ = 0.001f;
		lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
		lights[0].cosInner = cos(Math::DegreeToRadian(30));
		lights[0].exponent = 3.f;
		lights[0].spotDirection.Set(0.f, 1.f, 0.f);
		//bGodlights = true;
		SceneData* SD = SceneData::GetInstance();
		int totalFood = SD->GetPopulation() * 5;// 5 food is eaten per Villager
	
		//Costs extra food for tired/sick/dying/panic villagers
		for (auto go : m_goList)
		{
			if (!go->active || go->type != GameObject::GO_VILLAGER)
				continue;

			Villager* goVil = static_cast<Villager*>(go);

			if (goVil->eCurrState != Villager::HEALTHY)
			{
				//If they are not healthy
				//Exception for panic
				if (goVil->eCurrState == Villager::PANIC)
				{
					totalFood += 2;
				}
				else
				{
					int newState = Math::Max(0, (int)(goVil->eCurrState - 1));
					int extraCost = newState * 2 + 2;
					goVil->eCurrState = (Villager::STATES)(newState);

					totalFood += extraCost;
				}
				
			}
		}

		if(SD->GetFood() >= totalFood)
			SD->SetFood(Math::Max(0, SD->GetFood() - totalFood));
		else
		{
			SD->SetFood(0);
			bool someoneDied = false;
			for (auto go : m_goList)
			{
				if (!go->active || go->type != GameObject::GO_VILLAGER)
					continue;
				if (static_cast<Villager*>(go)->eCurrState == Villager::DYING)
				{
					someoneDied = true;
					go->active = false;
					EffectManager::GetInstance()->DoPrefabEffect(EffectManager::PREFAB_VILLAGER_DIE, go->pos);
				}
			}
			if (!someoneDied)
			{
				for (auto go : m_goList)
				{
					if (!go->active || go->type != GameObject::GO_VILLAGER)
						continue;
					if (!someoneDied)
					{
						go->active = false;
						someoneDied = true;
						EffectManager::GetInstance()->DoPrefabEffect(EffectManager::PREFAB_VILLAGER_DIE, go->pos);
						continue;
					}
					Villager* goVil = static_cast<Villager*>(go);
					int currState = goVil->eCurrState;
					if (goVil->eCurrState != Villager::PANIC)
					{
						goVil->eCurrState = (Villager::STATES)(currState + 1);
					}
				}
			}
		}
	}
}

void SceneSP::ProgressMonth()
{
	SceneData* SD = SceneData::GetInstance();
	SD->SetCurrMonth(SD->GetCurrMonth() + 1);
	if (SD->GetCurrMonth() >= 31)
	{
		SD->SetCurrMonth(1);
		//ChangeState(G_MAINMENU);
	}
	bGoalAchieved = false;
}

void SceneSP::Update(double dt)
{
	if (hovered != NULL)
	{
		std::cout << "There is a hovered obj" << std::endl;
	}
	if (Application::IsKeyPressed('R') && bGodMode)
	{
		Reset();
		Sleep(100);
		return;
	}

	SceneData* SD = SceneData::GetInstance();
	MousePicker* MP = MousePicker::GetInstance();
	MouseController* MC = MouseController::GetInstance();
	KeyboardController* KC = KeyboardController::GetInstance();
	UIManager* UIM = UIManager::GetInstance();
	EffectManager* EM = EffectManager::GetInstance();
	CalamityManager* CM = CalamityManager::GetInstance();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	GameObject* prevSelect = this->selected;

	SD->SetWorldHeight(m_worldHeight);
	SD->SetWorldWidth(m_worldWidth);
	SD->SetElapsedTime(SD->GetElapsedTime() + (float)dt);

	SceneBase::Update(dt);
	MP->Update(dt);

	mousePos = MP->GetIntersectionWithPlane(camera.position, Vector3(0, 0, 0), Vector3(0, 1, 0));
	if (SceneData::GetInstance()->AABBRAY)
	{
		hovered = GetHoveredObject();
	}

	SD->SetMousePos_World(mousePos);
	
	//debug stuff
	if (Application::IsKeyPressed(VK_OEM_MINUS) && bGodMode)
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed(VK_OEM_PLUS) && bGodMode)
	{
		m_speed += 0.1f;
	}
	if (KC->IsKeyPressed('G'))
	{
		bShowGrid = !bShowGrid;
	}
	if (KC->IsKeyPressed('T'))
	{
		bGodMode = !bGodMode;
	}

	// ui updates
	UIM->Update(dt);
	// goals
	if (!bGoalAchieved)
	{
		switch (SD->GetCurrMonth())
		{
		case 1:
			bGoalAchieved = SD->GetFood() >= 20;
			break;
		case 2:
			bGoalAchieved = SD->GetWood() >= 20;
			break;
		default:
			break;
		}
		if (KC->IsKeyPressed('E') && bGodMode) // press E in debug mode to set goal to done
			bGoalAchieved = true;
	}
	
	if (UIManager::GetInstance()->GetUI("ui_Text_DailyRequirement") != NULL)
	{
		for (int i = 0; i <= UIGameText::COMPONENT_TEXT_5 - UIGameText::COMPONENT_TEXT_1; ++i)
		{
			UIManager::GetInstance()->GetUI("ui_Text_DailyRequirement")->uiComponents_list[UIGameText::COMPONENT_TEXT_1 + i].text = ""; // clear ui
		}
		if (bGoalAchieved)
			UIManager::GetInstance()->GetUI("ui_Text_DailyRequirement")->uiComponents_list[UIGameText::COMPONENT_TEXT_5].text = "ACHIEVED";
		else
		{
			switch (SD->GetCurrMonth())
			{
			case 1:
				UIManager::GetInstance()->GetUI("ui_Text_DailyRequirement")->uiComponents_list[UIGameText::COMPONENT_TEXT_5].text = "Collect";
				UIManager::GetInstance()->GetUI("ui_Text_DailyRequirement")->uiComponents_list[UIGameText::COMPONENT_TEXT_4].text = "20 Food";
				break;
			case 2:
				UIManager::GetInstance()->GetUI("ui_Text_DailyRequirement")->uiComponents_list[UIGameText::COMPONENT_TEXT_5].text = "Collect";
				UIManager::GetInstance()->GetUI("ui_Text_DailyRequirement")->uiComponents_list[UIGameText::COMPONENT_TEXT_4].text = "20 Wood";
				break;
			default:
				UIManager::GetInstance()->GetUI("ui_Text_DailyRequirement")->uiComponents_list[UIGameText::COMPONENT_TEXT_5].text = "No Goal";
				break;
			}
		}
	}
	
	switch (game_state)
	{
	case G_SPLASHSCREEN:
	{
		// made with opengl
		if (fOpenGLInTimer < 1.f)
		{
			fOpenGLInTimer += dt;
			if (MC->IsButtonPressed(MouseController::LMB))//if LMB pressed, skip to logo splash
			{
				fOpenGLInTimer = 1;
				fOpenGLOutTimer = 0;
			}
		}
		else
		{
			fOpenGLInTimer = 1.f;
			if (fOpenGLOutTimer > 0.f) {
				fOpenGLOutTimer -= dt * 0.75f;
				if (MC->IsButtonPressed(MouseController::LMB))//if LMB pressed, skip to logo splash
				{
					fOpenGLOutTimer = 0;
				}
			}
			else
			{
				fOpenGLOutTimer = 0.f;
				// game icon
				if (fSplashScreenInTimer < 1.f) {
					fSplashScreenInTimer += dt;
					if (MC->IsButtonPressed(MouseController::LMB))//if LMB pressed, skip to main menu
					{
						fSplashScreenInTimer = 1;
						fGameStartTimer = 0;
					}
				}
				else
				{
					fSplashScreenInTimer = 1.f;
					/*if (fSplashScreenOutTimer > 0.f) { fSplashScreenOutTimer -= dt * 0.75f; }
					else*/
					{
						fSplashScreenOutTimer = 1.f;
						fGameStartTimer -= dt;
						if (fGameStartTimer <= 0.f || MC->IsButtonPressed(MouseController::LMB))//if timer has reached or LMB pressed, go to main menu
						{
							fGameStartTimer = 0.f;
							ChangeState(G_MAINMENU);
						}
					}
				}
			}
		}
		return;
	}
	break;
	case G_MAINMENU:
	{
		if (SD->GetMainMenuElapsedTime() < 1.f) // logo animation
		{
			if (MC->IsButtonPressed(MouseController::LMB))//if LMB pressed, skip
			{
				SD->SetMainMenuElapsedTime(1);
				fMainMenuDelta = m_worldWidth * 0.3f;
				UIM->GetUI("startbutton")->GetTween()->SetElapsedTime(0);
				UIM->GetUI("optionsbutton")->GetTween()->SetElapsedTime(-0.25f);
				UIM->GetUI("quitbutton")->GetTween()->SetElapsedTime(-0.5f);
			}
			SD->SetMainMenuElapsedTime(SD->GetMainMenuElapsedTime() + dt);
			fMainMenuDelta = Math::lerp(m_worldWidth * 0.5f, m_worldWidth * 0.3f, Math::Min(1.f, EasingStyle::easeInOutSine(SD->GetMainMenuElapsedTime(), 0, 1.f, 1.f)));
		}
		else if (UIM->GetUI("quitbutton")->GetTween() == NULL) // final (aft buttons and logo animation)
		{
			// triggers
			if (UIM->GetUI("startbutton")->IsMousePressed())
			{
				if (gameSave.LoadGame())
				{
					std::cout << "File Loaded Successfully" << std::endl;
				}
				else
				{
					std::cout << "File not Found" << std::endl;
				}
				ChangeState(G_INPLAY);
			}
			if (UIM->GetUI("optionsbutton")->IsMousePressed())
			{
				ChangeState(G_OPTIONS);
			}

			if (UIM->GetUI("quitbutton")->IsMousePressed())
			{
				Application::GetInstance().QuitGame();
			}
		}
		else // buttons animation
		{
			if (MC->IsButtonPressed(MouseController::LMB))//if LMB pressed, skip
			{
				UIM->GetUI("startbutton")->SetTweenDone();
				UIM->GetUI("optionsbutton")->SetTweenDone();
				UIM->GetUI("quitbutton")->SetTweenDone();
			}
		}
		return;
	}
	break;
	case G_OPTIONS:
	{
		// triggers
		if (UIM->GetUI("backbutton")->IsMousePressed())
		{
			ChangeState(G_MAINMENU);
		}
		if (UIM->GetUI("resetbutton")->IsMousePressed())
		{
			gameSave.ResetGame();
			Reset();
		}
		if (UIM->GetUI("objselectionbutton")->IsMousePressed())
		{
			SceneData::GetInstance()->AABBRAY = !SceneData::GetInstance()->AABBRAY;
			if (SceneData::GetInstance()->AABBRAY)
				UIM->GetUI("objselectionbutton")->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("objselectionaabb");
			else
				UIM->GetUI("objselectionbutton")->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("objselectiongrid");
		}
		return;
	}
	break;
	case G_RESEARCHTREE:
	{
		if (KC->IsKeyPressed('U'))
			ChangeState(G_INPLAY);
		// research
		if	((UIM->GetUI("WoodResearch")->IsMousePressed() || UIM->GetUI("WoodResearchCost")->IsMousePressed()) &&
			((SD->GetWood() >= 10 && !SceneData::GetInstance()->bWoodResearch) || bGodMode))
		{
			SD->SetWood(SD->GetWood() - 10);
			SceneData::GetInstance()->bWoodResearch = true;
			UIM->GetUI("WoodResearch")->uiComponents_list[UIResearchButton::COMPONENT_TICK].alpha = 1.f;
			UIM->GetUI("WoodResearchCost")->uiComponents_list[UIGameButton::COMPONENT_TEXT].text = "UNLOCKED";
			meshList[GEO_BUILDING]->textureArray[0] = LoadTGA("Image//house.tga");
		}
		else if (SceneData::GetInstance()->bWoodResearch && 
			 (UIM->GetUI("StoneResearch")->IsMousePressed() || UIM->GetUI("StoneResearchCost")->IsMousePressed()) &&
			((SD->GetWood() >= 10 && SD->GetStone() >= 10 && !SceneData::GetInstance()->bStoneResearch) || bGodMode))
		{
			SD->SetWood(SD->GetWood() - 10);
			SD->SetStone(SD->GetStone() - 10);
			SceneData::GetInstance()->bStoneResearch = true;
			UIManager::GetInstance()->GetUI("StoneResearch")->uiComponents_list[UIResearchButton::COMPONENT_TICK].alpha = 1.f;
			UIM->GetUI("StoneResearchCost")->uiComponents_list[UIGameButton::COMPONENT_TEXT].text = "UNLOCKED";
			meshList[GEO_BUILDING]->textureArray[0] = LoadTGA("Image//stonehouse.tga");
		}
		else if (SceneData::GetInstance()->bStoneResearch &&
			 (UIM->GetUI("FullStoneResearch")->IsMousePressed() || UIM->GetUI("FullStoneResearchCost")->IsMousePressed()) &&
			((SD->GetStone() >= 20 && !SceneData::GetInstance()->bFullStoneResearch) || bGodMode))
		{
			SD->SetStone(SD->GetStone() - 20);
			SceneData::GetInstance()->bFullStoneResearch = true;
			UIManager::GetInstance()->GetUI("FullStoneResearch")->uiComponents_list[UIResearchButton::COMPONENT_TICK].alpha = 1.f;
			UIM->GetUI("FullStoneResearchCost")->uiComponents_list[UIGameButton::COMPONENT_TEXT].text = "UNLOCKED";
			meshList[GEO_BUILDING]->textureArray[0] = LoadTGA("Image//fullstonehouse.tga");
		}
		return;
	}
	break;
	case G_INGAMEOPTIONS:
	{
		if (KC->IsKeyPressed('U'))
			ChangeState(G_INPLAY);
		// button pressin
		if (UIM->GetUI("backbutton")->IsMousePressed())
		{
			ChangeState(G_INPLAY);
		}
		if (UIM->GetUI("savebutton")->IsMousePressed())
		{
			gameSave.SaveGame();
			Reset();
		}
		if (UIM->GetUI("resetbutton")->IsMousePressed())
		{
			gameSave.ResetGame();
			Reset();
		}
		if (UIM->GetUI("objselectionbutton")->IsMousePressed())
		{
			SceneData::GetInstance()->AABBRAY = !SceneData::GetInstance()->AABBRAY;
			if (SceneData::GetInstance()->AABBRAY)
				UIM->GetUI("objselectionbutton")->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("objselectionaabb");
			else
				UIM->GetUI("objselectionbutton")->uiComponents_list[UIMenuButton::COMPONENT_GREYBAR].mesh = SceneData::GetInstance()->GetMesh("objselectiongrid");
		}
		return;
	}
	break;
	case G_LOSESCREEN:
	{
		if (UIM->GetUI("restartbutton")->IsMousePressed())
		{
			gameSave.ResetGame();
			Reset();
		}
		if (UIM->GetUI("loadbutton")->IsMousePressed())
		{
			if (gameSave.LoadGame())
			{
				std::cout << "File Loaded Successfully" << std::endl;
			}
			else
			{
				std::cout << "File not Found" << std::endl;
			}
			ChangeState(G_INPLAY);
		}
		if (UIM->GetUI("quitbutton")->IsMousePressed())
		{
			Application::GetInstance().QuitGame();
		}
		return;
	}
	break;
	case G_INPLAY:
	{
		UIM->GetUI("optionsbutton")->uiComponents_list[UIMenuButton::COMPONENT_OUTLINEBAR].alpha = 0.f;
		if (UIM->GetUI("optionsbutton")->IsMousePressed())
		{
			ChangeState(G_INGAMEOPTIONS);
		}
	}
	break;
	default:
	{

	}
	break;
	}

	camera.Update(dt);
	CM->Update(dt);
	EM->Update(dt);

	if (MC->IsMouseOnUI())
	{
		Application::GetInstance().SetMouseVisiblity(true);
	}
	else
	{
		Application::GetInstance().SetMouseVisiblity(false);
	}
	if (KC->IsKeyPressed('P') && bGodMode)
		CM->AddToCalamityQueue(new CalamityEarthquake());
	//SD->SetReligionValue(((int)SD->GetReligionValue() % (int)SD->GetMaxReligionValue()) + 25);
	else if (KC->IsKeyPressed('O') && bGodMode)
		CM->AddToCalamityQueue(new CalamityTsunami());
	else if (KC->IsKeyPressed('0') && bGodMode)
		CM->AddToCalamityQueue(new CalamityTornado());
	else if (KC->IsKeyPressed('9') && bGodMode)
		CM->AddToCalamityQueue(new CalamityBlizzard());
	else if (KC->IsKeyPressed('8') && bGodMode)
		CM->AddToCalamityQueue(new CalamityMeteorShower());

	if (KC->IsKeyPressed('S') && bGodMode)
		gameSave.SaveGame();
	if (KC->IsKeyPressed('L') && bGodMode)
		gameSave.LoadGame();
	if (KC->IsKeyPressed('C') && bGodMode)
		gameSave.ResetGame();

	if (KC->IsKeyPressed('U') && bGodMode)
	{
		tempCamera = camera;
		ChangeState(G_RESEARCHTREE);
	}
	if (KC->IsKeyPressed('Y') && bGodMode)
	{
		tempCamera = camera;
		ChangeState(G_INGAMEOPTIONS);
	}

	float LSPEED = 10.0f;
	float temp34 = fYPos;
	if (Application::IsKeyPressed('I') && bGodMode)
		fYPos += 0.01f;
		//lights[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K') && bGodMode)
		fYPos -= 0.01f;
		//lights[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('N'))
		lights[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('M'))
		lights[0].position.y += (float)(LSPEED * dt);

	if (fYPos != temp34)
		std::cout << fYPos << std::endl;

	/*if (Application::IsKeyPressed('Z') && bGodMode)
		lights[0].type = Light::LIGHT_POINT;
	else if (Application::IsKeyPressed('X') && bGodMode)
		lights[0].type = Light::LIGHT_DIRECTIONAL;
	else if (Application::IsKeyPressed('C') && bGodMode)
		lights[0].type = Light::LIGHT_SPOT;	*/

	if (KC->IsKeyPressed('V') && bGodMode)
	{
		gameSave.LoadEverything();
	}

	if (KC->IsKeyPressed('B') && bGodMode)
	{
		if (selected == NULL)
		{
			selected = FetchGO(GameObject::GO_HOUSE);
			GridPt currentGrid = GetPoint(mousePos);
			selected->pos = GetGridPos(currentGrid);
			selected->pos.y = selected->scale.y * 0.5f;
			static_cast<Building*>(selected)->eCurrState = Building::BLUEPRINT;

			bShowGrid = true;
		}
	}

	if (KC->IsKeyPressed(VK_CONTROL) && !m_VillagerList.empty())
	{
		if (selected == NULL || !selected->active || selected->type != GameObject::GO_VILLAGER)
		{
			iCurrItrVillagers = 0;
		}
		else
		{
			int temp = 0;
			for (auto i : m_goList)
			{
				if (i == selected)
					break;
				++temp;
			}
			iCurrItrVillagers = (temp + 1) % m_VillagerList.size();
		}
		selected = m_VillagerList[iCurrItrVillagers];
		CSoundEngine::GetInstance()->PlayASound("selection");
	}

	Vector3 clickTarget = NULL;

	static bool leftClick = false;
	if (MC->IsButtonPressed(MouseController::LMB) && !MC->IsMouseOnUI())
	{
		leftClick = true;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
		//std::cout << mousePos << std::endl;
		GridPt selectedPt = GetPoint(mousePos);
		if (SceneData::GetInstance()->AABBRAY)
		{
			if (selected == NULL)
			{
				//If initially no selected
				selected = hovered;
				if (selected != NULL)
				{
					selected->GiveAnimation(new AnimationJump());
					if (selected->type == GameObject::GO_VILLAGER)
						selected->direction = Vector3(0, 0, 1);
				}
			}
			else
			{
				//There is already a selected
				if (selected == hovered)
				{
					selected = NULL;
				}
				else
				{
					//Click somewhere else
					if (hovered != NULL)
					{
						//Clicked an object
						if (dynamic_cast<Villager*>(selected))
						{
							//If selected is a Villager
							selected->m_nextState = SMManager::GetInstance()->GetSM(selected->smID)->GetState("Idle");
							selected->goTarget = hovered;
							selected = NULL;
						}
						else
						{
							//If selected is not a Villager
							selected = hovered;
							selected->GiveAnimation(new AnimationJump());
							if (selected->type == GameObject::GO_VILLAGER)
								selected->direction = Vector3(0, 0, 1);
						}
					}
					else
					{
						// Clicked a grid point
						if (isPointInGrid(selectedPt))
						{				
							// If it is a villager
							Villager* goVil = dynamic_cast<Villager*>(selected);
							if (goVil)
							{
								selected->goTarget = NULL;

								selected->target = GetGridPos(selectedPt);
								selected->m_nextState = SMManager::GetInstance()->GetSM(selected->smID)->GetState("Idle");
							}
							else
							{
								// If it is a building
								Building* goBuilding = dynamic_cast<Building*>(selected);
								if (goBuilding)
								{
									if (goBuilding->eCurrState == Building::BLUEPRINT)
									{
										bool costPaid = false;
										if (!bGodMode)
										{
											//Cost
											if (SD->bFullStoneResearch)
											{
												if (SD->GetStone() >= 10)
												{
													SD->SetStone(SD->GetStone() - 10);
													costPaid = true;
												}
											}
											else if (SD->bStoneResearch)
											{
												if (SD->GetStone() >= 5 && SD->GetWood() >= 5)
												{
													SD->SetStone(SD->GetStone() - 5);
													SD->SetWood(SD->GetWood() - 5);
													costPaid = true;
												}
											}
											else if (SD->bWoodResearch)
											{
												if (SD->GetWood() >= 10)
												{
													SD->SetWood(SD->GetWood() - 10);
													costPaid = true;
												}
											}
											else //Straw
											{
												if (SD->GetWood() >= 5)
												{
													SD->SetWood(SD->GetWood() - 5);
													costPaid = true;
												}
											}
										}
										else
										{
											costPaid = true;
										}

										if (costPaid)
										{
											//Should be trying to contruct now
											if (!goBuilding->bBuilt)
												goBuilding->eCurrState = Building::CONSTRUCTING;
											else
												goBuilding->eCurrState = Building::COMPLETED;
											bShowGrid = false;
											EM->DoPrefabEffect(EffectManager::PREFAB_PLACEOBJECT, selected->pos);
											CSoundEngine::GetInstance()->PlayASound("place");
										}
										else
										{
											//Not enouf resources
											goBuilding->active = false;
											selected = NULL;
											//Maybe some ui for no resources
										}
									}
								}
							}
							selected = NULL;
						}
					}
				}
			}
		}
		else
		{
			if (isPointInGrid(selectedPt))
				//std::cout << "Selected Grid: " << selectedPt.x << ", " << selectedPt.z << std::endl;
			{
				bool objectFound = false;
				for (auto go : m_goList)
				{
					if (!go->active)
						continue;
					if (selectedPt == go->currentPt)
					{
						//Supposed to implement priority here
						if (selected == NULL)
						{
							selected = go;
							objectFound = true;
							go->GiveAnimation(new AnimationJump());
							if (go->type == GameObject::GO_VILLAGER)
								go->direction = Vector3(0, 0, 1);
							break;
						}
						else if (selected != go)
						{
							objectFound = true;
							if (dynamic_cast<Villager*>(selected))
							{
								selected->m_nextState = SMManager::GetInstance()->GetSM(selected->smID)->GetState("Idle");
								selected->goTarget = go;
								selected = NULL;
								objectFound = true;
							}
							else
							{
								selected = go;
								go->GiveAnimation(new AnimationJump());
								if (go->type == GameObject::GO_VILLAGER)
									go->direction = Vector3(0, 0, 1);
								break;
							}
						}
					}
					if (objectFound)
						break;
				}
				if (!objectFound)
				{
					if (selected != NULL)
					{
						//If it is a villager
						Villager* goVil = dynamic_cast<Villager*>(selected);
						if (goVil)
						{
							selected->goTarget = NULL;

							selected->target = GetGridPos(selectedPt);
							selected->m_nextState = SMManager::GetInstance()->GetSM(selected->smID)->GetState("Idle");
						}
						//If it is a building
						Building* goBuilding = dynamic_cast<Building*>(selected);
						if (goBuilding)
						{
							if (goBuilding->eCurrState == Building::BLUEPRINT)
							{
								//Should be trying to contruct now
								if (!goBuilding->bBuilt)
									goBuilding->eCurrState = Building::CONSTRUCTING;
								else
									goBuilding->eCurrState = Building::COMPLETED;
								bShowGrid = false;
								EM->DoPrefabEffect(EffectManager::PREFAB_PLACEOBJECT, selected->pos);
								CSoundEngine::GetInstance()->PlayASound("place");
							}

						}
						selected = NULL;
					}
				}
			}
		}
	}

	static float clickTimer = 0.f;
	if (leftClick)
	{
		if (selected != NULL && selected->type == GameObject::GO_VILLAGER)
		{
			clickTimer += dt;
			if (clickTimer > 0.3f)
			{
				selected->m_nextState = SMManager::GetInstance()->GetSM(selected->smID)->GetState("PickedUp");
				selected->pickupPt = selected->currentPt;
				clickTimer = 0.f;
				hand->SetState(EffectHand::HAND_GRAB_OBJECT);
				CSoundEngine::GetInstance()->PlayASound("gasp");
				leftClick = false;
			}
		}
		else
		{
			leftClick = false;
		}
	}

	if (MC->IsButtonReleased(MouseController::LMB))
	{
		leftClick = false;
		clickTimer = 0.f;
		if (selected != NULL && selected->type == GameObject::GO_VILLAGER)
		{
			if (selected->m_currState == SMManager::GetInstance()->GetSM(selected->smID)->GetState("PickedUp"))
			{
				GridPt selectedPt = GetPoint(mousePos);
				selected->m_nextState = SMManager::GetInstance()->GetSM(selected->smID)->GetState("Idle");
				hand->SetState(EffectHand::HAND_DEFAULT);
				if (isPointInGrid(selectedPt))
				{
					if (m_grid[GetGridIndex(selectedPt)] == Grid::TILE_USED)
					{
						bool targetFound = false;
						for (auto go : m_goList)
						{
							if (go != selected && go->currentPt == selectedPt)
							{
								selected->goTarget = go;
								targetFound = true;
								break;
							}
						}
						if (!targetFound)
						{
							//In grid but some barrier
							//selected->m_nextState = SMManager::GetInstance()->GetSM(selected->smID)->GetState("Idle");
						}
					}
					selected->currentPt = selectedPt;
					selected->pos = GetGridPos(selected->currentPt);
					selected->pos.y += selected->scale.y * 0.5f;
				}
				else
				{
					//Outside of grid
					//goes back to previous pos
					selected->currentPt = selected->pickupPt;
					selected->pos = GetGridPos(selected->currentPt);
					selected->pos.y += selected->scale.y * 0.5f;
				}
				EM->DoPrefabEffect(EffectManager::PREFAB_PLACEVILLAGER, selected->pos + Vector3(0, -selected->scale.y * 0.5f, 0));
				selected = NULL;
			}
		}
	}

	// day/night cycle
	fTimeOfDay += dt * m_speed * 0.35f;
	if (fTimeOfDay >= 24.f)
		fTimeOfDay = 0;
	else if (fTimeOfDay >= 6.f && fTimeOfDay <= 18.f && !bDay) // 0600 to 1800 day
		ChangeTimeOfDay();
	else if ((fTimeOfDay <= 6.f || fTimeOfDay >= 18.f) && bDay) // 1800 to 0600 night
		ChangeTimeOfDay();
	SD->SetTimeOfDay(fTimeOfDay);
	float SHADOW_LENGTH = 1.f;
	if (bDay)
	{
		lights[0].position.x = (12.f - fTimeOfDay) * SHADOW_LENGTH;
		lights[0].position.y = (-0.0025f * pow(lights[0].position.x, 2)) + 9;
	}
	// month
	if (SD->GetCurrDay() >= 31)
	{
		SD->SetCurrDay(1);
		ProgressMonth();
	}


	// sea movement
	if (fSeaDeltaX >= SEA_WIDTH / 4)
		fSeaDeltaX = -SEA_WIDTH / 4;
	fSeaDeltaX += dt * m_speed;
	if (fSeaDeltaZ >= SEA_HEIGHT / 4)
		fSeaDeltaZ = -SEA_HEIGHT / 4;
	fSeaDeltaZ += dt * m_speed;

	fSeaDeltaY = 0.0625f + 0.0625f * cosf(SD->GetElapsedTime());

	if (bWorldEnd)
		fWaterLevel += dt * 0.01f;

	ProjectileManager::GetInstance()->Update(dt * m_speed);

	//There is a currently selected object
	if (selected != NULL)
	{
		if (!selected->active)
			selected = NULL;
		if (selected != NULL)
		{
			Building* goBuilding = dynamic_cast<Building*>(selected);
			if (goBuilding) //selected is of a building class
			{
				if (goBuilding->eCurrState == Building::BLUEPRINT) //Building editor mode
				{
					GridPt currentGrid = GetPoint(mousePos);
					selected->pos = GetGridPos(currentGrid);
					selected->pos.y += selected->scale.y * 0.5f;
				}
				else if (goBuilding->eCurrState != Building::CONSTRUCTING)//The building is either completed / half broken
				{
					//Pop up some ui or something, maybe somewhere else
				}
			}
		}
	}

	//Update the Grid
	std::fill(m_grid.begin(), m_grid.end(), Grid::TILE_EMPTY);

	GridPt selectedGrid = GetPoint(mousePos);
	if (isPointInGrid(selectedGrid))
	{
		Vector3 gridPos = GetGridPos(selectedGrid);
		m_grid[GetGridIndex(selectedGrid)] = Grid::TILE_SELECTED;
	}
	for (auto go : m_goList)
	{
		if (!go->active)
			continue;
		GridPt gridPt = go->currentPt;
		if (isPointInGrid(gridPt))
		{
			Vector3 ptPos = GetGridPos(gridPt); //Position of gridPt in world space

			//Check if this GameObject takes up a GridPt
			if (go->iGridX > 0 && go->iGridZ > 0)
			{
				//Since this value is rounded down, this value is for the closer side of GridPt, refer to gridImage in beside solution file
				int iHalfGridX = go->iGridX * 0.5f;
				int iHalfGridZ = go->iGridZ * 0.5f;

				//Check which side
				bool bLeftSide = false;
				if (go->pos.x < ptPos.x)
				{
					bLeftSide = true;
				}
				//Check which side
				bool bUpSide = false;
				if (go->pos.z < ptPos.z)
				{
					bUpSide = true;
				}

				//x-axis
				for (int i = 1; i <= iHalfGridX; ++i)
				{
					for (int j = 1; j <= iHalfGridZ; ++j)
					{
						if (bLeftSide)
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
						else
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
					}
					for (int j = 0; j < (go->iGridZ - iHalfGridZ); ++j)
					{
						if (bLeftSide)
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
						else
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
					}
				}
				for (int i = 0; i < (go->iGridX - iHalfGridX); ++i)
				{
					for (int j = 1; j <= iHalfGridZ; ++j)
					{
						if (bLeftSide)
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
						else
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
					}
					for (int j = 0; j < (go->iGridZ - iHalfGridZ); ++j)
					{
						if (bLeftSide)
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
						else
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] != Grid::TILE_USED)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//Update the GameObjects
	static const float NPC_VELOCITY = 10.f;
	SD->SetPopulation(0);
	SD->SetPopulationLimit(0);
	SD->SetFoodLimit(0);
	SD->SetWoodLimit(0);
	SD->SetStoneLimit(0);
	m_VillagerList.clear();
	for (auto go : m_goList)
	{
		if (!go->active)
			continue;
		// updating resources limits and tier
		Building* goB = dynamic_cast<Building*>(go);
		if (goB)
		{
			if (SceneData::GetInstance()->bFullStoneResearch)
				goB->eCurrTier = Building::FULL_STONE;
			else if (SceneData::GetInstance()->bStoneResearch)
				goB->eCurrTier = Building::STONE;
			else if (SceneData::GetInstance()->bWoodResearch)
				goB->eCurrTier = Building::WOOD;
			else
				goB->eCurrTier = Building::STRAW;
		}
		switch (go->type)
		{
		case GameObject::GO_VILLAGER:		
		{
			Villager* goVil = static_cast<Villager*>(go);
			SD->SetPopulation(SD->GetPopulation() + 1);
			m_VillagerList.push_back(go);

			switch (goVil->eCurrState)
			{
			case Villager::HEALTHY:
				goVil->fEfficiency = 1.2f;
				break;
			case Villager::PANIC:
				goVil->fEfficiency = 1.0f;
				break;
			case Villager::TIRED:
				goVil->fEfficiency = 0.8f;
				break;
			case Villager::SICKLY:
				goVil->fEfficiency = 0.6f;
				break;
			case Villager::DYING:
				goVil->fEfficiency = 0.4f;
				break;
			}
		}
			break;
		case GameObject::GO_CHIEFHUT:
		{	
			ChiefHut* goHouse = static_cast<ChiefHut*>(go);
			SD->SetFoodLimit(SD->GetFoodLimit() + 10);
			SD->SetWoodLimit(SD->GetWoodLimit() + 10);
			SD->SetStoneLimit(SD->GetStoneLimit() + 10);
			if (goHouse->eCurrState == Building::COMPLETED)
			{
				SD->SetPopulationLimit(SD->GetPopulationLimit() + goHouse->iHousingSpace);
			}
			else if (goHouse->eCurrState == Building::BROKEN)
			{
				SD->SetPopulationLimit(SD->GetPopulationLimit() + goHouse->iHousingSpace * 0.5f);
			}
			goChiefHut = go;
		}
			break;
		case GameObject::GO_HOUSE:
		{	
			House* goHouse = static_cast<House*>(go);
			SD->SetFoodLimit(SD->GetFoodLimit() + 10);
			if (goHouse->eCurrState == Building::COMPLETED)
			{
				SD->SetPopulationLimit(SD->GetPopulationLimit() + goHouse->iHousingSpace);
			}
			else if (goHouse->eCurrState == Building::BROKEN)
			{
				SD->SetPopulationLimit(SD->GetPopulationLimit() + goHouse->iHousingSpace * 0.5f);
			}
		}
			break;
		case GameObject::GO_BUSH:
			break;
		case GameObject::GO_GRANARY:
		{
			float capacityMultiplier = 0;
			if (static_cast<Building*>(go)->eCurrState == Building::COMPLETED)
				capacityMultiplier = 1;
			else if (static_cast<Building*>(go)->eCurrState == Building::BROKEN)
				capacityMultiplier = 0.5f;
			SD->SetFoodLimit(SD->GetFoodLimit() + static_cast<Granary*>(go)->foodCapacity * capacityMultiplier);
		}
		break;
		case GameObject::GO_WOODSHED:
		{
			float capacityMultiplier = 0;
			if (static_cast<Building*>(go)->eCurrState == Building::COMPLETED)
				capacityMultiplier = 1;
			else if (static_cast<Building*>(go)->eCurrState == Building::BROKEN)
				capacityMultiplier = 0.5f;
			SD->SetWoodLimit(SD->GetWoodLimit() + static_cast<WoodShed*>(go)->woodCapacity * capacityMultiplier);
		}
		break;
		case GameObject::GO_STONESHED:
		{
			float capacityMultiplier = 0;
			if (static_cast<Building*>(go)->eCurrState == Building::COMPLETED)
				capacityMultiplier = 1;
			else if (static_cast<Building*>(go)->eCurrState == Building::BROKEN)
				capacityMultiplier = 0.5f;
			SD->SetStoneLimit(SD->GetStoneLimit() + static_cast<StoneShed*>(go)->stoneCapacity * capacityMultiplier);
		}
		break;
		case GameObject::GO_MOUNTAIN:
		{
			Mountain* mountainGo = static_cast<Mountain*>(go);
			if (mountainGo->iStoneAmount > 20)
			{
				mountainGo->scale.y = 2;
				mountainGo->pos.y = mountainGo->scale.y * 0.5f;
			}
			else if (mountainGo->iStoneAmount >= 10)
			{
				mountainGo->scale.y = 1.5f;
				mountainGo->pos.y = mountainGo->scale.y * 0.5f;
			}
			else
			{
				mountainGo->scale.y = 1.f;
				mountainGo->pos.y = mountainGo->scale.y * 0.5f;
			}
		}
		break;
		case GameObject::GO_ALTAR:
		{
			Altar* goAltar = static_cast<Altar*>(go);
			this->goAltar = go;
			static const float MAX_FOOD_TIMER = 3.f; //Rate for food to be decreased
			static float fFoodtimer = MAX_FOOD_TIMER;
			if (goAltar->iFoodOffered > 0)
			{
				if (fFoodtimer <= 0.f)
				{
					goAltar->iFoodOffered = Math::Max(0, goAltar->iFoodOffered - 1);
					fFoodtimer = MAX_FOOD_TIMER;
				}
				else
				{
					fFoodtimer -= (float)dt;
				}
			}
			SD->SetReligionValue(Math::Min(100.f, (float)(static_cast<Altar*>(go)->iFoodOffered)));
		}
		break;
		case GameObject::GO_RESEARCHLAB:
			this->goResearchLab = go;
			break;
		case GameObject::GO_TSUNAMI:
		{
			Tsunami* goTsunami = static_cast<Tsunami*>(go);
			goTsunami->fParticleTimer_Cloud += (float)dt * m_speed;
			if (goTsunami->fParticleTimer_Cloud > 0.1f)
			{
				goTsunami->fParticleTimer_Cloud = 0;
				EffectCloud* cloud = new EffectCloud(
					goTsunami->pos
					, Math::RandFloatMinMax(0.5f, 1.2f)
					, Vector3(1, 1, 1) * Math::RandFloatMinMax(1.6f, 2.4f)
				);
				cloud->vel *= 0.7;
				cloud->acc *= 0.35;
				EM->AddEffect(cloud);
			}
			if (goTsunami->tsunami_direction == Tsunami::DIRECTION_LEFT)
			{
				goTsunami->pos += Vector3(-SD->GetGridSize() * goTsunami->moveSpeed, 0, 0) * dt * m_speed;
				if (goTsunami->pos.x < GetGridPos(GridPt(-10, 0)).x)
				{
					goTsunami->active = false;
				}
			}
			//Check collision
			for (auto go2 : m_goList)
			{
				if (!go2->active || go2 == go)
					continue;
				if ((go2->pos - goTsunami->pos).LengthSquared() <= goTsunami->scale.x * goTsunami->scale.x)
				{
					goTsunami->Collided(go2);
				}
			}
		}
		break;
		case GameObject::GO_TORNADO:
		{
			Tornado* goTornado = static_cast<Tornado*>(go);
			float timeIncrement = (float)dt*m_speed;
			goTornado->fElapsedTime += timeIncrement;
			goTornado->fEffectTimer_Cloud += timeIncrement;
			goTornado->fEffectTimer_Dirt += timeIncrement;
			if (goTornado->fEffectTimer_Cloud > 0.06f)
			{
				goTornado->fEffectTimer_Cloud = 0;
				EffectCloud* cloud = new EffectCloud(
					goTornado->pos
					, Math::RandFloatMinMax(0.2f, 0.4f)
					, Vector3(1, 1, 1) * Math::RandFloatMinMax(1.6f, 2.4f)
				);
				cloud->vel *= 0.9;
				cloud->acc *= 0.35;
				EM->AddEffect(cloud);
				for (int j = 0; j < 2; ++j) 
				{
					EffectRing* ring = new EffectRing(
						goTornado->pos + Vector3(0, Math::RandFloatMinMax(-0.15f,0.5f), 0)
						, Math::RandFloatMinMax(0.4f, 0.475f)
						, Vector3(1, 1, 1) * 0.04f
						, Vector3(1, 1, 1) * Math::RandFloatMinMax(0.1f, 1.75f)
						, NULL
					);
					ring->startAlpha = ring->endAlpha = 1.f;
					ring->rotation.Set(Math::RandFloatMinMax(-20, 20), 0, Math::RandFloatMinMax(-20, 20));
					ring->vel.Set(0, Math::RandFloatMinMax(5, 10), 0);
					ring->acc = -ring->vel * 1.f;
					ring->bLightEnabled = true;
					EffectManager::GetInstance()->AddEffect(ring);
				}
			}
			if (goTornado->fEffectTimer_Dirt > 0.12f)
			{
				goTornado->fEffectTimer_Dirt = 0;
				EffectDirt* dirt = new EffectDirt(
					goTornado->pos + Vector3(0, -1, 0)
					, Math::RandFloatMinMax(0.8f, 1.2f)
					, Vector3(1, 1, 1) * Math::RandFloatMinMax(0.5f, 1.5f)
				);
				dirt->vel *= 5;
				dirt->acc *= 5;
				EM->AddEffect(dirt);
			}
			goTornado->pos += Vector3(-SD->GetGridSize() * goTornado->moveSpeed, 0, 0) * dt * m_speed;
			goTornado->pos.z = cosf(goTornado->fElapsedTime) * (SD->GetNoGrid() * SD->GetGridSize() * 0.5f);
			if (goTornado->pos.x < GetGridPos(GridPt(-10, 0)).x)
			{
				goTornado->active = false;
			}
			//Check collision
			for (auto go2 : m_goList)
			{
				if (!go2->active || go2 == go)
					continue;
				if ((go2->pos - goTornado->pos).LengthSquared() <= goTornado->scale.x * goTornado->scale.x)
				{
					goTornado->Collided(go2);
				}
			}
		}
		break;
		case GameObject::GO_METEOR:
		{
			Meteor* goMeteor = static_cast<Meteor*>(go);
			float timeIncrement = (float)dt*m_speed;
			goMeteor->fEffectTimer_Fire += timeIncrement;
			if (goMeteor->fEffectTimer_Fire > 0.09f)
			{
				goMeteor->fEffectTimer_Fire = 0;
				EM->DoPrefabEffect(EffectManager::PREFAB_METEOR_FIRE, goMeteor->pos);
			}
			goMeteor->pos += goMeteor->vel * dt * m_speed;
			if (goMeteor->pos.y <= 0)
			{
				EM->DoPrefabEffect(EffectManager::PREFAB_METEOR_IMPACT, goMeteor->pos);
				PostOffice::GetInstance()->Send("Scene",
					new MessageCameraShake(MessageCameraShake::SHAKE_METEOR, Math::RandFloatMinMax(0.5f, 1.25f), Math::RandFloatMinMax(0.3f, 0.35f))
				);
				goMeteor->active = false;
			}
			//Check collision
			for (auto go2 : m_goList)
			{
				if (!go2->active || go2 == go)
					continue;
				if ((go2->pos - goMeteor->pos).LengthSquared() <= goMeteor->scale.x * goMeteor->scale.x)
				{
					goMeteor->Collided(go2);
				}
			}
		}
		break;
		default:
			break;
		}

		/*if (go->type == GameObject::GO_NPC)
		{
			if (go->target == NULL)
			{
			}
			if (go->target != NULL)
			{
				if ((go->pos - go->target).LengthSquared() < 1.f)
				{
					go->pos = go->target;
				}
				else
				{
					try
					{
						go->pos += (go->target - go->pos).Normalized() * NPC_VELOCITY * m_speed * static_cast<float>(dt);
					}
					catch (DivideByZero)
					{
						//Do nothing
					}
				}
			}
		}
		else if (go->type == GameObject::GO_CREEP_MELEE || go->type == GameObject::GO_CREEP_RANGE)
		{
			if (go->smID != "")
			{
				SMManager::GetInstance()->GetSM(go->smID)->Update(dt * m_speed, go);
			}
		}*/
		//go->pos += go->vel * dt * m_speed;
	}
	for (auto go : m_goList)
	{
		if (!go->active)
			continue;
		go->currentPt = GetPoint(go->pos);
		go->Update(dt * m_speed);
		if (go->smID != "")
		{
			SMManager::GetInstance()->GetSM(go->smID)->Update(dt * m_speed, go);
		}
	}
	SD->SetFood(Math::Min(SD->GetFood(), SD->GetFoodLimit()));
	SD->SetWood(Math::Min(SD->GetWood(), SD->GetWoodLimit()));
	SD->SetStone(Math::Min(SD->GetStone(), SD->GetStoneLimit()));
	//lose game
	if (SD->GetPopulation() <= 0 || goAltar == NULL || goChiefHut == NULL || !goAltar->active || !goChiefHut->active)
		ChangeState(G_LOSESCREEN);
	if (prevSelect != selected)
	{
		UpdateSelectedUI();
	}
}

void SceneSP::RenderGO(GameObject *go)
{
	modelStack.PushMatrix();
	//modelStack.MultMatrix(go->animation.GetCurrentTransformation());
	switch (go->type)
	{
	case GameObject::GO_VILLAGER:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		if (go->animation != NULL)
		{
			float angle = Math::RadianToDegree(atan2(-go->direction.z, go->direction.x));
			if (go->animation != NULL)
			{
				Mtx44 temp;
				temp.SetToIdentity();
				temp.SetToRotation((angle), 0, 1, 0);
				//go->animation->Rotate = go->animation->Rotate * temp;
				go->animation->DirectionRotate.SetToRotation((angle), 0, 1, 0);
			}
			go->animation->MultiplyMtx();
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		}
		else
		{
			float angle = Math::RadianToDegree(atan2(-go->direction.z, go->direction.x));
			modelStack.Rotate(angle, 0, 1, 0);
		}

		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		Villager* goVil = static_cast<Villager*>(go);
		if(goVil->mEquipment != NULL)
			RenderMesh(goVil->mEquipment, bGodlights, 1.f);
		RenderMesh(meshList[GEO_VILLAGER], bGodlights, 1.f);
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_PIG:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		if (go->animation != NULL)
		{
			float angle = Math::RadianToDegree(atan2(-go->direction.z, go->direction.x));
			if (go->animation != NULL)
			{
				Mtx44 temp;
				temp.SetToIdentity();
				temp.SetToRotation((angle), 0, 1, 0);
				//go->animation->Rotate = go->animation->Rotate * temp;
				go->animation->DirectionRotate.SetToRotation((angle), 0, 1, 0);
			}
			go->animation->MultiplyMtx();
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		}
		else
		{
			float angle = Math::RadianToDegree(atan2(-go->direction.z, go->direction.x));
			modelStack.Rotate(angle, 0, 1, 0);
		}

		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PIG], bGodlights, 1.f);
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_BUILDING:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		if (go->animation != NULL)
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//Current state of the building
		switch (static_cast<Building*>(go)->eCurrState)
		{
		case Building::COMPLETED:
			RenderMesh(meshList[GEO_BUILDING], bGodlights, 1.f);
			break;
		case Building::BLUEPRINT:
			RenderMesh(meshList[GEO_BUILDING], false, 0.2f);
			break;
		case Building::CONSTRUCTING:
			RenderMesh(meshList[GEO_BUILDING], false, 0.6f);
			break;
		case Building::BROKEN:
			RenderMesh(meshList[GEO_BROKEN_BUILDING], bGodlights, 1.f);
		}
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_HOUSE:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		if (go->animation != NULL)
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//Current state of the building
		switch (static_cast<Building*>(go)->eCurrState)
		{
		case Building::COMPLETED:
			RenderMesh(meshList[GEO_BUILDING], bGodlights, 1.f);
			break;
		case Building::BLUEPRINT:
			RenderMesh(meshList[GEO_BUILDING], false, 0.2f);
			break;
		case Building::CONSTRUCTING:
			RenderMesh(meshList[GEO_BUILDING], false, 0.6f);
			break;
		case Building::BROKEN:
			RenderMesh(meshList[GEO_BROKEN_BUILDING], bGodlights, 1.f);
		}
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_GRANARY:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		if (go->animation != NULL)
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//Current state of the building
		switch (static_cast<Building*>(go)->eCurrState)
		{
		case Building::COMPLETED:
			RenderMesh(meshList[GEO_GRANARY], bGodlights, 1.f);
			break;
		case Building::BLUEPRINT:
			RenderMesh(meshList[GEO_GRANARY], false, 0.2f);
			break;
		case Building::CONSTRUCTING:
			RenderMesh(meshList[GEO_GRANARY], false, 0.6f);
			break;
		case Building::BROKEN:
			RenderMesh(meshList[GEO_BROKEN_GRANARY], bGodlights, 1.f);
		}
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_WOODSHED:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		if (go->animation != NULL)
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//Current state of the building
		switch (static_cast<Building*>(go)->eCurrState)
		{
		case Building::COMPLETED:
			RenderMesh(meshList[GEO_WOODSHED], bGodlights, 1.f);
			break;
		case Building::BLUEPRINT:
			RenderMesh(meshList[GEO_WOODSHED], false, 0.2f);
			break;
		case Building::CONSTRUCTING:
			RenderMesh(meshList[GEO_WOODSHED], false, 0.6f);
			break;
		case Building::BROKEN:
			RenderMesh(meshList[GEO_BROKEN_WOODSHED], bGodlights, 1.f);
		}
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_STONESHED:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		if (go->animation != NULL)
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//Current state of the building
		switch (static_cast<Building*>(go)->eCurrState)
		{
		case Building::COMPLETED:
			RenderMesh(meshList[GEO_STONESHED], bGodlights, 1.f);
			break;
		case Building::BLUEPRINT:
			RenderMesh(meshList[GEO_STONESHED], false, 0.2f);
			break;
		case Building::CONSTRUCTING:
			RenderMesh(meshList[GEO_STONESHED], false, 0.6f);
			break;
		case Building::BROKEN:
			RenderMesh(meshList[GEO_BROKEN_STONESHED], bGodlights, 1.f);
		}
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_CHIEFHUT:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		if (go->animation != NULL)
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//Current state of the building
		switch (static_cast<Building*>(go)->eCurrState)
		{
		case Building::COMPLETED:
			RenderMesh(meshList[GEO_BUILDING], bGodlights, 1.f);
			break;
		case Building::BLUEPRINT:
			RenderMesh(meshList[GEO_BUILDING], false, 0.2f);
			break;
		case Building::CONSTRUCTING:
			std::cout << "Chief Hut being contructed??" << std::endl;
			break;
		case Building::BROKEN:
			RenderMesh(meshList[GEO_BROKEN_BUILDING], bGodlights, 1.f);
		}
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_ALTAR:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		if (go->animation != NULL)
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//Current state of the building
		switch (static_cast<Building*>(go)->eCurrState)
		{
		case Building::COMPLETED:
			RenderMesh(meshList[GEO_ALTAR], bGodlights, 1.f);
			break;
		case Building::BLUEPRINT:
			RenderMesh(meshList[GEO_ALTAR], false, 0.2f);
			break;
		case Building::CONSTRUCTING:
			std::cout << "Altar being contructed??" << std::endl;
			break;
		case Building::BROKEN:
			RenderMesh(meshList[GEO_BROKEN_BUILDING], bGodlights, 1.f);
		}
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_RESEARCHLAB:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		if (go->animation != NULL)
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//Current state of the building
		switch (static_cast<Building*>(go)->eCurrState)
		{
		case Building::COMPLETED:
			RenderMesh(meshList[GEO_ALTAR], bGodlights, 1.f);
			break;
		case Building::BLUEPRINT:
			RenderMesh(meshList[GEO_ALTAR], false, 0.2f);
			break;
		case Building::CONSTRUCTING:
			std::cout << "ResearchLab being contructed??" << std::endl;
			break;
		case Building::BROKEN:
			RenderMesh(meshList[GEO_BROKEN_BUILDING], bGodlights, 1.f);
		}
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_MOUNTAIN:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		if (go->animation != NULL)
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_MOUNTAIN], bGodlights, 1.f);
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_LOGS:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		if (go->animation != NULL)
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//Current state of the building
		switch (static_cast<Building*>(go)->eCurrState)
		{
		case Building::COMPLETED:
			RenderMesh(meshList[GEO_LOGS], bGodlights, 1.f);
			break;
		case Building::BLUEPRINT:
			RenderMesh(meshList[GEO_LOGS], false, 0.2f);
			break;
		case Building::CONSTRUCTING:
			RenderMesh(meshList[GEO_LOGS], false, 0.6f);
			break;
		case Building::BROKEN:
			RenderMesh(meshList[GEO_BROKEN_BUILDING], bGodlights, 1.f);
		}
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_BUSH:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		if (go->animation != NULL)
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BUSH], bGodlights, 1.f);
		if (static_cast<Bush*>(go)->eCurrState == Bush::LUSH)
			RenderMesh(meshList[GEO_BERRIES], bGodlights, 1.f);
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_TREE:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//Offset to center to middle of used grids
		if (go->iGridX % 2 == 0)
		{
			modelStack.Translate(SceneData::GetInstance()->GetGridSize() * 0.5f, 0, 0);
		}
		if (go->iGridZ % 2 == 0)
		{
			modelStack.Translate(0, 0, SceneData::GetInstance()->GetGridSize() * 0.5f);
		}
		if (go->animation != NULL)
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		Tree* goTree = static_cast<Tree*>(go);
		switch (goTree->eCurrState)
		{
		case Tree::FULL:
			RenderMesh(meshList[GEO_TREE], bGodlights, 1.f);
			break;
		case Tree::HALFCHOPPED:
			RenderMesh(meshList[GEO_HALFTREE], bGodlights, 1.f);
			break;
		default:
			RenderMesh(meshList[GEO_TREE], bGodlights, 1.f);
			break;
		}
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_TSUNAMI:
	{
		float alpha;
		Tsunami* tsunami = dynamic_cast<Tsunami*>(go);
		if (tsunami->tsunami_direction == Tsunami::DIRECTION_LEFT)
		{
			Vector3 start = GetGridPos(GridPt(SceneData::GetInstance()->GetNoGrid() + 8, 0));
			Vector3 end = GetGridPos(GridPt(-10, 0));
			alpha = (1 - (-(end.x - go->pos.x) / (start.x * 2))) * 3.14f;
		}
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y + 4.f * sinf(alpha) - 2.f, go->pos.z);
		if (go->animation != NULL)
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_TSUNAMI], bGodlights, 1.f);
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_METEOR:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		if (go->animation != NULL)
			modelStack.MultMatrix(go->animation->GetCurrentTransformation());
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_EFFECT_DIRT], bGodlights, 1.f);
		modelStack.PopMatrix();
	}
	break;
	default:
		break;
	}
	modelStack.PopMatrix();
}

void SceneSP::RenderPassGPass()
{
	m_renderPass = RENDER_PASS_PRE;
	m_lightDepthFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_gPassShaderID);
	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
		m_lightDepthProj.SetToOrtho(-64, 64, -64, 64, -64, 64);
	else
		m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 1000);
	m_lightDepthView.SetToLookAt(lights[0].position.x,
		lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);
	RenderWorld();
}

void SceneSP::RenderPassMain()
{
	m_renderPass = RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetWindowWidth(),
		Application::GetWindowHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);
	//pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	//projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -1000, 1000);
	float aspect = (float)Application::GetInstance().GetWindowWidth() / (float)Application::GetInstance().GetWindowHeight();
	projection.SetToPerspective(70.f, aspect, 0.01f, 2000);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	//RenderMesh(meshList[GEO_BALL], false);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(mousePos.x, mousePos.y, mousePos.z);
	////modelStack.Scale(0.1, 0.1, 0.1);
	//RenderMesh(meshList[GEO_HAND_DEFAULT], true);
	//modelStack.PopMatrix();

	//RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	//modelStack.Translate(0, 0.5f + cosf(asd) * 0.15f, 0);
	modelStack.Translate(0, fYPos - 1.f, 0);
	//modelStack.Translate(0, 0, 0);
	//modelStack.Translate(0, -0.5f, 0);
	//modelStack.Rotate(-90, 1, 0, 0);
	//modelStack.Scale(15, 15, 15);
	modelStack.Scale(5, 1, 5);
	RenderMesh(meshList[GEO_ISLAND], bGodlights);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(fSeaDeltaX, fSeaDeltaY + (fYPos - 1.f) + fWaterLevel, fSeaDeltaZ);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(SEA_WIDTH, SEA_HEIGHT, SEA_HEIGHT);
	RenderMesh(meshList[GEO_SEA], false, 0.75f);
	modelStack.PopMatrix();

	RenderWorld();

	modelStack.PushMatrix();
	modelStack.Translate(0, 2, 0);
	modelStack.Scale(5, 5, 5);
	//RenderMesh(meshList[GEO_LIGHT_DEPTH_QUAD], false);
	modelStack.PopMatrix();

	SceneData* SD = SceneData::GetInstance();

	//Render Grid
	if (bShowGrid)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-0.5f * SD->GetNoGrid() * SD->GetGridSize(), fYPos, -0.5f * SD->GetNoGrid() * SD->GetGridSize());
		modelStack.Scale(1, 1, 1);
		glLineWidth(2.f);
		RenderMesh(meshList[GEO_GRID], false);
		glLineWidth(1.f);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-0.5f * SD->GetNoGrid() * SD->GetGridSize(), fYPos, -0.5f * SD->GetNoGrid() * SD->GetGridSize());
		for (int i = 0; i < SD->GetNoGrid() * SD->GetNoGrid(); ++i)
		{
			std::pair<int, int> pt = GetPoint(i);
			modelStack.PushMatrix();
			modelStack.Translate(pt.first * SD->GetGridSize() + SD->GetGridOffset(), 0.f, pt.second * SD->GetGridSize() + SD->GetGridOffset());
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(SD->GetGridSize(), SD->GetGridSize(), SD->GetGridSize());
			switch (m_grid[i])
			{
			case Grid::TILE_EMPTY:
				RenderMesh(meshList[GEO_GREENQUAD], false, 0.3f);
				break;
			case Grid::TILE_USED:
				RenderMesh(meshList[GEO_REDQUAD], false, 0.3f);
				break;
			case Grid::TILE_SELECTED:
				RenderMesh(meshList[GEO_YELLOWQUAD], false, 0.3f);
				break;
			}
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();

	}

	//On screen text
	std::ostringstream ss;

	ss.str("");
	ss.precision(3);
	ss << "Speed:" << m_speed;
	if (bGodMode)
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 67, 6);

	ss.str("");
	ss.precision(5);
	ss << "FPS:" << fps;
	if (bGodMode)
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 67, 3);

	ss.str("");
	if (bGodMode)
		ss << "[T] Player Mode";
	else
		ss << "[T] Debug Mode";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 67, 0);

	UIManager::GetInstance()->Render(this);
}

void SceneSP::RenderMenuState()
{
	m_renderPass = RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetWindowWidth(),
		Application::GetWindowHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);
	//pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	//RenderMesh(meshList[GEO_AXES], false);

	switch (game_state)
	{
	case G_SPLASHSCREEN:
	{
		if (fOpenGLOutTimer > 0.f)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 1.f);
			modelStack.Scale(m_worldWidth, ((m_worldWidth / 1024) * 720), m_worldHeight);
			if (fOpenGLInTimer < 1.0f)
				RenderMesh(meshList[GEO_SPLASHSCREEN], false, fOpenGLInTimer);
			else
				RenderMesh(meshList[GEO_SPLASHSCREEN], false, fOpenGLOutTimer);
			modelStack.PopMatrix();
		}
		else
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 1.f);
			modelStack.Scale(m_worldWidth, ((m_worldWidth / 1024) * 720), m_worldHeight);
			if (fSplashScreenInTimer < 1.0f)
				RenderMesh(meshList[GEO_LOGO], false, fSplashScreenInTimer);
			else
				RenderMesh(meshList[GEO_LOGO], false, fSplashScreenOutTimer);
			modelStack.PopMatrix();
		}
	}
	break;
	case G_MAINMENU:
	{
		modelStack.PushMatrix();
		modelStack.Translate(Math::Clamp(fMainMenuDelta, m_worldWidth * 0.3f, m_worldWidth * 0.5f), m_worldHeight * 0.5f, 0.f);
		modelStack.Scale(m_worldWidth, ((m_worldWidth / 1024) * 720), m_worldHeight);
		RenderMesh(meshList[GEO_LOGO], false);
		modelStack.PopMatrix();

		//if(fMainMenuDelta <= m_worldWidth * 0.3f)
		if (SceneData::GetInstance()->GetMainMenuElapsedTime() > 1.f)
			UIManager::GetInstance()->Render(this);
	}
	break;
	case G_OPTIONS:
	{
		UIManager::GetInstance()->Render(this);
	}
	break;
	case G_LOSESCREEN:
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 1.f);
		modelStack.Scale(m_worldWidth, ((m_worldWidth / 1024) * 720), m_worldHeight);
		RenderMesh(meshList[GEO_LOSESCREEN], false);
		modelStack.PopMatrix();

		//if (SceneData::GetInstance()->GetMainMenuElapsedTime() > 1.f)
			UIManager::GetInstance()->Render(this);
	}
	break;
	default:
	{

	}
	break;
	}

}

void SceneSP::RenderWorld()
{
	EffectManager::GetInstance()->Render(this);
	for (auto go : m_goList)
	{
		if (!go->active)
			continue;
		RenderGO(go);
	}
}

GameObject * SceneSP::GetHoveredObject()
{
	//Currently using the closer object, might change to closer intersection point
	GameObject* currGo = NULL;
	float lengthSquared = FLT_MAX;
	for (auto go : m_goList)
	{
		if (!go->active)
			continue;
		Vector3 hitPos;
		Vector3 goPos = go->pos;

		Vector3 minAABB = goPos + go->minAABB;
		Vector3 maxAABB = goPos + go->maxAABB;

		if (GetIntersectionAABB(camera.position, mousePos, minAABB, maxAABB, hitPos) == true)
		{
			float distanceSquared = (go->pos - camera.position).LengthSquared();
			if (currGo == NULL)
			{
				currGo = go;
				lengthSquared = distanceSquared;
			}
			else if(lengthSquared > distanceSquared)
			{
				currGo = go;
				lengthSquared = distanceSquared;
			}
		}
	}
	if (currGo == NULL)
	{
		SceneData::GetInstance()->aabbHitPoint.Set(0, -50, 0);
	}
	return currGo;
}

bool SceneSP::GetIntersectionAABB(Vector3 lineStart, Vector3 lineEnd, Vector3 minAABB, Vector3 maxAABB, Vector3 & hitPosition)
{
	if ((GetIsIntersecting(lineStart.x - minAABB.x, lineEnd.x - minAABB.x, lineStart, lineEnd, hitPosition) &&
		InBox(hitPosition, minAABB, maxAABB, 1))
		|| (GetIsIntersecting(lineStart.y - minAABB.y, lineEnd.y - minAABB.y, lineStart, lineEnd, hitPosition) &&
			InBox(hitPosition, minAABB, maxAABB, 2))
		|| (GetIsIntersecting(lineStart.z - minAABB.z, lineEnd.z - minAABB.z, lineStart, lineEnd, hitPosition) &&
			InBox(hitPosition, minAABB, maxAABB, 3))
		|| (GetIsIntersecting(lineStart.x - maxAABB.x, lineEnd.x - maxAABB.x, lineStart, lineEnd, hitPosition) &&
			InBox(hitPosition, minAABB, maxAABB, 1))
		|| (GetIsIntersecting(lineStart.y - maxAABB.y, lineEnd.y - maxAABB.y, lineStart, lineEnd, hitPosition) &&
			InBox(hitPosition, minAABB, maxAABB, 2))
		|| (GetIsIntersecting(lineStart.z - maxAABB.z, lineEnd.z - maxAABB.z, lineStart, lineEnd, hitPosition) &&
			InBox(hitPosition, minAABB, maxAABB, 3)))
	{
		SceneData::GetInstance()->aabbHitPoint = hitPosition;
		return true;
	}
	return false;
}

bool SceneSP::GetIsIntersecting(const float fDst1, const float fDst2, Vector3 lineStart, Vector3 lineEnd, Vector3 & hitPosition)
{
	if ((fDst1*fDst2) >= 0.0f)
	{
		return false;
	}
	else if (fDst1 == fDst2)
	{
		return false;
	}
	//Intersection pos = startPos + dir * ((length of startPos - collisionPt) / (endPos - startPos)) //Note that fDst1 is a negative value
	//Intersection pos = startPos + Entire Line * ratio of startPos to intersection / entire line
	hitPosition = lineStart + (lineEnd - lineStart) * (-fDst1 / (fDst2 - fDst1));
	return true;
}

bool SceneSP::InBox(Vector3 hitPosition, Vector3 minAABB, Vector3 maxAABB, const int Axis)
{
	if (Axis == 1 && hitPosition.z > minAABB.z && hitPosition.z < maxAABB.z && hitPosition.y > minAABB.y && hitPosition.y < maxAABB.y) return true;
	if (Axis == 2 && hitPosition.z > minAABB.z && hitPosition.z < maxAABB.z && hitPosition.x > minAABB.x && hitPosition.x < maxAABB.x) return true;
	if (Axis == 3 && hitPosition.x > minAABB.x && hitPosition.x < maxAABB.x && hitPosition.y > minAABB.y && hitPosition.y < maxAABB.y) return true;
	return false;
}

void SceneSP::Render()
{
	//SceneBase::Render();
	switch (game_state)
	{
	case G_SPLASHSCREEN:
	case G_MAINMENU:
	case G_OPTIONS:
	case G_LOSESCREEN:
		RenderMenuState();
		break;
	case G_INPLAY:
	case G_RESEARCHTREE:
	case G_INGAMEOPTIONS:
		RenderPassGPass();
		RenderPassMain();
		break;
	default:
		break;
	}
}

void SceneSP::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}