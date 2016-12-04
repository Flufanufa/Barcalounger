#include "gameObject.h"
#include "gameComponent.h"
#include "Engine.h"

GameObject::~GameObject()
{
	for (unsigned int i = 0; i < components.size(); i++)
		if (components[i])
			delete components[i];

	for (unsigned int i = 0; i < children.size(); i++)
		if (children[i])
			delete children[i];
}

GameObject* GameObject::AddChild(GameObject* _child)
{
	children.push_back(_child);
	_child->GetTransform().SetParent(&transform);
	_child->SetEngine(coreEngine);
	return this;
}

GameObject* GameObject::AddComponent(GameComponent* _component)
{
	components.push_back(_component);
	_component->SetParent(this);
	return this;
}

void GameObject::InputAll(float _delta)
{
	Input(_delta);

	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->InputAll(_delta);
}

void GameObject::UpdateAll(float _delta)
{
	Update(_delta);

	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->UpdateAll(_delta);
}

void GameObject::RenderAll(Shader* _shader, RenderingEngine* _renderingEngine)
{
	Render(_shader, _renderingEngine);

	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->RenderAll(_shader, _renderingEngine);
}

void GameObject::Input(float _delta)
{
	transform.Update();

	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Input(_delta);
}

void GameObject::Update(float _delta)
{
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Update(_delta);
}

void GameObject::Render(Shader* _shader, RenderingEngine* _renderingEngine)
{
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Render(_shader, _renderingEngine);
}

void GameObject::SetEngine(CoreEngine* _engine)
{
	if (coreEngine != _engine)
	{
		coreEngine = _engine;

		for (unsigned int i = 0; i < components.size(); i++)
			components[i]->AddToEngine(_engine);

		for (unsigned int i = 0; i < children.size(); i++)
			children[i]->SetEngine(_engine);
	}
}

std::vector<GameObject*> GameObject::GetAllAttached()
{
	std::vector<GameObject*> result;

	for (unsigned int i = 0; i < children.size(); i++)
	{
		std::vector<GameObject*> childObjects = children[i]->GetAllAttached();
		result.insert(result.end(), childObjects.begin(), childObjects.end());
	}

	result.push_back(this);
	return result;
}
