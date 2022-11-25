/*
* File:		Graphics.h
* Author:	NetSkelter
* Created:	2022.11.25
*/

#ifndef ASC_GRAPHICS_H
#define ASC_GRAPHICS_H

#include <vector>
#include <map>

#include "Entity.h"

namespace ASC {
	class Shader {
	public:
		void operator = (const Shader&);
		bool init(const std::string&, const std::string&, const std::string&,
			const std::vector<unsigned int>&);
		void begin();
		void end();
		void destroy();
		inline Camera& getCamera() {
			return camera_;
		}

	private:
		GLuint ID_ = 0;
		struct VAttrib {
			unsigned int size = 0;
			unsigned int offset = 0;
		};
		std::vector<VAttrib> attributes_;
		int stride_ = 0;
		Camera camera_;
		std::string cameraUniform_ = "";
	};

	class Renderer {
	public:
		static GLuint NO_TEXTURE;
		static Font NO_FONT;

		bool init(const glm::vec3&);
		void update(float);
		void begin();
		template <typename T>
		void submit(T& sprite, Shader& shader) {
			sprite.draw();
			Sprite* s = (Sprite*)&sprite;
			if (s->vertices_.empty()) {
				return;
			}
			sprites_.push_back(std::pair<Shader*, Sprite*>(&shader, s));
		}
		template <typename T>
		void submit(T& sprite) {
			submit(sprite, defaultShader_);
		}
		template <typename T>
		void submit(std::vector<T>& sprites, Shader& shader) {
			for (unsigned int i = 0; i < sprites.size(); i++) {
				submit(sprites[i], shader);
			}
		}
		template <typename T>
		void submit(std::vector<T>& sprites) {
			submit(sprites, defaultShader_);
		}
		void submit(const std::string&, const glm::vec3&, const glm::vec4&, float, const glm::vec3&,
			const Font&, Alignment, Alignment, Shader&);
		void submit(const std::string&, const glm::vec3&, const glm::vec4&, float, const glm::vec3&,
			const Font&, Alignment, Alignment);
		inline void dump() {
			dump_ = true;
		}
		void end();
		const GLuint getTexture(const std::string&);
		const Font& getFont(const std::string&);
		void destroy();
		inline Shader& getShader() {
			return defaultShader_;
		}
		inline void setShader(const Shader& s) {
			defaultShader_ = s;
		}

	private:
		bool dump_ = false;
		GLuint VAO_ = 0;
		GLuint VBO_ = 0;
		GLuint IBO_ = 0;
		std::map<std::string, GLuint> textures_;
		std::map<std::string, Font> fonts_;
		FT_Library freeType_ = 0;
		std::map<Shader*, std::vector<Glyph>> glyphs_;
		std::vector<std::pair<Shader*, Sprite*>> sprites_;
		Shader defaultShader_;

		void drawVertices(std::vector<float>&, std::vector<unsigned int>&);
	};
}

#endif