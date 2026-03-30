#include "Menu.h"

#include "../Rt.h"
#include "../Bus.h"
#include "../Draw.h"
#include "../Font.h"
#include "../Gfx.h"
#include "../../config.h"

Menu::Menu(Entity &parent) : Component(parent) {
    this->level.push(Main);

    allItems.emplace(ItemNewGame, std::make_shared<MenuItem>("New Game", [](GameKeyEvent &key) {
        if (key.key == GK_A) {
            RT_Context.setActiveScene(SceneForest);
        }
    }));

    allItems.emplace(ItemLoad, std::make_shared<MenuItem>("Load game", [](GameKeyEvent &key) {
        if (key.key == GK_A) {
            Bus::instance().publish(StateChangeRequestedEvent(StateLoading));
        }
    }));

    allItems.emplace(ItemContinue, std::make_shared<MenuItem>("Continue", [](GameKeyEvent &key) {
        if (key.key == GK_A) {
            Bus::instance().publish(StateChangeRequestedEvent(StateLoading));
        }
    }));

    allItems.emplace(ItemSettings, std::make_shared<MenuItem>("Settings", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(Settings);
        this->selectedIndex = 0;
        this->scrollOffset = 0;
    }));

    allItems.emplace(ItemBack, std::make_shared<MenuItem>("Back", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.pop();
        this->selectedIndex = 0;
        this->scrollOffset = 0;
    }));

    allItems.emplace(ItemAudioSettings, std::make_shared<MenuItem>("Audio Settings", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(AudioSettings);
        this->selectedIndex = 0;
        this->scrollOffset = 0;
    }));

    allItems.emplace(ItemMusicVolume,
                     std::make_shared<MenuItem>("Music Volume (%)", &this->musicVolume, [](GameKeyEvent &key) {
                         if (key.key == GK_LEFT) {
                             St::instance().decMusicVolume();
                         } else if (key.key == GK_RIGHT) {
                             St::instance().incMusicVolume();
                         }
                     }));

    allItems.emplace(ItemEffectsVolume,
                     std::make_shared<MenuItem>("Effects Volume (%)", &this->effectsVolume, [](GameKeyEvent &key) {
                         if (key.key == GK_LEFT) {
                             St::instance().decEffectsVolume();
                         } else if (key.key == GK_RIGHT) {
                             St::instance().incEffectsVolume();
                         }
                     }));

    allItems.emplace(ItemFps,
                     std::make_shared<MenuItem>("Target FPS", &this->currentFps, [](GameKeyEvent &key) {
                         if (key.key == GK_LEFT) {
                             St::instance().decFps();
                         } else if (key.key == GK_RIGHT) {
                             St::instance().incFps();
                         }
                     }));

    allItems.emplace(ItemWindowSize,
                     std::make_shared<MenuItem>("Window Size", &this->windowSize, [](GameKeyEvent &key) {
                         if (key.key == GK_LEFT) {
                             St::instance().decWindowSize();
                         } else if (key.key == GK_RIGHT) {
                             St::instance().incWindowSize();
                         }
                     }));

    allItems.emplace(ItemVideoSettings, std::make_shared<MenuItem>("Video Settings", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(VideoSettings);
        this->selectedIndex = 0;
        this->scrollOffset = 0;
    }));

    allItems.emplace(ItemKeyboardSettings, std::make_shared<MenuItem>("Keyboard", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(Keyboard);
        this->selectedIndex = 0;
        this->scrollOffset = 0;
    }));

    allItems.emplace(ItemGamepadSettings, std::make_shared<MenuItem>("Gamepad", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(Gamepad);
        this->selectedIndex = 0;
        this->scrollOffset = 0;
    }));

    allItems.emplace(ItemQuit, std::make_shared<MenuItem>("Quit", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(AreYouSure);
        this->selectedIndex = 1;
    }));

    allItems.emplace(ItemAreYouSure, std::make_shared<MenuItem>("Are you sure?"));

    allItems.emplace(ItemYes, std::make_shared<MenuItem>("Yes", [](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        St::instance().serialize();
        Bus::instance().publish(Event(EventQuit));
    }));

    allItems.emplace(ItemNo, std::make_shared<MenuItem>("No", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.pop();
        this->selectedIndex = 0;
        this->scrollOffset = 0;
    }));

    allItems.emplace(ItemReloadScene, std::make_shared<MenuItem>("Reload Scene", [](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        RT_Context.reloadActiveScene();
    }));

}

void Menu::render(uint8_t) {
    this->currentFps = std::to_string(static_cast<int>(St::instance().getFps()));
    this->musicVolume = std::to_string(St::instance().getMusicVolume());
    this->effectsVolume = std::to_string(St::instance().getEffectsVolume());
    St::instance().getWindowSize(this->windowSize);

    buildMenu();

    // Semi-transparent blue overlay
    SDL_Rect overlay;
    overlay.x = 0;
    overlay.y = 0;
    overlay.w = configWindowWidth;
    overlay.h = configWindowHeight;
    Color bgColor{88, 88, 250, 102};
    Draw::instance().box(bgColor, overlay);

    // Layout
    int lineH = Font::instance().lineHeight();
    int padding = static_cast<int>(4 * configZoomFactor);
    int itemH = lineH + padding;
    int itemW = configWindowWidth * 3 / 4;
    int startX = (configWindowWidth - itemW) / 2;

    // How many items fit on screen (with margin)
    int margin = static_cast<int>(16 * configZoomFactor);
    int visibleArea = configWindowHeight - (margin * 2);
    int maxVisible = visibleArea / itemH;

    // Clamp scroll so selected item is always visible
    if (selectedIndex < scrollOffset) {
        scrollOffset = selectedIndex;
    }
    if (selectedIndex >= scrollOffset + maxVisible) {
        scrollOffset = selectedIndex - maxVisible + 1;
    }

    int totalItems = (int)items.size();
    int visibleCount = std::min(totalItems - scrollOffset, maxVisible);

    // Center the visible items vertically
    int totalH = visibleCount * itemH;
    int startY = (configWindowHeight - totalH) / 2;

    for (int i = 0; i < visibleCount; i++) {
        int idx = i + scrollOffset;
        int y = startY + i * itemH;
        items.at(idx)->render(idx == selectedIndex, startX, y, itemW, itemH);
    }

    // Scrollbar (only when list is scrollable)
    if (totalItems > maxVisible) {
        int barW = static_cast<int>(4 * configZoomFactor);
        int trackX = startX + itemW + barW;
        int trackY = startY;
        int trackH = totalH;

        // Track
        Color trackColor{255, 255, 255, 40};
        SDL_Rect track;
        track.x = trackX;
        track.y = trackY;
        track.w = barW;
        track.h = trackH;
        Draw::instance().box(trackColor, track);

        // Thumb
        int thumbH = std::max(trackH * maxVisible / totalItems, barW * 2);
        int scrollRange = trackH - thumbH;
        int maxScroll = totalItems - maxVisible;
        int thumbY = trackY + (maxScroll > 0 ? scrollOffset * scrollRange / maxScroll : 0);

        Color thumbColor{255, 255, 255, 180};
        SDL_Rect thumb;
        thumb.x = trackX;
        thumb.y = thumbY;
        thumb.w = barW;
        thumb.h = thumbH;
        Draw::instance().box(thumbColor, thumb);
    }
}

void Menu::up() {
    for (int i = selectedIndex - 1; i >= 0; --i) {
        if (items.at(i)->canSelect) {
            selectedIndex = i;
            break;
        }
    }
}

void Menu::down() {
    for (int i = selectedIndex + 1; i < (int)items.size(); ++i) {
        if (items.at(i)->canSelect) {
            selectedIndex = i;
            break;
        }
    }
}

void Menu::key(GameKeyEvent &key) {
    if (!key.valid) return;
    if (key.state != GK_PUSHED) return;
    if (key.key == GK_B && menuState != AwaitBinding) {
        if (this->level.size() > 1) this->level.pop();
        this->selectedIndex = 0;
        this->scrollOffset = 0;
    } else if (key.key == GK_UP && menuState != AwaitBinding) {
        up();
    } else if (key.key == GK_DOWN && menuState != AwaitBinding) {
        down();
    } else {
        this->items.at(selectedIndex)->call(key);
    }
}

void Menu::resetMenu() {
    while (!this->level.empty()) this->level.pop();
    this->level.push(Main);
    this->selectedIndex = 0;
    this->scrollOffset = 0;
}

void Menu::buildStartMenu(bool started) {
    switch (this->level.top()) {
        case Main:
            if (!started) {
                this->items.push_back(allItems.at(ItemNewGame));
            }
            if (RT_Context.savegameExists()) {
                if (!started) {
                    this->items.push_back(allItems.at(ItemContinue));
                } else {
                    this->items.push_back(allItems.at(ItemLoad));
                }
            }
            if (started) {
                this->items.push_back(allItems.at(ItemReloadScene));
            }
            this->items.push_back(allItems.at(ItemSettings));
            this->items.push_back(allItems.at(ItemQuit));
            return;
        case Settings:
            this->items.push_back(allItems.at(ItemAudioSettings));
            this->items.push_back(allItems.at(ItemVideoSettings));
            this->items.push_back(allItems.at(ItemKeyboardSettings));
            this->items.push_back(allItems.at(ItemGamepadSettings));
            this->items.push_back(allItems.at(ItemBack));
            return;
        case AreYouSure:
            this->items.push_back(allItems.at(ItemAreYouSure));
            this->items.push_back(allItems.at(ItemYes));
            this->items.push_back(allItems.at(ItemNo));
            return;
        case AudioSettings:
            this->items.push_back(allItems.at(ItemMusicVolume));
            this->items.push_back(allItems.at(ItemEffectsVolume));
            this->items.push_back(allItems.at(ItemBack));
            return;
        case VideoSettings:
            this->items.push_back(allItems.at(ItemWindowSize));
            this->items.push_back(allItems.at(ItemFps));
            this->items.push_back(allItems.at(ItemBack));
            return;
        case Keyboard:
            populateKeyboardMenu();
            this->items.push_back(allItems.at(ItemBack));
            return;
        case Gamepad:
            populateGamepadMenu();
            this->items.push_back(allItems.at(ItemBack));
            return;
        default:
            return;
    }
}

void Menu::buildMenu() {
    this->items.clear();

    auto state = RT_State.currentState();

    switch (state) {
        case StateStart:
            buildStartMenu(false);
            return;
        case StateMainMenu:
            buildStartMenu(true);
            return;
        default:
            return;
    }
}

void Menu::addGamepadMenuItem(GameKey k, SDL_GameControllerButton button) {
    auto bound = Input::toString(k);
    auto b = SDL_GameControllerGetStringForButton(button);
    items.push_back(
            std::make_unique<MenuItem>(bound.c_str(), b, [this, button](GameKeyEvent &key) {
                if (this->menuState == Normal && key.key != GK_A) return;
                if (!key.source) return;
                if (key.source->type != SDL_CONTROLLERBUTTONDOWN) return;
                if (this->menuState == Normal) {
                    this->menuState = AwaitBinding;
                } else if (this->menuState == AwaitBinding) {
                    Input::rebind(button, key.key);
                    this->menuState = Normal;
                }
            }));
}

void Menu::addKeyboardMenuItem(GameKey k, SDL_Keycode button) {
    auto bound = Input::toString(k);
    auto b = SDL_GetKeyName(button);
    items.push_back(
            std::make_unique<MenuItem>(bound.c_str(), b, [this, button](GameKeyEvent &key) {
                if (this->menuState == Normal && key.key != GK_A) return;
                if (!key.source) return;
                if (key.source->type != SDL_KEYDOWN) return;
                if (this->menuState == Normal) {
                    this->menuState = AwaitBinding;
                } else if (this->menuState == AwaitBinding) {
                    Input::rebind(button, key.key);
                    this->menuState = Normal;
                }
            }));
}

void Menu::populateKeyboardMenu() {
    SDL_Keycode bound;

    bound = Input::boundKey(GK_UP);
    addKeyboardMenuItem(GK_UP, bound);

    bound = Input::boundKey(GK_DOWN);
    addKeyboardMenuItem(GK_DOWN, bound);

    bound = Input::boundKey(GK_LEFT);
    addKeyboardMenuItem(GK_LEFT, bound);

    bound = Input::boundKey(GK_RIGHT);
    addKeyboardMenuItem(GK_RIGHT, bound);

    bound = Input::boundKey(GK_A);
    addKeyboardMenuItem(GK_A, bound);

    bound = Input::boundKey(GK_B);
    addKeyboardMenuItem(GK_B, bound);

    bound = Input::boundKey(GK_X);
    addKeyboardMenuItem(GK_X, bound);

    bound = Input::boundKey(GK_Y);
    addKeyboardMenuItem(GK_Y, bound);

    bound = Input::boundKey(GK_L);
    addKeyboardMenuItem(GK_L, bound);

    bound = Input::boundKey(GK_R);
    addKeyboardMenuItem(GK_R, bound);

    bound = Input::boundKey(GK_START);
    addKeyboardMenuItem(GK_START, bound);

    bound = Input::boundKey(GK_SELECT);
    addKeyboardMenuItem(GK_SELECT, bound);
}

void Menu::populateGamepadMenu() {
    SDL_GameControllerButton bound;

    bound = Input::bound(GK_UP);
    addGamepadMenuItem(GK_UP, bound);

    bound = Input::bound(GK_DOWN);
    addGamepadMenuItem(GK_DOWN, bound);

    bound = Input::bound(GK_LEFT);
    addGamepadMenuItem(GK_LEFT, bound);

    bound = Input::bound(GK_RIGHT);
    addGamepadMenuItem(GK_RIGHT, bound);

    bound = Input::bound(GK_A);
    addGamepadMenuItem(GK_A, bound);

    bound = Input::bound(GK_B);
    addGamepadMenuItem(GK_B, bound);

    bound = Input::bound(GK_X);
    addGamepadMenuItem(GK_X, bound);

    bound = Input::bound(GK_Y);
    addGamepadMenuItem(GK_Y, bound);

    bound = Input::bound(GK_L);
    addGamepadMenuItem(GK_L, bound);

    bound = Input::bound(GK_R);
    addGamepadMenuItem(GK_R, bound);

    bound = Input::bound(GK_START);
    addGamepadMenuItem(GK_START, bound);

    bound = Input::bound(GK_SELECT);
    addGamepadMenuItem(GK_SELECT, bound);
}

MenuItem::MenuItem(std::string key) : key(key), cb(nullptr), canSelect(false) {}

MenuItem::MenuItem(std::string key, menu_Callback cb) : key(key), cb(cb), canSelect(true) {}

MenuItem::MenuItem(std::string key, std::string *value, menu_Callback cb) : key(key), value(value), cb(cb),
                                                                            canSelect(true) {}

MenuItem::MenuItem(const char *key, const char *value, menu_Callback cb) : key(key), value2(value), cb(cb),
                                                                           canSelect(true) {}


void MenuItem::call(GameKeyEvent &key) {
    if (this->cb) cb(key);
}

static void renderText(const char *text, SDL_Color color, int x, int y) {
    TTF_Font *f = Font::instance().get();
    SDL_Surface *surface = TTF_RenderUTF8_Blended(f, text, color);
    if (!surface) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(Gfx_Renderer, surface);
    SDL_Rect target;
    target.x = x;
    target.y = y;
    target.w = surface->w;
    target.h = surface->h;
    SDL_RenderCopy(Gfx_Renderer, texture, nullptr, &target);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void MenuItem::render(bool selected, int x, int y, int w, int h) {
    SDL_Color textColor = {255, 255, 255, 255};

    if (selected) {
        Color white{255, 255, 255, 255};
        SDL_Rect bg;
        bg.x = x;
        bg.y = y;
        bg.w = w;
        bg.h = h;
        Draw::instance().box(white, bg);
        textColor = {0, 0, 0, 255};
    }

    int textY = y + (h - Font::instance().lineHeight()) / 2;

    if (this->value || this->value2.size() > 0) {
        // Two columns: key on left, value on right
        renderText(this->key.c_str(), textColor, x + 4, textY);
        const char *val = this->value ? this->value->c_str() : this->value2.c_str();
        renderText(val, textColor, x + w / 2, textY);
    } else {
        // Single column: centered text
        int textW = 0;
        TTF_SizeUTF8(Font::instance().get(), this->key.c_str(), &textW, nullptr);
        int textX = x + (w - textW) / 2;
        renderText(this->key.c_str(), textColor, textX, textY);
    }
}
