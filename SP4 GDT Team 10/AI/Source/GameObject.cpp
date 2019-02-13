
#include "GameObject.h"
#include "ConcreteMessages.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	moveSpeed(1.f),
	//energy(10.f),
	smID(""),
	goTarget(NULL),
	m_currState(NULL),
	m_nextState(NULL),
	target(NULL),
	timer(0),
	currFrame(0),
	iGridX(1),
	iGridZ(1)
	//NTarget(NULL),
	//steps(0),
	//countDown(0),
	//side(Node::TOP_LEFT),
	//curr(0, 0),
	//mapRead(false),
	//moving(false),
	//currNode(NULL),
	//damage(0),
	//health(0),
	//actionTime(0.f),
	//range(0),
	//attBounceTime(0),
	//attMaxBounceTime(1)
{
	static int count = 0;
	id = ++count;
	this->type;
	//moveLeft = moveRight = moveUp = moveDown = true;

	std::cout << "GameObject Constructor" << std::endl;
}

GameObject::~GameObject()
{
	//while (!path.empty())
	{
	//	path.pop();
	}
}

bool GameObject::Handle(Message* msg)
{
	MessageObject* messageObject = dynamic_cast<MessageObject*>(msg);
	switch (messageObject->type)
	{
	case MessageObject::ACTIVE_OBJECT:
		if (this->active)
		{
			delete msg;
			return true;
		}
		break;
	}
	delete msg;
	return false;
}

void GameObject::TheFunction(GameObject * go)
{
	std::cout << "GameObject Function" << std::endl;
}
