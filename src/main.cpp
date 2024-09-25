#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(MyEditorUI, EditorUI) {
	struct Fields {
		Mod* mod = Mod::get();
	};
	static void onModify(auto& self) {
		(void) self.setHookPriority("EditorUI::getCreateBtn", -2123456789);
	}
	bool getBool(std::string_view key) {
		return m_fields->mod->getSettingValue<bool>(key);
	}
	int64_t getInt(std::string_view key) {
		return m_fields->mod->getSettingValue<int64_t>(key);
	}
	double getDouble(std::string_view key) {
		return m_fields->mod->getSettingValue<double>(key);
	}
	cocos2d::ccColor4B getColor(std::string_view key) {
		return m_fields->mod->getSettingValue<cocos2d::ccColor4B>(key);
	}
	CreateMenuItem* getCreateBtn(int id, int bg) {
		auto result = EditorUI::getCreateBtn(id, bg);
		if (!MyEditorUI::getBool("enabled")) return result;
		ButtonSprite* buttonSprite = nullptr;
		for (auto node : CCArrayExt<CCNode*>(result->getChildren())) {
			if (auto bs = typeinfo_cast<ButtonSprite*>(node)) {
				buttonSprite = bs;
				break;
			}
		}
		if (!buttonSprite) return result;
		GameObject* gameObject = nullptr;
		for (auto node : CCArrayExt<CCNode*>(buttonSprite->getChildren())) {
			if (auto go = typeinfo_cast<GameObject*>(node)) {
				gameObject = go;
				break;
			}
		}
		if (!gameObject) return result;
		std::string fontFile = "bigFont.fnt";
		int font = MyEditorUI::getInt("stackSizeFont");
		if (font == 0) {
			fontFile = "bigFont.fnt";
		} else if (font == -1) {
			fontFile = "goldFont.fnt";
		} else if (font == -2) {
			fontFile = "chatFont.fnt";
		} else if (font > 0) {
			fontFile = fmt::format("gjFont{:02d}.fnt", font);
		}
		auto objectIDAsCString = fmt::format("{}", id);
		auto label = CCLabelBMFont::create(objectIDAsCString.c_str(), fontFile.c_str(), 10.f, kCCTextAlignmentRight);
		auto nodeID = fmt::format("{}/objectID-{}-stacksize", m_fields->mod->getID(), id);
		auto color = MyEditorUI::getColor("color");
		auto padding = static_cast<float>(MyEditorUI::getDouble("padding"));
		if (MyEditorUI::getBool("readableMode")) label->setBlendFunc({GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA});
		label->setAnchorPoint({1.f, 0.f});
		label->setColor({color.r, color.g, color.b});
		label->setOpacity(color.a);
		label->setScale(static_cast<float>(getDouble("scale")));
		label->setPosition(
			{
				buttonSprite->getPositionX() + buttonSprite->getContentWidth() / 2.f - padding,
				buttonSprite->getPositionY() - buttonSprite->getContentHeight() / 2.f + padding
			}
		);
		label->setZOrder(gameObject->getZOrder() + 2);
		label->setID(nodeID);
		buttonSprite->addChild(label);
		buttonSprite->updateLayout();
		return result;
	}
};