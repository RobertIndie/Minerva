// Core.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

#include "renderer.h"

class App :public Renderer
{
public:
	typedef class Renderer base;
	void Initialize();
	void Update();
	~App();
};