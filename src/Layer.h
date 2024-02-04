#pragma once

class Layer {
public:
	Layer() {};
	virtual ~Layer() {};

	virtual void OnAttack() {}
	virtual void onDetach() {}
	virtual void onUpdate() {}
	virtual void onEvent(/* TODO: Event& event */) {}
};