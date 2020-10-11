#include "Game.h"
#include "renderer/common/Mesh.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <glm/glm.hpp>

bool loadObj( const std::string& path,
	std::vector<Vertex>& out_vertices,
	std::vector<GLushort>& out_elements );