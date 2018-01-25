#pragma once
#include "Engine/Math/IntVector2.hpp"
#include <vector>
class Path
{
public:
	Path();
	~Path();

public:
	std::vector<IntVector2> m_path;
};