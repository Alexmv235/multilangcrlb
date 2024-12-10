#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cjson/cJSON.h"
#include "fileLoaderSnap.h"  // Incluir la nueva biblioteca

#define MAX_TRANSLATIONS 100

typedef struct {
    char key[50];
    char value[100];
} Translation;

Translation translations[MAX_TRANSLATIONS];
int translation_count = 0;

// Helper function to determine the language file path using fileLoaderSnap
const char *GetLanguageFilePath(const char *relativePath) {
    return GetFilePath(relativePath);  // Usar la función de fileLoaderSnap para obtener la ruta correcta
}

// Load translations from a JSON file
void LoadLanguage(const char *filename) {
    char *buffer = LoadFile(filename);
    if (!buffer) return;

    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        free(buffer);
        return;
    }

    translation_count = 0;
    cJSON *current = json->child;
    while (current && translation_count < MAX_TRANSLATIONS) {
        strncpy(translations[translation_count].key, current->string, sizeof(translations[translation_count].key) - 1);
        strncpy(translations[translation_count].value, cJSON_GetStringValue(current), sizeof(translations[translation_count].value) - 1);
        translation_count++;
        current = current->next;
    }

    cJSON_Delete(json);
    free(buffer);
}

// Get a translation for a given key
const char *Translate(const char *key) {
    for (int i = 0; i < translation_count; i++) {
        if (strcmp(translations[i].key, key) == 0) {
            return translations[i].value;
        }
    }
    return key;
}

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    // Load the splash screen image
    Texture2D splashTexture = LoadTexture(GetLanguageFilePath("resources/splash_image.png"));

    // Time for the splash screen
    float splashTime = 4.0f; // Total time for splash screen (fade in + fade out)
    float fadeInTime = 1.0f; // Time for fade in
    float fadeOutTime = 1.0f; // Time for fade out

    float elapsedTime = 0.0f;
    float fadeAlpha = 0.0f; // Initial opacity (fully transparent)
    bool fadeIn = true;

    // Load default language
    LoadLanguage("i18n/lang_en.json");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main splash screen loop
    while (!WindowShouldClose()) {
        // Update splash screen time
        elapsedTime += GetFrameTime();

        if (elapsedTime < fadeInTime) {
            // Fade In
            fadeAlpha = (elapsedTime / fadeInTime);  // Gradually increase alpha from 0 to 1
        } else if (elapsedTime < splashTime - fadeOutTime) {
            // Maintain full opacity during the middle of the splash
            fadeAlpha = 1.0f;
        } else {
            // Fade Out
            fadeAlpha = 1.0f - ((elapsedTime - (splashTime - fadeOutTime)) / fadeOutTime);  // Gradually decrease alpha from 1 to 0
        }

        if (elapsedTime >= splashTime) {
            break;  // Exit splash screen after the total time has passed
        }

        // Draw splash screen with fade effect
        BeginDrawing();
        ClearBackground(RAYWHITE);
        // Draw the splash screen image with the calculated alpha
        DrawTexture(splashTexture, screenWidth / 2 - splashTexture.width / 2, screenHeight / 2 - splashTexture.height / 2, (Color){255, 255, 255, (unsigned char)(fadeAlpha * 255)});
        EndDrawing();
    }

    // Unload splash screen texture before starting main game
    UnloadTexture(splashTexture);

    // Main game loop for the rest of the application
    while (!WindowShouldClose()) {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_L)) {
            // Toggle between languages when "L" is pressed
            static int currentLanguage = 0;
            currentLanguage = 1 - currentLanguage;
            if (currentLanguage == 0) LoadLanguage("i18n/lang_en.json");
            else LoadLanguage("i18n/lang_es.json");
        }
        //----------------------------------------------------------------------------------

        // Draw main application window
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText(Translate("congrats"), 190, 200, 20, LIGHTGRAY);
        DrawText(Translate("press_l_to_change_language"), 190, 250, 20, DARKGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
