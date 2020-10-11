#ifndef ENTITIES_H
#define ENTITIES_H

#include <GL/glew.h>

struct Entity
{
	GLshort id;
	GLshort version;
};

class Entities
{
	public:
		Entities();
		~Entities();


};

#endif