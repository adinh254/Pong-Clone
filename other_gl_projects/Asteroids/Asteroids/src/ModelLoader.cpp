#include "ModelLoader.h"

bool loadObj( const std::string& path,
	std::vector<Vertex>& out_vertices,
	std::vector<GLushort>& out_elements )
{
	std::ifstream in_file( path, std::ios::in );
	if( !in_file )
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Cannot open %s!", path.c_str() );
		return false;
	}
	std::string line;
	while( std::getline( in_file, line ) )
	{
		std::istringstream stream;
		if( line.find("v ") != std::string::npos )
		{
			stream.str( line.substr( 2 ) );
			Vertex vertex;
			glm::vec3 position;
			stream >> position.x;
			stream >> position.y;
			stream >> position.z;
			vertex.position = position;

			out_vertices.push_back( vertex );
		}
		else if( line.find( "f " ) != std::string::npos )
		{
			stream.str( line.substr( 2 ) );
			GLushort face_index;
			while( stream >> face_index )
			{
				face_index--;
				out_elements.push_back( face_index );
			}
		}
	}

	in_file.close();
	return true;
}