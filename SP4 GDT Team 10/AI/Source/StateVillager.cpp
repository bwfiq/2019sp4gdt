#include "StateVillager.h"

#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"
#include "SMManager.h"
#include "ProjectileManager.h"
#include <iostream>

#include "Villager.h"
#include "Building.h"
#include "Bush.h"
#include "Tree.h"
#include "Mountain.h"

#include "AnimationWalk.h"
#include "AnimationPickUp.h"
#include "AnimationChopping.h"

#include "MousePicker.h"
#include "MouseController.h"
//State::State(const std::string & stateID)
//	: m_stateID(stateID)
//{
//}
//
//State::~State()
//{
//}
//
//const std::string & State::GetStateID()
//{
//	return m_stateID;
//}

static float RUN_SPEED = 3.f;
static float WALK_SPEED = 1.5f;

//StateIdle
StateIdle::StateIdle(const std::string & stateID)
	: State(stateID)
{
}

StateIdle::~StateIdle()
{
}


void StateIdle::Enter(GameObject* m_go)
{
	std::cout << "Enter Idle State" << std::endl;

	std::cout << "Clearing existing m_go path" << std::endl;

	while (!m_go->path.empty())
	{
		m_go->path.pop_back();
	}
}

void StateIdle::Update(double dt, GameObject* m_go)
{
	Villager* goVillager = static_cast<Villager*>(m_go);

	if (m_go->goTarget != NULL || m_go->target != NULL)
	{
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Path");
		if (m_go->goTarget != NULL && m_go->active)
		{
			MessageWRU* messagewru = new MessageWRU(m_go, MessageWRU::PATH_TO_TARGET, 1);
			PostOffice::GetInstance()->Send("Scene", messagewru);
		}
		else if (m_go->target != NULL)
		{
			MessageWRU* messagewru = new MessageWRU(m_go, MessageWRU::PATH_TO_POINT, 1);
			PostOffice::GetInstance()->Send("Scene", messagewru);
		}
		return;
	}
	goVillager->fIdleTimer -= dt;
	if (goVillager->fIdleTimer <= 0.f)
	{
		goVillager->fIdleTimer = Math::RandFloatMinMax(3.f, 10.f);
		MessageWRU* messagewru = new MessageWRU(m_go, MessageWRU::RANDOM_TARGET, 2 * 2 + 2 * 2);
		PostOffice::GetInstance()->Send("Scene", messagewru);
	}
}

void StateIdle::Exit(GameObject* m_go)
{
}

//StatePath
StatePath::StatePath(const std::string & stateID)
	: State(stateID)
{
}

StatePath::~StatePath()
{
}

void StatePath::Enter(GameObject * m_go)
{
	std::cout << "Enter Path State" << std::endl;
	m_go->GiveAnimation(new AnimationWalk());
}

void StatePath::Update(double dt, GameObject * m_go)
{
	SceneData* SD = SceneData::GetInstance();
	if (m_go->target != NULL || m_go->goTarget != NULL)
	{
		if (m_go->animation == NULL)
		{
			m_go->GiveAnimation(new AnimationWalk());
		}
		if (m_go->target != NULL)
		{
			if (!m_go->path.empty())
			{
				Vector3 ptPos = GetGridPos(m_go->path.back()); //Position of gridPt in world space
				Vector3 direction = ptPos - m_go->pos;
				direction.y = 0;
				if (direction.LengthSquared() < 0.05f)
				{
					m_go->pos += direction;
					m_go->path.pop_back();
				}
				else
				{
					m_go->pos += (direction).Normalized() * dt * WALK_SPEED;

					//float currentAngle = acos(m_go->direction.Normalized().Dot(Vector3(1, 0, 0)));
					//Vector3 godirNormalized = m_go->direction.Normalized();
					//float currentAngletheSequel = atan2f(godirNormalized.x, godirNormalized.z);
					//std::cout << Math::RadianToDegree(currentAngletheSequel) - 90 << std::endl;
					//Vector3 dirNormalized = direction.Normalized();
					//std::cout << Math::RadianToDegree(atan2f(dirNormalized.x, dirNormalized.z)) << std::endl;
					//float deltaAngleSequel = Math::lerp(currentAngletheSequel, atan2f(dirNormalized.x, dirNormalized.z),Math::Min(1.f,(float)dt*10.5f));
					////std::cout << Math::RadianToDegree(deltaAngleSequel) << std::endl;
					////float alphaAngleSequel = currentAngletheSequel + deltaAngleSequel * Math::Min(1.f, (float)dt*10.5f);
					//m_go->direction.x = sinf(deltaAngleSequel);
					//m_go->direction.z = cosf(deltaAngleSequel);

					float currentAngle = acos(m_go->direction.Normalized().Dot(Vector3(1, 0, 0)));

					if ((m_go->direction).Cross(Vector3(1, 0, 0)).y > 0)
					{
						currentAngle = -currentAngle;
					}
					Vector3 goDirNormalised = m_go->direction.Normalized();
					Vector3 dirNormalised = direction.Normalized();
					float dotProduct = goDirNormalised.Dot(dirNormalised);
					float deltaAngle = acos(Math::Clamp(dotProduct, -0.999999f, 0.999999f));
					if (isnan(deltaAngle))
					{
						std::cout << "Infinity" << std::endl;
					}
					if (m_go->direction.Cross(direction).y < 0)
					{
						deltaAngle = -deltaAngle;
					}
					float alpha = 10.5f;
					float alphaAngle = Math::lerp(currentAngle, currentAngle + deltaAngle, Math::Min(1.f, (float)dt * alpha));/*currentAngle + deltaAngle * Math::Min(1.f, (float)dt * alpha);*/
					m_go->direction.x = cosf(alphaAngle);
					m_go->direction.z = -sinf(alphaAngle);
				}
			}
			else
			{
				m_go->target = NULL;
				m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
				return;
			}
		}
		else if (m_go->goTarget != NULL)
		{
			if (!m_go->goTarget->active)
			{
				m_go->goTarget = NULL;
				m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
			}
			//Movement
			if (!m_go->path.empty())
			{
				Vector3 ptPos = GetGridPos(m_go->path.back()); //Position of gridPt in world space
				Vector3 direction = ptPos - m_go->pos;
				direction.y = 0;
				if (direction.LengthSquared() < 0.05f)
				{
					m_go->pos += direction;
					m_go->path.pop_back();
				}
				else
				{
					m_go->pos += (direction).Normalized() * dt * RUN_SPEED;

					float currentAngle = acos(m_go->direction.Normalized().Dot(Vector3(1, 0, 0)));

					if ((m_go->direction).Cross(Vector3(1, 0, 0)).y > 0)
					{
						currentAngle = -currentAngle;
					}
					Vector3 goDirNormalised = m_go->direction.Normalized();
					Vector3 dirNormalised = direction.Normalized();
					float dotProduct = goDirNormalised.Dot(dirNormalised);
					float deltaAngle = acos(Math::Clamp(dotProduct, -0.999999f, 0.999999f));
					if (m_go->direction.Cross(direction).y < 0)
					{
						deltaAngle = -deltaAngle;
					}
					float alpha = 10.5f;
					float alphaAngle = Math::lerp(currentAngle, currentAngle + deltaAngle, Math::Min(1.f, (float)dt * alpha));/*currentAngle + deltaAngle * Math::Min(1.f, (float)dt * alpha);*/
					if (alphaAngle == INFINITY)
					{
						std::cout << "Infinity" << std::endl;
					}
					m_go->direction.x = cosf(alphaAngle);
					m_go->direction.z = -sinf(alphaAngle);
				}
			}
			else //Reached Destination
			{
				Villager* goVillager = dynamic_cast<Villager*>(m_go->goTarget);
				if (goVillager)
				{
					//If its another villager
					m_go->goTarget = NULL;
					m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
					return;
				}
				Building* goBuilding = dynamic_cast<Building*>(m_go->goTarget);
				if (goBuilding)
				{
					//If its a building class
					switch (goBuilding->eCurrState)
					{
					case Building::COMPLETED:
					{
						switch (goBuilding->type)
						{
						case GameObject::GO_BUILDING:
							m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("InHut");
							break;
						case GameObject::GO_CHIEFHUT:
							m_go->goTarget = NULL;
							m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
							break;
						case GameObject::GO_GRANARY:
							//m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("InHut");
							SD->SetFood(SD->GetFood() + static_cast<Villager*>(m_go)->iFoodStored);

							static_cast<Villager*>(m_go)->iFoodStored = 0;

							m_go->goTarget = NULL;
							m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
							break;
						case GameObject::GO_WOODSHED:
							//m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("InHut");
							SD->SetWood(Math::Min(SD->GetWoodLimit(), SD->GetWood() + static_cast<Villager*>(m_go)->iWoodStored));

							static_cast<Villager*>(m_go)->iWoodStored = 0;

							m_go->goTarget = NULL;
							m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
							break;
						case GameObject::GO_HOUSE:
							m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("InHut");
							break;
						}
					}
					break;
					case Building::CONSTRUCTING:
					{
						m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Constructing");
					}
					break;
					case Building::BROKEN:
					{
						m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Constructing");
					}
					break;
					}
					return;
				}
				Environment* goEnvironment = dynamic_cast<Environment*>(m_go->goTarget);
				if (goEnvironment)
				{
					switch (goEnvironment->type)
					{
					case GameObject::GO_BUSH:
						m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Foraging");
						break;
					case GameObject::GO_TREE:
						m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("ChopTree");
						break;
					case GameObject::GO_MOUNTAIN:
						m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Mining");
						break;
					}
					return;
				}
				/* //For Enemies
				case GameObject::GO_ENEMY:
					m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Attack");
					break;
				*/
				m_go->goTarget = NULL;
				m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
				return;
			}
		}
	}
	else
	{
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
	}
}

void StatePath::Exit(GameObject * m_go)
{
	if (m_go->animation != NULL)
	{
		if (m_go->animation->type == AnimationBase::A_WALK)
		{
			delete m_go->animation;
			m_go->animation = NULL;
		}
	}
}

//StateChopTree
StateChopTree::StateChopTree(const std::string & stateID)
	: State(stateID)
{
}

StateChopTree::~StateChopTree()
{
}

void StateChopTree::Enter(GameObject * m_go)
{
	std::cout << "Enter Chop Tree State" << std::endl;
	Villager* goVil = static_cast<Villager*>(m_go);
	goVil->fActionTimer = static_cast<Environment*>(m_go->goTarget)->fTimer;
	m_go->scale *= 0.2f;
	m_go->pos.x += m_go->goTarget->scale.x * 0.3f;
	m_go->pos.y = m_go->scale.y * 0.5f;
	m_go->pos.z += m_go->goTarget->scale.z * 0.3f;
	m_go->direction.Set(-1, 0, -1);
	m_go->direction.Normalize();

	SceneData* SD = SceneData::GetInstance();
	goVil->mEquipment = SD->GetMesh("hatchet");
	m_go->GiveAnimation(new AnimationChopping());
}

void StateChopTree::Update(double dt, GameObject * m_go)
{
	//In StateChopTree, the goTarget must be a Tree class
	if (m_go->goTarget->type != GameObject::GO_TREE)
	{
		std::cout << "Wrong State : ChopTree" << std::endl;
		return;
	}
	if (!m_go->goTarget->active)
	{
		m_go->goTarget = NULL;
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
	}
	Tree* treeGo = static_cast<Tree*>(m_go->goTarget);
	Villager* vGo = static_cast<Villager*>(m_go);
	if (vGo->fActionTimer <= 0.f)
	{
		if (treeGo->eCurrState == Tree::FULL)
		{
			vGo->iWoodStored = Math::Min(vGo->fStats[Villager::WOODCUTTING] * treeGo->iWoodAmount + vGo->iWoodStored, (float)vGo->iMaxWoodStored);
			treeGo->eCurrState = Tree::HALFCHOPPED;
		}
		else if (treeGo->eCurrState == Tree::HALFCHOPPED)
		{
			vGo->iWoodStored = Math::Min(vGo->fStats[Villager::WOODCUTTING] * treeGo->iWoodAmount + vGo->iWoodStored, (float)vGo->iMaxWoodStored);
			treeGo->active = false;
		}
		MessageWRU* messagewru = new MessageWRU(m_go, MessageWRU::FIND_NEAREST_WOODSHED, 1);
		PostOffice::GetInstance()->Send("Scene", messagewru);
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
	}
	else
	{
		vGo->fActionTimer -= dt;
	}
	return;
}

void StateChopTree::Exit(GameObject * m_go)
{
	static_cast<Villager*>(m_go)->fActionTimer = 0;
	m_go->scale *= 5.f;
	m_go->pos.y = m_go->scale.y * 0.5f;
	Villager* goVil = static_cast<Villager*>(m_go);
	goVil->mEquipment = NULL;

	m_go->ClearAnimation();
}

//StateForaging
StateForaging::StateForaging(const std::string & stateID)
	: State(stateID)
{
}

StateForaging::~StateForaging()
{
}

void StateForaging::Enter(GameObject * m_go)
{
	std::cout << "Enter Foraging State" << std::endl;
	static_cast<Villager*>(m_go)->fActionTimer = static_cast<Environment*>(m_go->goTarget)->fTimer;
}

void StateForaging::Update(double dt, GameObject * m_go)
{

	if (m_go->goTarget == NULL ||!m_go->goTarget->active)
	{
		m_go->goTarget = NULL;
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
		return;
	}
	//In StateForaging, the goTarget must be a Bush class
	if (m_go->goTarget->type != GameObject::GO_BUSH)
	{
		std::cout << "Wrong State : Foraging" << std::endl;
		return;
	}

	Bush* bushGo = static_cast<Bush*>(m_go->goTarget);
	Villager* vGo = static_cast<Villager*>(m_go);
	if (bushGo->eCurrState == Bush::LUSH)
	{
		if (vGo->fActionTimer <= 0.f)
		{
			//Insert gathering time here
			vGo->iFoodStored = Math::Min(vGo->fStats[Villager::FORAGING] * bushGo->iFoodAmount + vGo->iFoodStored, (float)vGo->iMaxFoodStored);
			bushGo->eCurrState = Bush::DEPLETED;
			m_go->goTarget = NULL;

			MessageWRU* messagewru = new MessageWRU(m_go, MessageWRU::FIND_NEAREST_GRANARY, 1);
			PostOffice::GetInstance()->Send("Scene", messagewru);
			m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
			return;
		}
		else
		{
			vGo->fActionTimer -= dt;
		}
	}
	else
	{
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
	}
}

void StateForaging::Exit(GameObject * m_go)
{
	static_cast<Villager*>(m_go)->fActionTimer = 0;
}

//StateAttack
StateAttack::StateAttack(const std::string & stateID)
	: State(stateID)
{
}

StateAttack::~StateAttack()
{
}

void StateAttack::Enter(GameObject* m_go)
{
	std::cout << "Enter Attack State" << std::endl;
}

void StateAttack::Update(double dt, GameObject* m_go)
{
}

void StateAttack::Exit(GameObject* m_go)
{
}

//StatePickedUp
StatePickedUp::StatePickedUp(const std::string & stateID)
	: State(stateID)
{
}

StatePickedUp::~StatePickedUp()
{
}

void StatePickedUp::Enter(GameObject* m_go)
{
	std::cout << "Enter PickedUp State" << std::endl;

	std::cout << "Clearing existing m_go path" << std::endl;

	while (!m_go->path.empty())
	{
		m_go->path.pop_back();
	}

	m_go->GiveAnimation(new AnimationPickUp());
}

void StatePickedUp::Update(double dt, GameObject* m_go)
{
	SceneData* SD = SceneData::GetInstance();
	MouseController* MC = MouseController::GetInstance();
	Vector3 dir = SD->GetCamPosition() - SD->GetMousePos_World();
	try
	{
		dir.Normalize();
	}
	catch (DivideByZero)
	{
		dir.SetZero();
	}
	m_go->pos = SD->GetMousePos_World() + dir * (-MC->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) + 4) * 0.4f;
	//m_go->pos = SD->GetMousePos_World() + Vector3(0, (-MC->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) + 4) * 0.5f, 0);
}

void StatePickedUp::Exit(GameObject* m_go)
{
	if (m_go->animation != NULL)
	{
		delete m_go->animation;
		m_go->animation = NULL;
	}
}

//StateDead
StateDead::StateDead(const std::string & stateID)
	: State(stateID)
{
}

StateDead::~StateDead()
{
}

void StateDead::Enter(GameObject* m_go)
{
	std::cout << "Enter Dead State" << std::endl;
}

void StateDead::Update(double dt, GameObject* m_go)
{
}

void StateDead::Exit(GameObject* m_go)
{
}

//StateInHut
StateInHut::StateInHut(const std::string & stateID)
	: State(stateID)
{
}

StateInHut::~StateInHut()
{
}


void StateInHut::Enter(GameObject* m_go)
{
	std::cout << "Enter InHut State" << std::endl;
}

void StateInHut::Update(double dt, GameObject* m_go)
{
	//In StateInHut, the goTarget must be a Building class
	Building* goBuilding = dynamic_cast<Building*>(m_go->goTarget);
	Villager* vGo = static_cast<Villager*>(m_go);
	if (goBuilding)
	{
		//IMPORTANT!! MUST BE CHANGED, TEMPORARY CODE FOR CHANING BUILDING'S STATE
		if (!goBuilding->active)
		{
			m_go->goTarget = NULL;
			m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
			return;
		}
		m_go->goTarget = NULL;
		goBuilding->eCurrState = Building::BROKEN;
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
	}
	else
	{
		std::cout << "Wrong State : InHut" << std::endl;
		return;
	}
}

void StateInHut::Exit(GameObject* m_go)
{
}

//StateConstructing
StateConstructing::StateConstructing(const std::string & stateID)
	: State(stateID)
{
}

StateConstructing::~StateConstructing()
{
}


void StateConstructing::Enter(GameObject* m_go)
{
	std::cout << "Enter Constructing State" << std::endl;
}

void StateConstructing::Update(double dt, GameObject* m_go)
{
	//In StateConstructing, the goTarget must be a Building class
	Building* goBuilding = dynamic_cast<Building*>(m_go->goTarget);
	Villager* vGo = static_cast<Villager*>(m_go);
	if (goBuilding)
	{
		if (!goBuilding->active)
		{
			m_go->goTarget = NULL;
			m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
			return;
		}
		//Construction and Repair code here, maybe some timer idk
		m_go->goTarget = NULL;
		goBuilding->eCurrState = Building::COMPLETED;
		goBuilding->bBuilt = true;
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
	}
	else
	{
		std::cout << "Wrong State : Constructing" << std::endl;
		return;
	}
}

void StateConstructing::Exit(GameObject* m_go)
{
}

//StateMining
StateMining::StateMining(const std::string & stateID)
	: State(stateID)
{
}

StateMining::~StateMining()
{
}


void StateMining::Enter(GameObject* m_go)
{
	std::cout << "Enter Mining State" << std::endl;
	static_cast<Villager*>(m_go)->fActionTimer = static_cast<Environment*>(m_go->goTarget)->fTimer;
}

void StateMining::Update(double dt, GameObject* m_go)
{
	//In StateMining, the goTarget must be a Mountain class
	if (m_go->goTarget->type != GameObject::GO_MOUNTAIN)
	{
		std::cout << "Wrong State : Mining" << std::endl;
		return;
	}
	if (!m_go->goTarget->active)
	{
		m_go->goTarget = NULL;
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
	}
	Mountain* mountainGo = static_cast<Mountain*>(m_go->goTarget);
	Villager* vGo = static_cast<Villager*>(m_go);

	//Insert gathering time here
	if (vGo->fActionTimer <= 0.f)
	{
		vGo->iStoneStored = Math::Min(vGo->fStats[Villager::MINING] * mountainGo->iStoneGain, (float)vGo->iMaxStoneStored);
		mountainGo->iStoneAmount -= mountainGo->iStoneGain;

		if (mountainGo->iStoneAmount <= 0)
		{
			mountainGo->active = false;
		}

		MessageWRU* messagewru = new MessageWRU(m_go, MessageWRU::FIND_CHIEFHUT, 1);
		PostOffice::GetInstance()->Send("Scene", messagewru);
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
		return;
	}
	else
	{
		vGo->fActionTimer -= dt;
	}
}

void StateMining::Exit(GameObject* m_go)
{
	static_cast<Villager*>(m_go)->fActionTimer = 0;
}