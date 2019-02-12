#include "SceneSP.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "SceneData.h"

#include "SceneData.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "ProjectileManager.h"

#include "SMManager.h"
#include "MouseController.h"

#include "Villager.h"
#include "Building.h"
#include "ChiefHut.h"

#define START_PLAYER false
#define SEA_WIDTH	50.f
#define SEA_HEIGHT	50.f
SceneSP::SceneSP()
{
}

SceneSP::~SceneSP()
{
}

void SceneSP::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	SceneData::GetInstance()->SetWorldHeight(m_worldHeight);
	SceneData::GetInstance()->SetWorldWidth(m_worldWidth);
	SceneData::GetInstance()->SetElapsedTime(0);
	PostOffice::GetInstance()->Register("Scene", this);

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	goVillager = FetchGO(GameObject::GO_VILLAGER);
	goVillager->pos.Set(0, goVillager->scale.y * 0.5f, 0);

	goChiefHut = FetchGO(GameObject::GO_CHIEFHUT);
	goChiefHut->scale.y = 1.5f;
	goChiefHut->pos.Set(1.5f, goChiefHut->scale.y * 0.5f, 0);


	//go->vel.Set(1, 0, 0);
	MousePicker::GetInstance()->Init();
	MousePicker::GetInstance()->SetProjectionStack(projectionStack);
	MousePicker::GetInstance()->SetViewStack(viewStack);
}


bool SceneSP::Handle(Message* message)
{

	MessageWRU* messageWRU = dynamic_cast<MessageWRU*>(message);
	if (messageWRU)
	{
		switch (messageWRU->type)
		{
		default:
			break;
		}
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
				go->scale.Set(1.f, 1.f, 1.f);
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
		case GameObject::GO_BUILDING:
			go = new Building(type);
			break;
		case GameObject::GO_CHIEFHUT:
			go = new ChiefHut(type);
			break;
		default:
			go = new GameObject(type);
			break;
		}
		if(go != nullptr)
			m_goList.push_back(go);
	}
	return FetchGO(type);
}

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

void SceneSP::Reset()
{	
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}

	m_speed = 1.f;

	//game_state = NEUTRAL;
}

void SceneSP::Update(double dt)
{
	if (Application::IsKeyPressed('R'))
	{
		Reset();
		Sleep(100);
		return;
	}
	SceneData* SD = SceneData::GetInstance();
	MousePicker* MP = MousePicker::GetInstance();
	MouseController* MC = MouseController::GetInstance();

	SceneBase::Update(dt);
	MP->Update(dt);
	camera.Update(dt);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	

	SD->SetWorldHeight(m_worldHeight);
	SD->SetWorldWidth(m_worldWidth);
	SD->SetElapsedTime(SD->GetElapsedTime() + (float)dt);

	if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		m_speed += 0.1f;
	}
	if (Application::IsKeyPressed('N'))
	{
	}
	if (Application::IsKeyPressed('M'))
	{
	}
	if (Application::IsKeyPressed(VK_RETURN))
	{
	}
	if (MC->IsButtonPressed(MouseController::LMB))
		std::cout << "asd" << std::endl;

	//Input Section
	static bool bPState = false;
	if (!bPState && Application::IsKeyPressed('P'))
	{
		bPState = true;
	}
	else if(bPState && !Application::IsKeyPressed('P'))
	{
		bPState = false;
		std::cout << "P UP" << std::endl;
	}
	static bool bFState = false;
	if (!bFState && Application::IsKeyPressed('F'))
	{
		bFState = true;

		//static_cast<Villager*>(goVillager)->DoFunction();
		goVillager->goTarget = goChiefHut;
	}
	else if (bFState && !Application::IsKeyPressed('F'))
	{
		bFState = false;
		std::cout << "F UP" << std::endl;
	}

	/*
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
		if (posX < m_worldHeight && posY < m_worldHeight)
		{
		//	m_graph.CreateNode(Vector3(posX, posY));
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	*/
	Vector3 clickTarget = NULL;
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
		if (posX < m_worldHeight && posY < m_worldHeight)
		{
			clickTarget.Set(posX, posY, 0);
		}

	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}
	static bool bSpaceState = false;
	if (!bSpaceState && Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = true;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
		if (posX < m_worldHeight && posY < m_worldHeight)
		{
			//GameObject* go = FetchGO(GameObject::GO_NPC);
			//go->pos.Set(posX, posY);
		}
	}
	else if (bSpaceState && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
	}

	// sea movement
	/*if (Application::IsKeyPressed('I'))
		fSeaDeltaZ += 0.5f * dt;
	if (Application::IsKeyPressed('K'))
		fSeaDeltaZ -= 0.5f * dt;
	if (Application::IsKeyPressed('J'))
		fSeaDeltaX += 0.5f * dt;
	if (Application::IsKeyPressed('L'))
		fSeaDeltaX -= 0.5f * dt;


	fSeaDeltaX += (5.f - fSeaDeltaX) *0.1f * dt;*/
	if (fSeaDeltaX >= SEA_WIDTH / 4)
		fSeaDeltaX = -SEA_WIDTH / 4;
	fSeaDeltaX += dt;
	if (fSeaDeltaZ >= SEA_HEIGHT / 4)
		fSeaDeltaZ = -SEA_HEIGHT / 4;
	fSeaDeltaZ += dt;

	fSeaDeltaY = 0.25f + 0.25f * cosf(SD->GetElapsedTime());

	ProjectileManager::GetInstance()->Update(dt * m_speed);
	static const float NPC_VELOCITY = 10.f;
	for (auto go : m_goList)
	{
		if (!go->active)
			continue;
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
		if (go->smID != "")
		{
			SMManager::GetInstance()->GetSM(go->smID)->Update(dt * m_speed, go);
		}
	}
}


void SceneSP::RenderGO(GameObject *go)
{
	static float healthBarScale = 0.7f;
	switch (go->type)
	{
	case GameObject::GO_VILLAGER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//RenderMesh(meshList[GEO_VILLAGER], false, 1.f);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BUILDING:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BUILDING], false, 1.f);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_CHIEFHUT:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BUILDING], false, 1.f);
		modelStack.PopMatrix();
		break;
	default:
		break;
	}
}

void SceneSP::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	modelStack.PushMatrix();
	Vector3 adsada = MousePicker::GetInstance()->GetIntersectionWithPlane(camera.position, Vector3(0, 0.6f, 0), Vector3(0, 1, 0));
	modelStack.Translate(adsada.x, adsada.y, adsada.z);
	modelStack.Scale(0.025, 0.025, 0.025);
	RenderMesh(meshList[GEO_VILLAGER], false);
	modelStack.PopMatrix();

	RenderMesh(meshList[GEO_AXES], false);

	static float asd = 0;
	asd += 0.01;
	modelStack.PushMatrix();
	//modelStack.Translate(0, 0.5f + cosf(asd) * 0.15f, 0);
	modelStack.Translate(0, -0.5f, 0);
	//modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(10, 1, 10);
	RenderMesh(meshList[GEO_GRASS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0.001f, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_GRID], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(fSeaDeltaX, fSeaDeltaY - 0.51f, fSeaDeltaZ);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(SEA_WIDTH, 50, 50);
	RenderMesh(meshList[GEO_SEA], false);
	modelStack.PopMatrix();

	//On screen text
	std::ostringstream ss;

	for (auto go : m_goList)
	{
		if (!go->active)
			continue;
		RenderGO(go);
	}

	ss.str("");
	ss.precision(3);
	ss << "Speed:" << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 6);

	ss.str("");
	ss.precision(5);
	ss << "FPS:" << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 3);

	ss.str("");
	ss << "Graph " << 0;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 0);
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
