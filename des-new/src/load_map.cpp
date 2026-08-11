#include "raylib.h"
#include "tmxlite/Layer.hpp"
#include "tmxlite/ObjectGroup.hpp"
#include "tmxlite/Tileset.hpp"
#include "tmxlite/Types.hpp"
#include <cstdint>
#include <format>
#include <print>
#include <string>
#include <tmxlite/Map.hpp>
#include <vector>
#include <tmxlite/TileLayer.hpp>
#include <unordered_map>

class TileMap {
    private:
        int tile_width = 16;
        int tile_height = 16;
        std::vector<tmx::Tileset> tilesets;
        std::unordered_map<std::string, Texture2D> textures;
        tmx::Map map;

        // Returns the tileset of a gid
        tmx::Tileset* find_tileset(uint32_t gid) {

            tmx::Tileset* output = nullptr;
            for (auto& tileset: tilesets) {
                if (gid > tileset.getLastGID()) {
                    continue;
                } else {
                    output = &tileset;
                    break;
                }
            }

            return output;
        }

        void create_textures() {
            for (auto& tileset : tilesets) {
                Image image = LoadImage(tileset.getImagePath().c_str());
                Texture2D texture = LoadTextureFromImage(image);
                textures.emplace(tileset.getName(), texture);
                UnloadImage(image);
            }
        }


    public:
        TileMap(std::string name) {
            auto path = std::format("./assets/tilemaps/{}", name);
            if (!map.load(path)) {
                std::println("Failed to load map from: {}", path);
            }

            tilesets = map.getTilesets();
            create_textures();
        }

        ~TileMap() {
            for (auto& [_, texture] : textures) {
                UnloadTexture(texture);
            }
        }

        tmx::Vector2f getSpawnPoint() {
            const auto& layers = map.getLayers();

            for (const auto& layer : layers) {

                if (layer->getType() != tmx::Layer::Type::Object) {
                    continue;
                }

                if (layer->getName() != "Markers") {
                    continue;
                }

                auto* object_layer = dynamic_cast<const tmx::ObjectGroup*>(layer.get());

                for (const auto& object : object_layer->getObjects()) {
                    if (object.getName() == "SpawnPoint") {
                        return object.getPosition();
                    }
                }
            }

            return tmx::Vector2f {
                0,
                0,
            };
        }

        tmx::Vector2<unsigned int> getMapSize() {
            return map.getTileCount() * map.getTileSize();
        }

        void render() {
            const auto& layers = map.getLayers();

            for (const auto& layer : layers) {

                if (layer->getType() != tmx::Layer::Type::Tile) {
                    continue;
                }

                auto& size = layer->getSize();

                auto* tile_layer = dynamic_cast<const tmx::TileLayer*>(layer.get());

                auto& tiles = tile_layer->getTiles();

                for (unsigned y = 0; y < size.y; y++) {
                    for (unsigned x = 0; x < size.x; x++) {

                        auto& tile = tiles[y * size.x + x];

                        uint32_t gid = tile.ID;

                        if (gid == 0)
                            continue;

                        auto tileset = find_tileset(gid);

                        uint32_t local = gid - tileset->getFirstGID();

                        Rectangle source {
                            static_cast<float>((local % tileset->getColumnCount()) * tile_width),
                            static_cast<float>((local / tileset->getColumnCount()) * tile_height),
                            static_cast<float>(tile_width),
                            static_cast<float>(tile_height)
                        };

                        Rectangle dist {
                            static_cast<float>(x * tile_width),
                            static_cast<float>(y * tile_height),
                            16,
                            16
                        };


                        auto texture = textures.at(tileset->getName());
                        Vector2 origin {static_cast<float>(tile_width / 2), static_cast<float>(tile_height / 2)};

                        DrawTexturePro(texture, source, dist, origin, 0, WHITE);

                    }
                }
            }
        }
};
