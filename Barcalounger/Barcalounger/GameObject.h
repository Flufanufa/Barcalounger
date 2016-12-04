#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "transform.h"
class CoreEngine;
class GameComponent;
class Shader;
class RenderingEngine;

class GameObject
{
public:
	GameObject() { coreEngine = 0; }
	virtual ~GameObject();

	GameObject* AddChild(GameObject* child);
	GameObject* AddComponent(GameComponent* component);

	void InputAll(float delta);
	void UpdateAll(float delta);
	void RenderAll(Shader* shader, RenderingEngine* renderingEngine);

	std::vector<GameObject*> GetAllAttached();

	inline Transform& GetTransform() { return transform; }
	void SetEngine(CoreEngine* engine);
protected:
private:
	void Input(float delta);
	void Update(float delta);
	void Render(Shader* shader, RenderingEngine* renderingEngine);

	std::vector<GameObject*> children;
	std::vector<GameComponent*> components;
	Transform transform;
	CoreEngine* coreEngine;
};

#endif // GAMEOBJECT_H
