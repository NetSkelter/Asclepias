/*
* File:		UI.cpp
* Author:	NetSkelter
* Created:	2022.11.25
*/

#include "Asclepias.h"

namespace ASC {
	void UIGroup::init(Scene& parent, unsigned int ID, Shader& shader, const Font& font) {
		parent_ = &parent;
		ID_ = ID;
		cmpts_.clear();
		nextID_ = 0;
		shader_ = &shader;
		font_ = font;
		enabled_ = true;
		visible_ = true;
		prevWindowDims_ = App::window().getDims();
		ASCLOG(UIGroup, Info, "Initialized UI group ", ID, ".");
	}

	bool UIGroup::addCmpt(UICmpt& cmpt) {
		if (std::find(cmpts_.begin(), cmpts_.end(), &cmpt) != cmpts_.end()) {
			ASCLOG(UIGroup, Warning, "Failed to add UI component to group ", ID_, ".");
			return false;
		}
		cmpt.ID_ = nextID_++;
		cmpt.parent_ = this;
		if (enabled_) {
			App::input().addLstr(cmpt);
		}
		cmpts_.push_back(&cmpt);
		ASCLOG(UIGroup, Info, "Added UI component ", cmpt.getID(), " to group ", ID_, ".");
		return true;
	}

	bool UIGroup::removeCmpt(UICmpt& cmpt) {
		std::vector<UICmpt*>::iterator it = std::find(cmpts_.begin(), cmpts_.end(), &cmpt);
		if (it == cmpts_.end()) {
			ASCLOG(UIGroup, Info, "Failed to remove UI component from group ", ID_, ".");
			return false;
		}
		if (enabled_) {
			App::input().removeLstr(cmpt);
		}
		cmpts_.erase(it);
		ASCLOG(UIGroup, Info, "Removed UI componet ", cmpt.getID(), " from group ", ID_, ".");
		return true;
	}

	void UIGroup::draw() {
		if (visible_) {
			for (UICmpt* cmpt : cmpts_) {
				cmpt->draw();
			}
		}
	}

	void UIGroup::processInput() {
		if (enabled_) {
			for (UICmpt* cmpt : cmpts_) {
				cmpt->processInput();
			}
		}
	}

	void UIGroup::cmptEvent(unsigned int cID, unsigned int eID) {
		parent_->cmptEvent(ID_, cID, eID);
	}

	void UIGroup::update(float dt) {
		if (enabled_) {
			for (UICmpt* cmpt : cmpts_) {
				cmpt->update(dt);
			}
		}
		if (prevWindowDims_ != App::window().getDims()) {
			glm::vec2 wd((float)App::window().getDims().x, (float)App::window().getDims().y);
			glm::vec2 scale(wd.x / (float)prevWindowDims_.x, wd.y / (float)prevWindowDims_.y);
			prevWindowDims_ = wd;
			for (UICmpt* cmpt : cmpts_) {
				glm::vec2 dims = cmpt->getDims();
				dims.x *= scale.x;
				dims.y *= scale.y;
				cmpt->setDims(dims);
				glm::vec3 pos = cmpt->getPos();
				pos.x *= scale.x;
				pos.y *= scale.y;
				cmpt->setPos(pos);
			}
		}
	}

	void UIGroup::destroy() {
		ASCLOG(UIGroup, Info, "Destroying UI group ", ID_, ".");
		for (UICmpt* cmpt : cmpts_) {
			cmpt->destroy();
		}
		cmpts_.clear();
		parent_ = 0;
		ID_ = 0;
		nextID_ = 0;
		shader_ = 0;
		font_ = Renderer::NO_FONT;
		enabled_ = true;
		visible_ = true;
		prevWindowDims_ = glm::ivec2();
	}

	void UIGroup::setEnabled(bool enabled) {
		if (enabled_ == enabled) {
			return;
		}
		enabled_ = enabled;
		if (enabled_) {
			for (UICmpt* cmpt : cmpts_) {
				App::input().addLstr(*cmpt);
			}
			ASCLOG(UIGroup, Info, "Enabled UI group ", ID_, ".");
		}
		else {
			for (UICmpt* cmpt : cmpts_) {
				App::input().removeLstr(*cmpt);
			}
			ASCLOG(UIGroup, Info, "Disabled UI group ", ID_, ".");
		}
	}

	void Label::init(const glm::vec3& pos, const glm::vec2& dims, const GLuint texture,
		const std::string& text, float textScale, const glm::vec3& textColor, Alignment hAlign,
		Alignment vAlign) {
		bgSprite_.init(glm::vec3(), glm::vec2(), texture);
		setPos(pos);
		setDims(dims);
		text_ = text;
		textScale_ = textScale;
		textColor_ = textColor;
		hAlign_ = hAlign;
		vAlign_ = vAlign;
	}

	void Label::destroy() {
		parent_ = 0;
		ID_ = 0;
		bgSprite_.destroy();
		text_ = "";
		textScale_ = 1.0f;
		textColor_ = glm::vec3();
		hAlign_ = ASC_ALIGN_CENTER;
		vAlign_ = ASC_ALIGN_CENTER;
	}

	glm::vec3 Label::getPos() const {
		glm::ivec2 wd = App::window().getDims();
		return glm::vec3(bgSprite_.pos.x / wd.x, bgSprite_.pos.y / wd.y, bgSprite_.pos.z);
	}

	void Label::setPos(const glm::vec3& pos) {
		glm::ivec2 wd = App::window().getDims();
		bgSprite_.pos = glm::vec3(pos.x * wd.x, pos.y * wd.y, pos.z);
	}

	glm::vec2 Label::getDims() const {
		glm::ivec2 wd = App::window().getDims();
		return glm::vec2(bgSprite_.dims.x / wd.x, bgSprite_.dims.y / wd.y);
	}

	void Label::setDims(const glm::vec2& dims) {
		glm::ivec2 wd = App::window().getDims();
		bgSprite_.dims = glm::vec2(dims.x * wd.x, dims.y * wd.y);
	}

	void Label::draw() {
		App::renderer().submit(bgSprite_, parent_->getShader());
		glm::vec3 textPos(bgSprite_.pos.x, bgSprite_.pos.y, bgSprite_.pos.z + 0.1f);
		glm::vec4 textBounds(bgSprite_.pos.x, bgSprite_.pos.y, bgSprite_.dims.x, bgSprite_.dims.y);
		App::renderer().submit(text_, textPos, textBounds, textScale_, textColor_, parent_->getFont(),
			hAlign_, vAlign_, parent_->getShader());
	}
}