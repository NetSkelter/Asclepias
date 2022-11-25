/*
* File:		Entity.h
* Author:	NetSkelter
* Created:	2022.11.25
*/

#ifndef ASC_ENTITY_H
#define ASC_ENTITY_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <ft2build.h>
#include <freetype/freetype.h>

namespace ASC {
	class Entity {
	public:
		glm::vec3 pos = glm::vec3();
		glm::vec2 vel = glm::vec2();
	};

	class Camera : public Entity {
	public:
		float scale = 1.0f;
		float scaleVel = 0.0f;

		void operator = (const Camera&);
		void init();
		void update(float);
		const glm::mat4& getMatrix() const {
			return matrix_;
		}

	private:
		glm::mat4 orthoMatrix_ = glm::mat4();
		glm::mat4 matrix_ = glm::mat4();
		glm::vec3 prevPos_ = glm::vec3();
		float prevScale_ = 1.0f;
		glm::ivec2 prevWindowDims_ = glm::ivec2();

		void updateMatrix();
	};

	class Sprite : public Entity {
	public:
		glm::vec2 dims = glm::vec2();
		float rot = 0.0f;
		float rotVel = 0.0f;
		GLuint texture = 0;
		glm::vec4 textureRect = glm::vec4();
		glm::vec3 color = glm::vec3();
		bool reflectH = false;
		bool reflectV = false;

		void init(const glm::vec3&, const glm::vec2&, const GLuint);
		virtual void update(float);
		virtual void destroy();
		virtual bool intersects(const Sprite&);

	protected:
		friend class Renderer;

		static std::vector<unsigned int> Indices;
		std::vector<float> vertices_;

		virtual void draw();
		glm::vec3 rotate(const glm::vec3&);
	};

	class Animation : public Sprite {
	public:
		void init(const glm::vec3&, const glm::vec2&, const GLuint, const glm::ivec2&, float);
		virtual void update(float) override;
		inline virtual void play() {
			playing_ = true;
		}
		inline virtual void pause() {
			playing_ = false;
		}
		virtual void stop();
		inline virtual bool isPlaying() const {
			return playing_;
		}
		virtual void destroy() override;

	protected:
		float frameTime_ = 0.0f;
		std::vector<glm::vec2> frameCoords_;
		bool playing_ = false;
		float timer_ = 0.0f;
		unsigned int frame_ = 0;
	};

	enum Alignment {
		ASC_ALIGN_LEFT, ASC_ALIGN_RIGHT, ASC_ALIGN_BOTTOM, ASC_ALIGN_TOP, ASC_ALIGN_CENTER,
		ASC_ALIGN_NONE,
	};

	class Glyph : public Sprite {
	public:
		glm::ivec2 bearing = glm::ivec2();
		unsigned int offset = 0;
	};

	class Font {
	public:
		float minBearing = 0.0f;
		float maxBearing = 0.0f;
		std::map<char, Glyph> glyphs;
	};
}

#endif