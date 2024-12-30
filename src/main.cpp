#include <Geode/modify/EditorUI.hpp>

#define getBool Mod::get()->getSettingValue<bool>
#define getDouble Mod::get()->getSettingValue<double>
#define getInt Mod::get()->getSettingValue<int64_t>
#define getColor Mod::get()->getSettingValue<cocos2d::ccColor4B>

using namespace geode::prelude;

class $modify(MyEditorUI, EditorUI) {
	static void onModify(auto& self) { (void) self.setHookPriority("EditorUI::getCreateBtn", -2123456789); }
	CCLabelBMFont* makeLabel(int id) {
		std::string fontFile = "bigFont.fnt";
		int font = getInt("stackSizeFont");
		if (font == -1) {
			fontFile = "goldFont.fnt";
		} else if (font == -2) {
			fontFile = "chatFont.fnt";
		} else if (font > 0) {
			fontFile = fmt::format("gjFont{:02d}.fnt", font);
		}
		auto label = CCLabelBMFont::create(fmt::format("{}", id).c_str(), fontFile.c_str());
		auto nodeID = fmt::format("{}/objectID-{}-stacksize", Mod::get()->getID(), id);
		auto color = getColor("color");
		if (getBool("readableMode") && font == -2)
			label->setBlendFunc({GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA});
		label->setAlignment(kCCTextAlignmentRight);
		label->setAnchorPoint({1.f, 0.f});
		label->setColor({color.r, color.g, color.b});
		label->setOpacity(color.a);
		label->setScale(static_cast<float>(getDouble("scale")));
		label->setID(nodeID);
		return label;
	}
	CreateMenuItem* getCreateBtn(int id, int bg) {
		auto result = EditorUI::getCreateBtn(id, bg);
		if (!getBool("enabled")) return result;
		CCLabelBMFont* label = makeLabel(id);
		auto padding = static_cast<float>(getDouble("padding"));
		if (getBool("extraSafety")) {
			ButtonSprite* buttonSprite = result->getChildByType<ButtonSprite>(0);
			if (!buttonSprite) return result;
			GameObject* gameObject = buttonSprite->getChildByType<GameObject>(0);
			if (!gameObject || gameObject->m_objectID != id) return result;
			label->setPosition(
				{
					buttonSprite->getPositionX() + buttonSprite->getContentWidth() / 2.f - padding,
					buttonSprite->getPositionY() - buttonSprite->getContentHeight() / 2.f + padding
				}
			);
			label->setZOrder(gameObject->getZOrder() + 2);
			buttonSprite->addChild(label);
			buttonSprite->updateLayout();
		} else {
			padding /= 2.f;
			label->setPosition({35 - padding, 2 + padding});
			label->setZOrder(1);
			result->addChild(label);
			result->updateLayout();
		}
		return result;
	}
};