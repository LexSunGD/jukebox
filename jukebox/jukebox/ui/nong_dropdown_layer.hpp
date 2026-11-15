#pragma once

#include <vector>

#include <Geode/binding/CustomSongWidget.hpp>
#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/cocos/cocoa/CCObject.h>
#include <Geode/cocos/platform/CCPlatformMacros.h>  // IWYU pragma: keep
#include <Geode/loader/Event.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/cocos.hpp>

#include "jukebox/nong/nong.hpp"
#include "jukebox/ui/list/nong_list.hpp"
#include "jukebox/ui/list/song_cell.hpp"

namespace jukebox {

class NongDropdownLayer : public geode::Popup {
protected:
    std::vector<int> m_songIDS;
    std::optional<int> m_currentSongID = std::nullopt;
    int m_defaultSongID = 0;
    geode::Ref<CustomSongWidget> m_parentWidget;
    NongList* m_list = nullptr;
    std::optional<int> m_levelID;

    geode::Button* m_addBtn = nullptr;
    geode::Button* m_discordBtn = nullptr;
    geode::Button* m_deleteBtn = nullptr;
    cocos2d::CCNode* m_bottomRightMenu = nullptr;

    geode::ListenerHandle m_songErrorListener;
    geode::ListenerHandle m_songInfoListener;
    geode::ListenerHandle m_downloadFailedListener;

    bool m_fetching = false;

    bool init(
        float width,
        float height,
        std::vector<int> ids,
        CustomSongWidget* parent,
        int defaultSongID,
        std::optional<int> levelID,
        const char* bg
    );
    void createList();
    cocos2d::CCSize getCellSize() const;
    void deleteAllNongs();
    void fetchSongFileHub(cocos2d::CCObject*);
    void onSettings();
    void openAddPopup();

public:
    void onSelectSong(int songID);
    void onDiscord();
    void addSong(Nongs&& song, bool popup = true);
    void updateParentWidget(SongMetadata const& song);

    static NongDropdownLayer* create(
        std::vector<int> ids, CustomSongWidget* parent, const int defaultSongID, const std::optional<int> levelID
    ) {
        auto* ret = new NongDropdownLayer;
        if (ret->init(420.f, 280.f, std::move(ids), parent, defaultSongID, levelID, "GJ_square01.png")) {
            ret->autorelease();
            return ret;
        }

        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

}  // namespace jukebox
