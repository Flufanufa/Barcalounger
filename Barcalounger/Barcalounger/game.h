#ifndef MYGAME_H
#define MYGAME_H

#include "gameObject.h"
#include "Engine.h"

class Game
{
public:
	Game() {}
	virtual ~Game() {}

	//CHANGE ADDTOSCENE BACK TO PROTECTED
	void AddToScene(GameObject* child) { root.AddChild(child); }

	virtual void Init();
	void Input(float delta);
	void Update(float delta);
	void Render(RenderingEngine* renderingEngine);

	inline GameObject& GetRoot() { return root; }

	inline void SetEngine(CoreEngine* engine) { root.SetEngine(engine); }
protected:
	
private:
	Game(Game& game) {}
	void operator=(Game& game) {}

	GameObject root;
};

#endif