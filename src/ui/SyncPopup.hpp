#pragma once

#include <rhatt/util/WebRes.hpp>

#include <Geode/Geode.hpp>

namespace cw::ferry {
    enum class SyncType : uint8_t {
        GameSettings,
        GeodeSettings,
        ModSettings,
        ModSaves,
    };

    namespace ui {
        class SyncPopup final : public geode::Popup {
            using TaskResult = arc::Future<geode::Result<>>;

            struct SaveButtonData final {
                std::string id;
                std::string text;
                std::string icon;
                std::string background;
                geode::Button::ButtonCallback callback = nullptr;
            };

        private:
            std::atomic<bool> m_inProgress = false;
            geode::Ref<geode::ProcessPopup> m_progressPopup = nullptr;

            geode::async::TaskHolder<geode::Result<>> m_tasks;

            asp::Mutex<std::map<uint8_t, SyncType>> m_toSync;

            asp::SmallVec<SyncType, 4> getSyncTypes() const;

            TaskResult runTaskForIndex(uint8_t i, bool upload = false);

            TaskResult startGVUploadTask();
            TaskResult startGVDownloadTask();

            TaskResult startGeodeUploadTask();
            TaskResult startGeodeDownloadTask();

            TaskResult startModUploadTask();
            TaskResult startModDownloadTask();

        protected:
            TaskResult runUploadTasks();
            TaskResult runDownloadTasks();

            void onExit() override;

            bool init() override;

        public:
            static SyncPopup* create();
        };

        class SyncSelect final : public cocos2d::CCMenu {
            using Callback = geode::Function<void(SyncType, bool)>;

        private:
            SyncType m_type;
            Callback m_callback = nullptr;

        protected:
            void onToggle(cocos2d::CCObject* sender);

            bool init(SyncType type, Callback&& cb);

        public:
            static SyncSelect* create(SyncType type, Callback&& cb);
        };
    };
};