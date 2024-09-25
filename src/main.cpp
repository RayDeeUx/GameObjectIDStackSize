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
	CreateMenuItem* getCreateBtn(int id, int bg) {
		auto result = EditorUI::getCreateBtn(id, bg);
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
		int font = getInt("stackSizeFont");
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
		label->setAnchorPoint({1.f, 0.f});
		label->setScale(static_cast<float>(getDouble("scale")));
		label->setPosition(
			{
				buttonSprite->getPositionX() + buttonSprite->getContentWidth() / 2.f - static_cast<float>(getDouble("padding")),
				buttonSprite->getPositionY() - buttonSprite->getContentHeight() / 2.f + static_cast<float>(getDouble("padding"))
			}
		);
		label->setZOrder(gameObject->getZOrder() + 2);
		label->setID(nodeID);
		buttonSprite->addChild(label);
		buttonSprite->updateLayout();
		return result;
	}
};

/*
#include <Geode/modify/CreateMenuItem.hpp>
class $modify(MyCreateMenuItem, CreateMenuItem) {
	static void onModify(auto& self) {
		(void) self.setHookPriority("CreateMenuItem::create", -2123456789);
	}
	static CreateMenuItem* create(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCObject* p2, cocos2d::SEL_MenuHandler p3) {
		auto result = CreateMenuItem::create(p0, p1, p2, p3);
		auto buttonSprite = getChildOfType<ButtonSprite>(result, 1);
		if (!buttonSprite) return result;
		auto gameObject = getChildOfType<GameObject>(buttonSprite, 1);
		if (!gameObject) return result;
		int objectID = gameObject->m_objectID;
		auto label = CCLabelBMFont::create();
		auto objectIDAsCString = fmt::format("{}", objectID);
		label->setString(objectIDAsCString.c_str());
		std::string fontFile = "bigFont.fnt";
		int font = Mod::get()->getSettingValue<int64_t>("stackSizeFont");
		if (font == 0) {
			fontFile = "bigFont.fnt";
		} else if (font == -1) {
			fontFile = "goldFont.fnt";
		} else if (font == -2) {
			fontFile = "chatFont.fnt";
		} else if (font > 0) {
			fontFile = fmt::format("gjFont{:02d}.fnt", font);
		}
		label->setFntFile(fontFile.c_str());
		label->setAlignment(kCCTextAlignmentRight);
		buttonSprite->addChild(label);
		buttonSprite->updateLayout();
		return result;
	}
};
*/