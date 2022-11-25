/*
* File:		UI.h
* Author:	NetSkelter
* Created:	2022.11.25
*/

#ifndef ASC_UI_H
#define ASC_UI_H

#include "Entity.h"

namespace ASC {
	class Scene;
	class Shader;
	class UIGroup;

	class UILstr {
	public:
		virtual void cmptEvent(unsigned int, unsigned int) = 0;
		virtual Shader& getShader() = 0;
		virtual const Font& getFont() const = 0;
	};

	class UICmpt : public InputLstr {
	public:
		virtual void destroy() = 0;
		inline virtual UILstr& getParent() {
			return *parent_;
		}
		inline virtual unsigned int getID() const {
			return ID_;
		}
		virtual glm::vec3 getPos() const = 0;
		virtual void setPos(const glm::vec3&) = 0;
		virtual glm::vec2 getDims() const = 0;
		virtual void setDims(const glm::vec2&) = 0;

	protected:
		friend class UIGroup;

		UILstr* parent_ = 0;
		unsigned int ID_ = 0;

		virtual void draw() = 0;
		virtual void processInput() = 0;
		virtual void update(float) = 0;
	};

	class UIGroup : public UILstr {
	public:
		void init(Scene&, unsigned int, Shader&, const Font&);
		bool addCmpt(UICmpt&);
		bool removeCmpt(UICmpt&);
		void draw();
		void processInput();
		void cmptEvent(unsigned int, unsigned int) override;
		void update(float);
		void destroy();
		inline Scene& getParent() {
			return *parent_;
		}
		inline unsigned int getID() const {
			return ID_;
		}
		inline Shader& getShader() override {
			return *shader_;
		}
		inline void setShader(Shader& shader) {
			shader_ = &shader;
		}
		inline const Font& getFont() const override {
			return font_;
		}
		inline void setFont(const Font& font) {
			font_ = font;
		}
		inline bool isEnabled() const {
			return enabled_;
		}
		void setEnabled(bool enabled);
		inline bool isVisible() const {
			return visible_;
		}
		inline void setVisible(bool visible) {
			visible_ = visible;
		}

	private:
		Scene* parent_ = 0;
		unsigned int ID_ = 0;
		std::vector<UICmpt*> cmpts_;
		unsigned int nextID_ = 0;
		Shader* shader_ = 0;
		Font& font_ = Renderer::NO_FONT;
		bool enabled_ = true;
		bool visible_ = true;
		glm::ivec2 prevWindowDims_ = glm::ivec2();
	};

	class Label : public UICmpt {
	public:
		virtual void init(const glm::vec3&, const glm::vec2&, const GLuint, const std::string&,
			float, const glm::vec3&, Alignment = ASC_ALIGN_CENTER, Alignment = ASC_ALIGN_CENTER);
		virtual void destroy() override;
		virtual glm::vec3 getPos() const override;
		virtual void setPos(const glm::vec3&) override;
		virtual glm::vec2 getDims() const override;
		virtual void setDims(const glm::vec2&) override;
		inline virtual const std::string& getText() const {
			return text_;
		}
		inline virtual void setText(const std::string& text) {
			text_ = text;
		}
		inline virtual float getTextScale() const {
			return textScale_;
		}
		inline virtual void setTextScale(float textScale) {
			textScale_ = textScale;
		}
		inline virtual const glm::vec3& getTextColor() const {
			return textColor_;
		}
		inline virtual void setTextColor(const glm::vec3& textColor) {
			textColor_ = textColor;
		}
		inline virtual Alignment getHAlign() const {
			return hAlign_;
		}
		inline virtual void setHAlign(Alignment hAlign) {
			hAlign_ = hAlign;
		}
		inline virtual Alignment getVAlign() const {
			return vAlign_;
		}
		inline virtual void setVAlign(Alignment vAlign) {
			vAlign_ = vAlign;
		}

	protected:
		Sprite bgSprite_;
		std::string text_ = "";
		float textScale_ = 1.0f;
		glm::vec3 textColor_ = glm::vec3();
		Alignment hAlign_ = ASC_ALIGN_CENTER;
		Alignment vAlign_ = ASC_ALIGN_CENTER;

		virtual void draw() override;
		virtual void processInput() override {}
		virtual void update(float) override {}
	};

	class Button : public Label {
	public:
		const static unsigned int UNSELECTED_EVENT = 0;
		const static unsigned int SELECTED_EVENT = 1;
		const static unsigned int CLICKED_EVENT = 2;

		virtual void init(const glm::vec3&, const glm::vec2&, const GLuint, const std::string&,
			float, const glm::vec3&, Alignment = ASC_ALIGN_CENTER, Alignment = ASC_ALIGN_CENTER) override;
		virtual void destroy() override;
		inline virtual bool isSelected() const {
			return selected_;
		}
		virtual void setSelected(bool selected);
		inline virtual bool isClicked() const {
			return clicked_;
		}
		virtual void setClicked(bool clicked);

	protected:
		friend class Cycle;

		static glm::vec4 UNSELECTED_COORDS;
		static glm::vec4 SELECTED_COORDS;
		static glm::vec4 CLICKED_COORDS;
		bool selected_ = false;
		bool clicked_ = false;

		virtual void processInput() override;
	};

	class Switch : public Label {
	public:
		const static unsigned int OFF_EVENT = 0;
		const static unsigned int ON_EVENT = 1;

		virtual void init(const glm::vec3&, const glm::vec2&, const GLuint, const std::string&,
			float, const glm::vec3&, Alignment = ASC_ALIGN_TOP, Alignment = ASC_ALIGN_LEFT,
			Alignment = ASC_ALIGN_CENTER);
		virtual void destroy() override;
		inline virtual bool isOn() const {
			return on_;
		}
		virtual void setOn(bool);
		inline virtual Alignment getLAlign() const {
			return lAlign_;
		}
		inline virtual void setLAlign(Alignment lAlign) {
			lAlign_ = lAlign;
		}

	protected:
		static glm::vec4 OFF_COORDS;
		static glm::vec4 ON_COORDS;
		bool on_ = false;
		Alignment lAlign_ = ASC_ALIGN_TOP;

		virtual void draw() override;
		virtual void processInput() override;
	};

	class Slider : public Label {
	public:
		const static unsigned int SET_EVENT = 0;

		virtual void init(const glm::vec3&, const glm::vec2&, const GLuint, const GLuint, float,
			const std::string&, float, const glm::vec3&, Alignment = ASC_ALIGN_TOP,
			Alignment = ASC_ALIGN_LEFT, Alignment = ASC_ALIGN_CENTER);
		virtual void destroy() override;
		inline virtual float getValue() const {
			return value_;
		}
		inline virtual void setValue(float value) {
			value_ = value;
			parent_->cmptEvent(ID_, SET_EVENT);
		}

	protected:
		Alignment lAlign_ = ASC_ALIGN_TOP;
		Sprite cursor_;
		float value_ = 0.0f;
		bool selected_ = false;

		virtual void draw() override;
		virtual void processInput() override;
		virtual void update(float) override;
	};

	class Cycle : public Label, UILstr {
	public:
		const static unsigned int SET_EVENT = 0;

		virtual void init(const glm::vec3&, const glm::vec2&, const GLuint, const GLuint,
			const std::vector<std::string>&, const std::string&, const std::string&,
			const std::string&, float, const glm::vec3&, Alignment = ASC_ALIGN_CENTER,
			Alignment = ASC_ALIGN_CENTER, Alignment = ASC_ALIGN_RIGHT, Alignment = ASC_ALIGN_TOP,
			Alignment = ASC_ALIGN_LEFT, Alignment = ASC_ALIGN_CENTER);
		virtual void destroy() override;
		virtual void addValue(const std::string&);
		virtual void removeValue(int);
		virtual int getIndexOf(const std::string&) const;
		virtual const std::string& getValue() const;
		inline virtual int getIndex() const {
			return value_;
		}
		virtual void setText(const std::string&) override;
		virtual glm::vec3 getPos() const override;
		virtual void setPos(const glm::vec3&) override;
		virtual glm::vec2 getDims() const override;
		virtual void setDims(const glm::vec2&) override;
		virtual void cmptEvent(unsigned int, unsigned int) override;
		inline virtual Shader& getShader() override {
			return parent_->getShader();
		}
		inline virtual const Font& getFont() const override {
			return parent_->getFont();
		}

	protected:
		Alignment bAlign_ = ASC_ALIGN_RIGHT;
		Button prevB_;
		Button nextB_;
		std::vector<std::string> values_;
		int value_ = -1;
		std::string labelText_ = "";
		Alignment lAlign_ = ASC_ALIGN_TOP;
		Alignment lHAlign_ = ASC_ALIGN_LEFT;
		Alignment lVAlign_ = ASC_ALIGN_CENTER;

		virtual void draw() override;
		virtual void processInput() override;
		virtual void update(float) override;
	};

	class TextBox : public Label {
	public:
		const static unsigned int UNSELECTED_EVENT = 0;
		const static unsigned int SELECTED_EVENT = 1;
		const static unsigned int ENTERED_EVENT = 2;

		virtual void init(const glm::vec3&, const glm::vec2&, const GLuint, const GLuint, float,
			const std::string&, const std::string&, float, const glm::vec3&, int = -1,
			const std::string& = "", Alignment = ASC_ALIGN_CENTER, Alignment = ASC_ALIGN_TOP,
			Alignment = ASC_ALIGN_LEFT, Alignment = ASC_ALIGN_CENTER);
		virtual void setPos(const glm::vec3&) override;
		virtual void setText(const std::string&) override;
		virtual void charTyped(char c) override;

	protected:
		static glm::vec4 UNSELECTED_COORDS;
		static glm::vec4 SELECTED_COORDS;
		bool selected_ = false;
		std::string labelText_ = "";
		float textPos_ = 0.0f;
		Sprite cursor_;
		unsigned int cursorPos_ = 0;
		float timer_ = 0.0f;
		int maxChars_ = -1;
		std::string allowedChars_ = "";
		Alignment lAlign_ = ASC_ALIGN_TOP;
		Alignment lHAlign_ = ASC_ALIGN_LEFT;
		Alignment lVAlign_ = ASC_ALIGN_CENTER;

		virtual void draw() override;
		virtual void processInput() override;
		virtual void update(float) override;
	};
}

#endif