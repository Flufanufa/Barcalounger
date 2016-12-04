#ifndef GAMECOMPONENT_H_INCLUDED
#define GAMECOMPONENT_H_INCLUDED

#include "transform.h"
#include "gameObject.h"
class RenderingEngine;
class Shader;

class GameComponent
{
public:
	virtual ~GameComponent() {}

	virtual void Input(float delta) {}
	virtual void Update(float delta) {}
	virtual void Render(Shader* shader, RenderingEngine* renderingEngine) {}

	virtual void AddToEngine(CoreEngine* engine) { }

	inline void SetParent(GameObject* _parent) { parent = _parent; }
	inline Transform& GetTransform() { return parent->GetTransform(); }
	inline const Transform& GetTransform() const { return parent->GetTransform(); }

private:
	GameObject* parent;
};

#endif // GAMECOMPONENT_H_INCLUDED
