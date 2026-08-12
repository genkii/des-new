#include <tmxlite/Map.hpp>
#include <raylib.h>
#include "load_map.cpp"
#include <asio.hpp>
#include <msgpack.hpp>
#include "network/packets/hello.hpp"
#include "network/sender.cpp"

int main() {

    PacketSender sender = PacketSender();

    sender.send_packet(Hello {
        .player_name = "genki",
        .password = "test123",
    });

    std::println("Packet sent");

    const int screenWidth = 800;
    const int screenHeight = 450;

    constexpr float cameraSpeed = 200.0f;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Dungeon Escape");

    // Uses Raylib functions so must be after InitWindow
    TileMap map{"map1.tmx"};

    Camera2D camera;

    auto spawn_point = map.getSpawnPoint();

    camera.target = {spawn_point.x, spawn_point.y};
    camera.offset = {400, 300};
    camera.zoom = 5.0f;
    camera.rotation = 0.0f;

    SetTargetFPS(60);

    constexpr double tickRate = 1.0 / 60.0;
    double accumulator = 0.0;

    while (!WindowShouldClose()) {

        accumulator += GetFrameTime();

        // 60Hz
        while (accumulator >= tickRate) {

            if (IsKeyDown(KEY_W)) camera.target.y -= cameraSpeed * tickRate;
            if (IsKeyDown(KEY_S)) camera.target.y += cameraSpeed * tickRate;
            if (IsKeyDown(KEY_A)) camera.target.x -= cameraSpeed * tickRate;
            if (IsKeyDown(KEY_D)) camera.target.x += cameraSpeed * tickRate;

            camera.target.x = round(camera.target.x);
            camera.target.y = round(camera.target.y);

            // Simulation loop
            accumulator -= tickRate;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode2D(camera);

        map.render();

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndMode2D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
