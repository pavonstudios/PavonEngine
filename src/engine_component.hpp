#pragma once
constexpr unsigned int NULL_COMPONENT = 0;
class Engine;
class EngineComponent {
public:
	unsigned int id = NULL_COMPONENT;
	const char* name;
	Engine* engine;
};