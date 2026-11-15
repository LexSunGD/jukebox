#include "jukebox/ui/indexes_setting.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <Geode/Result.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/cocos/cocoa/CCObject.h>
#include <Geode/cocos/platform/CCPlatformMacros.h>  // IWYU pragma: keep
#include <Geode/loader/SettingV3.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/cocos.hpp>
#include <matjson.hpp>

#include "jukebox/nong/index.hpp"
#include "jukebox/ui/indexes_popup.hpp"

using namespace geode::prelude;

namespace jukebox {

using namespace jukebox::index;

bool Indexes::operator==(const Indexes& other) const { return this->indexes == other.indexes; }

SettingNodeV3* IndexSetting::createNode(float width) {
    return IndexSettingNode::create(std::static_pointer_cast<IndexSetting>(shared_from_this()), width);
}

bool IndexSettingNode::init(const std::shared_ptr<IndexSetting>& setting, float width) {
    if (!SettingValueNodeV3::init(setting, width)) { return false; }

    float height = 40.0f;
    this->setContentSize({width, height});
    auto* viewSpr = ButtonSprite::create("View");
    viewSpr->setScale(0.72f);
    auto* viewBtn = Button::createWithNode(viewSpr, [this](Button*) { this->onView(); });
    viewBtn->setID("view-button");
    viewBtn->setPosition(width - 40.f, height - 20.f);

    this->addChild(viewBtn);
    handleTouchPriority(this);

    /*updateVisuals();*/

    return true;
}

void IndexSettingNode::onView() {
    const std::shared_ptr<IndexSetting> setting = this->getSetting();
    IndexesPopup::create(setting->getValue().indexes, [this](std::vector<IndexSource> indexes) {
        this->setValue(Indexes {std::move(indexes)}, this);
    })->show();
}

IndexSettingNode* IndexSettingNode::create(const std::shared_ptr<IndexSetting>& setting, float width) {
    auto* const ret = new IndexSettingNode();

    if (ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

Result<std::shared_ptr<SettingV3>> IndexSetting::parse(
    const std::string& key, const std::string& modID, const matjson::Value& json
) {
    const auto ret = std::make_shared<IndexSetting>();
    auto root = checkJson(json, "IndexSetting");
    ret->parseBaseProperties(key, modID, root);

    root.checkUnknownKeys();

    return root.ok(ret).map([](std::shared_ptr<IndexSetting> value) {
        return std::static_pointer_cast<SettingV3>(value);
    });
}

}  // namespace jukebox
