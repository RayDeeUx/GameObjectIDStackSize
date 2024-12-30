#include <Geode/modify/EditorUI.hpp>

#define getBool Mod::get()->getSettingValue<bool>
#define getDouble Mod::get()->getSettingValue<double>
#define getInt Mod::get()->getSettingValue<int64_t>
#define getColor Mod::get()->getSettingValue<cocos2d::ccColor4B>

using namespace geode::prelude;

class $modify(MyEditorUI, EditorUI) {
	static void onModify(auto& self) {
		(void) self.setHookPriority("EditorUI::getCreateBtn", -2123456789);
	}
	struct Fields {
		float m_padding;
		float m_scale;
		ccColor4B m_color;
		int m_font;
		bool m_enabled;
		bool m_extraSafe;
		bool m_readableFont;
		std::string m_fontFile;
	};
	bool init(LevelEditorLayer* editorLayer) {
		if (!EditorUI::init(editorLayer)) return false;
		auto fields = m_fields.self();
		fields->m_padding = getDouble("padding");
		fields->m_scale = getDouble("scale");
		fields->m_color = getColor("color");
		fields->m_font = getInt("stackSizeFont");
		fields->m_enabled = getBool("enabled");
		fields->m_extraSafe = getBool("extraSafety");
		fields->m_readableFont = getBool("readableMode");
		fields->m_fontFile = "bigFont.fnt";
		if (fields->m_font == 0) return true;
		if (fields->m_font == -1) {
			fields->m_fontFile = "goldFont.fnt";
		} else if (fields->m_font == -2) {
			fields->m_fontFile = "chatFont.fnt";
		} else if (fields->m_font > 0) {
			fields->m_fontFile = fmt::format("gjFont{:02d}.fnt", fields->m_font);
		}
		return true;
	}
	CCLabelBMFont* makeLabel(int id) {
		auto fields = m_fields.self();
		auto label = CCLabelBMFont::create(fmt::format("{}", id).c_str(), fields->m_fontFile.c_str());
		auto nodeID = fmt::format("{}/objectID-{}-stacksize", Mod::get()->getID(), id);
		auto color = m_fields->m_color;
		if (fields->m_readableFont && fields->m_font == -2)
			label->setBlendFunc({GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA});
		label->setAlignment(kCCTextAlignmentRight);
		label->setAnchorPoint({1.f, 0.f});
		label->setColor({color.r, color.g, color.b});
		label->setOpacity(color.a);
		label->setScale(fields->m_scale);
		label->setID(nodeID);
		return label;
	}
	CreateMenuItem* getCreateBtn(int id, int bg) {
		auto result = EditorUI::getCreateBtn(id, bg);
		auto fields = m_fields.self();
		if (!fields->m_enabled) return result;
		CCLabelBMFont* label = makeLabel(id);
		auto padding = m_fields->m_padding;
		if (m_fields->m_extraSafe) {
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